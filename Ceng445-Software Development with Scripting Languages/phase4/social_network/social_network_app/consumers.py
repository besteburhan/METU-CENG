from asgiref.sync import async_to_sync
from channels.generic.websocket import WebsocketConsumer
import json
from .models import *
from datetime import datetime
from django.db.models import Q
from isbnlib import meta


class AnnouncementConsumer(WebsocketConsumer):
    def connect(self):
        self.room_name = "announcement"
        self.room_group_name = 'announcement'

        # Join room group
        async_to_sync(self.channel_layer.group_add)(
            self.room_group_name,
            self.channel_name
        )

        self.accept()

    def disconnect(self, close_code):
        # Leave room group
        async_to_sync(self.channel_layer.group_discard)(
            self.room_group_name,
            self.channel_name
        )

    # Receive message from WebSocket
    def receive(self, text_data):
        text_data_json = json.loads(text_data)
        message = text_data_json['msg']
        states = text_data_json['states']
        item_id = text_data_json['item_id']
        item = Item.objects.get(id=item_id)
        announcement = []
        announcement.append(Announcement.objects.create(item_id=item_id, friend_state=int(states), msg=message,
                                    date=datetime.now()).id)



        # Send message to room group
        async_to_sync(self.channel_layer.group_send)(
            self.room_group_name,
            {
                'type': 'announcement_handler',
                'announcement_ids': announcement
            }
        )

    # Receive message from room group
    def announcement_handler(self, event):
        announcement_ids = event['announcement_ids']

        # Send message to WebSocket
        self.send(text_data=json.dumps({
            'announcement_ids': announcement_ids
        }))



class NotificationConsumer(WebsocketConsumer):
    def connect(self):
        self.room_name = "notification"
        self.room_group_name = 'notification'

        # Join room group
        async_to_sync(self.channel_layer.group_add)(
            self.room_group_name,
            self.channel_name
        )

        self.accept()

    def disconnect(self, close_code):
        # Leave room group
        async_to_sync(self.channel_layer.group_discard)(
            self.room_group_name,
            self.channel_name
        )

    # Receive message from WebSocket
    def receive(self, text_data):
        text_data = json.loads(text_data)
        func_name = text_data['func_name']
        if func_name == "set_state":
            state_names = text_data['state_names']
            states = text_data['states']
            item_id = text_data['item_id']

            item = Item.objects.get(pk=item_id)
            item.__setattr__(state_names, int(states))
            if item.detail > item.view:
                item.view = item.detail
                # messages.warning(request,
                #                  'Detail permission cannot have higher priority than view permission. View permission also be set ' +
                #                  STATE_TYPE[int(item.detail)])

            if item.borrow > item.view:
                item.borrow = item.view
                # messages.warning(request,
                #                  'Borrow permission cannot have higher priority than view permission. Borrow permission also be set ' +
                #                  STATE_TYPE[int(item.view)])

            notification = []
            if state_names == 'borrow':
                requests = WatchRequest.objects.filter(Q(item=item) & Q(watch_method=1))
                text = f'{item.owner.first_name} {item.owner.last_name} changed borrow state to {STATE_TYPE[int(states)]} for the item with title {item.title}.'
                for cr in requests:
                    notification.append(Notification.objects.create(sender_user=item.owner, receiver_user=cr.user, item_id=item.id,
                                                text=text, date=datetime.now()).id)
            if state_names == 'comment':
                requests = WatchRequest.objects.filter(Q(item=item) & Q(watch_method=0))
                text = f'{item.owner.first_name} {item.owner.last_name} changed comment state to {STATE_TYPE[int(states)]} for the item with title {item.title}.'
                for cr in requests:
                    notification.append(Notification.objects.create(sender_user=item.owner, receiver_user=cr.user, item_id=item.id,
                                                text=text, date=datetime.now()).id)
            item.save()
        elif func_name == "delete_item":
            item = Item.objects.get(pk=text_data['item'])
            borrow_requests = BorrowRequest.objects.filter(Q(item=item))
            text = f'{item.owner.first_name} {item.owner.last_name} deleted the item with title {item.title} requested by you.'
            notification = []
            for br in borrow_requests:
                notification.append(Notification.objects.create(sender_user=item.owner, receiver_user=br.user, text=text,
                                            date=datetime.now()).id)
            item.delete()
        elif func_name == 'add_item':
            item = Item()
            item.owner_id = text_data['user_id']
            user = User.objects.get(id=text_data['user_id'])
            item.type = text_data['type']
            item.title = text_data['title']
            item.artist = text_data['artist']
            item.genre = text_data['genre']
            item.year = text_data['year']
            item.location = text_data['location']
            item.view = text_data['view']
            item.detail = text_data['detail']
            item.borrow = text_data['borrow']
            item.comment = text_data['comment']
            item.search = text_data['search']
            if text_data['isbn'] != '':
                metadata = None
                try:

                    metadata = meta(isbn=str(text_data['isbn']))
                except:
                    pass
                if metadata is None:
                    messages.warning('Meta data couldn\'t be obtained')
                else:
                    item.title = metadata['Title']
                    item.year = metadata['Year']
                    item.artist = metadata['Authors'][0]

            if item.detail > item.view:
                item.view = item.detail
                # messages.warning(request,
                #                  'Detail permission cannot have higher priority than view permission. View permission also be set ' +
                #                  STATE_TYPE[int(item.detail)])

            if item.borrow > item.view:
                item.borrow = item.view
                # messages.warning(request,
                #                  'Borrow permission cannot have higher priority than view permission. Borrow permission also be set ' +
                #                  STATE_TYPE[int(item.view)])

            # if item.detail > item.comment:
            #     item.comment = item.detail
            #     messages.warning(request, 'Detail permission cannot have higher priority than comment permission. Comment permission also be set ' + STATE_TYPE[int(item.detail)])
            item.save()
            #
            watch_requests = WatchRequest.objects.filter(Q(followed_user=user) & Q(watch_method=2))
            text = f'{user.first_name} {user.last_name} added new item -> with title : {item.title}'
            notification = []
            for wr in watch_requests:
                notification.append(Notification.objects.create(sender_user=user, receiver_user=wr.user, text=text,
                                                           date=datetime.now()).id)
        elif func_name == "make_comment":
            comment_text = text_data['comment_text']
            user = User.objects.get(pk = text_data['user_id'])
            item = Item.objects.get(pk=text_data['item_id'])
            Comment(user=user, item=item, text=comment_text, date=datetime.now()).save()

            comment_requests = WatchRequest.objects.filter(Q(item=item) & Q(watch_method=0))
            text = f'{user.first_name} {user.last_name} commented for the item with title {item.title}.'
            notification = []
            for cr in comment_requests:
                notification.append(Notification.objects.create(sender_user=user, receiver_user=cr.user, item_id=item.id,
                                            text=text, date=datetime.now()).id)


        # Send message to room group
        async_to_sync(self.channel_layer.group_send)(
            self.room_group_name,
            {
                'type': 'notification_handler',
                'notification_ids': notification,
            }
        )

    # Receive message from room group
    def notification_handler(self, event):
        notification_ids = event['notification_ids']

        # Send message to WebSocket
        self.send(text_data=json.dumps({
            'notification_ids': notification_ids
        }))