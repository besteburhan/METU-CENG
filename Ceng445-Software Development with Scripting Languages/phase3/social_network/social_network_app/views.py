from django.shortcuts import render, redirect
from django.views.generic import View
from social_network_app import forms
from django.urls import reverse
from .models import *
from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.decorators import login_required
from django.http import JsonResponse, HttpResponseRedirect, HttpResponse, HttpResponseForbidden
from django.db.models import Q
from isbnlib import meta
from datetime import datetime, timedelta
from secrets import token_hex
from django.contrib import messages
import string
from random import *

user_verification_dict = {}


class IndexView(View):
    def get(self, request):
        items = {}
        if request.user.id:
            items = Item.objects.filter(owner=request.user)
        return render(request, 'index.html', {'items': items})

    def post(self, request):
        old_pass = request.POST['old_password']
        new_pass = request.POST['new_password']
        usr = User.objects.get(pk=request.user.id)
        if old_pass != "":
            if usr.check_password(old_pass):
                usr.set_password(new_pass)
                usr.save()

                return redirect("/")
            else:
                messages.warning(request, "Wrong Password, if you want to get new password, please do not fill old password field.")
                return redirect("/")
        else:
            characters = string.ascii_letters + string.punctuation + string.digits
            generated_password = "".join(choice(characters) for x in range(randint(8, 16)))
            usr.set_password(generated_password)
            usr.save()
            print("Generated Password : ",generated_password)

            return redirect("/")


class RegisterView(View):
    user_form = forms.UserForm()

    def get(self, request):
        return render(request, 'register.html', {'user_form': self.user_form})

    def post(self, request):
        self.user_form = forms.UserForm(data=request.POST)
        if self.user_form.is_valid():
            user = self.user_form.save(commit=False)
            user.set_password(self.user_form.cleaned_data['password'])
            user.username = self.user_form.cleaned_data['email'].lower()
            user.is_active = False
            user.save()
            characters = string.ascii_letters + string.punctuation + string.digits
            generated_password = token_hex(16)
            user_verification_dict[user] = generated_password
            print("You registered successfully. Please use the verification number to log-in. Verification number: ", generated_password)
            return HttpResponseRedirect(reverse('social_network_app:login'))
        else:
            print(self.user_form.errors)
            return render(request, 'register.html', {'user_form': self.user_form})


class LoginView(View):
    login_form = forms.LoginForm()

    def get(self, request):
        return render(request, 'login.html', {'login_form': self.login_form})

    def post(self, request):
        self.login_form = forms.LoginForm(data=request.POST)
        try:
            usr = User.objects.get(email=request.POST['email'])

        except:
            print('Login failed')
            return HttpResponse("invalid login details")
        if self.login_form.is_valid() and usr.is_active:
            user = authenticate(username=self.login_form.cleaned_data['email'], password=self.login_form.cleaned_data['password'])
            if user and user.is_active:
                login(request, user)
                return redirect('/')
            else:
                return HttpResponse("invalid login details")
        else:
            print('Login failed')
            return HttpResponse("invalid login details")


class VerificationView(View):
    verification_form = forms.VerificationForm()

    def get(self, request):
        return render(request, 'verification.html', {'verification_form': self.verification_form})

    def post(self, request):
        self.verification_form = forms.VerificationForm(data=request.POST)
        try:
            usr = User.objects.get(email=request.POST['email'])

        except:
            return HttpResponse("invalid verification details")
        if user_verification_dict[usr] == request.POST['verification'] and usr.check_password(request.POST['password']):
            usr.is_active = 1
            usr.save()
            return redirect('/login/')

        else:
            messages.warning(request, "Verification failed.")
            return redirect('/verification/')


class LogoutView(View):
    def get(self, request):
        logout(request)
        return HttpResponseRedirect(reverse('index'))


class UsersView(View):
    def get(self, request, user_id):
        location_available_items = {}
        location_unavailable_items = {}
        borrow_requests = {}
        usr = User.objects.get(id=user_id)
        if user_id:
            if request.user.id != user_id:
                # location null if it is borrowed
                try:
                    br = Borrow.objects.filter(is_returned=0).values_list('item', flat=True)
                except:
                    br = []
                friend_state = 0
                try:
                    friend_state = Friend.objects.get((Q(sender_user=user_id ) & Q(receiver_user=request.user)) | (Q(receiver_user=user_id ) & Q(sender_user=request.user))).state
                except:
                    pass
                if friend_state == 0:
                    location_available_items = Item.objects.filter(Q(owner=user_id) & Q(view=3) & ~Q(id__in=br))
                    location_unavailable_items = Item.objects.filter(Q(owner=user_id) & Q(view=3) & Q(id__in=br))
                else:
                    location_available_items = Item.objects.filter(Q(owner=user_id) & Q(view__gte=friend_state) & ~Q(id__in=br))
                    location_unavailable_items = Item.objects.filter(Q(owner=user_id) & Q(view__gte=friend_state) & Q(id__in=br))
            else:  # own items

                location_available_items = Item.objects.filter(Q(owner=request.user))


        return render(request, 'users.html', {'location_unavailable_items': location_unavailable_items, 'location_available_items': location_available_items,
                                                        'borrow_requests': borrow_requests, 'user_viewed':usr})

    def post(self, request, user_id):

        if user_id == request.user.id:
            if request.POST['submit_type'] == 'save':
                item = Item()
                item.owner_id = int(user_id)
                item.type = request.POST['type']
                item.title = request.POST['title']
                item.artist = request.POST['artist']
                item.genre = request.POST['genre']
                item.year = request.POST['year']
                item.location = request.POST['location']
                item.view = request.POST['view']
                item.detail = request.POST['detail']
                item.borrow = request.POST['borrow']
                item.comment = request.POST['comment']
                item.search = request.POST['search']
                if request.POST['isbn'] != '':
                    metadata = None
                    try:

                        metadata = meta(isbn=str(request.POST['isbn']))
                    except:
                        messages.warning(request, 'ISBN Number is wrong! Item couldn\'t be added' )
                        return redirect('/users/' + str(user_id))
                    if metadata is None:
                        messages.warning(request, 'Meta data couldn\'t be obtained' )
                        return redirect('/users/' + str(user_id))
                    else:
                        item.title = metadata['Title']
                        item.year = metadata['Year']
                        item.artist = metadata['Authors'][0]

                if item.detail > item.view:
                    item.view = item.detail
                    messages.warning(request, 'Detail permission cannot have higher priority than view permission. View permission also be set ' + STATE_TYPE[int(item.detail)])

                if item.borrow > item.view:
                    item.borrow = item.view
                    messages.warning(request, 'Borrow permission cannot have higher priority than view permission. Borrow permission also be set ' + STATE_TYPE[int(item.view)])

                # if item.detail > item.comment:
                #     item.comment = item.detail
                #     messages.warning(request, 'Detail permission cannot have higher priority than comment permission. Comment permission also be set ' + STATE_TYPE[int(item.detail)])
                item.save()

                watch_requests = WatchRequest.objects.filter(Q(followed_user=request.user) & Q(watch_method=2))
                text = f'{request.user.first_name} {request.user.last_name} added new item -> with title : {item.title}'
                for wr in watch_requests:
                    Notification.objects.create(sender_user=request.user, receiver_user=wr.user, text=text, date=datetime.now())

                messages.success(request, 'Item is added successfully.')
                return redirect('/users/' + str(user_id))
            elif request.POST['submit_type'] == 'returned':
                borrow_item = Borrow.objects.get(item_id=request.POST['item_id'], is_returned=0)
                borrow_item.is_returned = 1
                borrow_item.save()

                borrow_requests = WatchRequest.objects.filter(Q(item=borrow_item.item) & Q(watch_method=1))
                text = f'Item with title: {borrow_item.item.title} is returned to its owner.'
                for br in borrow_requests:
                    Notification.objects.create(sender_user=borrow_item.item.owner, receiver_user=br.user, item_id=borrow_item.item.id, text=text, date=datetime.now())

                messages.success(request, 'Item is returned.')
                return redirect('/users/' + str(user_id))
            elif request.POST['submit_type'] == 'change_location':
                item = Item.objects.get(id=request.POST['item_id'])
                item.location = request.POST['location']
                item.save()
                messages.success(request, 'Location is changed.')
                return redirect('/users/' + str(user_id))
            elif request.POST['submit_type'] == 'delete':
                item = Item.objects.get(id=request.POST['item_id'])

                borrow_requests = BorrowRequest.objects.filter(Q(item=item))
                text = f'{request.user.first_name} {request.user.last_name} deleted the item with title {item.title} requested by you.'
                for br in borrow_requests:
                    Notification.objects.create(sender_user=request.user, receiver_user=br.user, text=text, date=datetime.now())

                item.delete()
                messages.success(request, 'Item is deleted.')
                return redirect('/users/' + str(user_id))

            else:  # lend
                item_id = request.POST['item_id']

                if request.POST['returned_date'] == '':
                    taking_date = datetime.now()
                    return_date = taking_date + timedelta(weeks=2)
                    Borrow(user_id=request.POST['requested_user'], item_id=item_id, taken_date=datetime.now(), returned_date=return_date).save()
                else:
                    Borrow(user_id=request.POST['requested_user'], item_id=item_id, taken_date=datetime.now(), returned_date=request.POST['returned_date']).save()
                borrow_request = BorrowRequest.objects.get(user_id=request.POST['requested_user'], item_id=item_id)
                borrow_request.delete()
                messages.warning(request, 'Item lent')
                return redirect(('/users/' + str(user_id)))
        else:
            if request.POST['submit_type'] == 'watch':
                WatchRequest.objects.create(user=request.user, followed_user_id=user_id, watch_method=2)
                return redirect('/users/' + str(user_id))
            elif request.POST['submit_type'] == 'unwatch':
                watch_request = WatchRequest.objects.get(user=request.user, followed_user_id=user_id, watch_method=2)
                watch_request.delete()
                return redirect('/users/' + str(user_id))


class FriendsView(View):
    def get(self, request):
        friends = {}
        if request.user:
            friends = Friend.objects.filter(Q(sender_user=request.user) & ~Q(state=0))
            friends = friends | Friend.objects.filter(Q(receiver_user=request.user) & ~Q(state=0))

        friend_requests = {}
        if request.user:
            friend_requests = Friend.objects.filter(Q(receiver_user=request.user) & Q(state=0))

        return render(request, 'friendships.html', {'friends': friends, 'friend_requests':friend_requests })

    def post(self, request):
        selected_value = int(request.POST['selectpicker'])
        friend_req_id = int(request.POST['friend_id'])
        friend_request = Friend.objects.get(id=friend_req_id)
        friend_request.state = selected_value
        friend_request.save()
        return redirect('/friendships/')


class BorrowRequestsView(View):
    def get(self, request):
        borrow_requests = {}
        if request.user:
            borrow_requests = BorrowRequest.objects.filter(Q(user=request.user) & ~Q(item__owner=request.user) ).order_by('request_date')
        # TODO borrowedlanınca requestlerden sill!!!!!
        borrowed_items = Borrow.objects.filter(user=request.user, is_returned=0)

        returned_items = Borrow.objects.filter(user=request.user, is_returned=1)

        return render(request, 'borrow_requests.html', {'borrow_requests': borrow_requests, 'borrowed_items': borrowed_items, 'returned_items': returned_items })

    def post(self, request):
        # rate
        rate = request.POST['rate']
        borrow_id = request.POST['borrow_id']
        borrow = Borrow.objects.get(id=borrow_id)
        borrow.rate = rate
        borrow.save()
        return redirect('/borrow_requests/')


class AllItemsView(View):
    def get(self, request):
        # viewable
        try:
            s_users = Friend.objects.filter(receiver_user=request.user).values_list('sender_user', flat=True)
            r_users = Friend.objects.filter(sender_user=request.user).values_list('receiver_user', flat=True)
        except:
            s_users = []
            r_users = []
        viewable_items = Item.objects.filter(~Q(view=0) & ~Q(owner=request.user) &
                                               ( Q(view=3) |
                                                (Q(owner_id__in=s_users) & ( ( Q(owner__friend_sender_user__state=2 ) & Q(view=2)) | ( Q(owner__friend_sender_user__state=1 ) & Q(view__gte=1))) )
                                                 | ( Q(owner_id__in=r_users) &( ( Q(owner__friend_receiver_user__state=2 ) & Q(view=2)) | ( Q(owner__friend_receiver_user__state=1 ) & Q(view__gte=1))))))
        # borrowable
        already_requested = BorrowRequest.objects.filter(user=request.user).values_list('item', flat=True)
        borrowable_items = Item.objects.filter(~Q(id__in=already_requested) & ~Q(owner=request.user) & ~Q(borrow=0) &
                                               (Q(borrow=3) |
                                                (Q(owner_id__in=s_users) & (
                                                            (Q(owner__friend_sender_user__state=2) & Q(borrow=2)) | (
                                                                Q(owner__friend_sender_user__state=1) & Q(borrow__gte=1))))
                                                | (Q(owner_id__in=r_users) & ((Q(owner__friend_receiver_user__state=2) &
                                                                               Q(borrow=2)) | (Q(owner__friend_receiver_user__state=1) & Q(borrow__gte=1))))))

        viewable_items = viewable_items.exclude(pk__in=borrowable_items)
        return render(request, 'all_items.html', {'borrowable_items': borrowable_items, 'viewable_items': viewable_items})

    def post(self, request):
        if request.POST['submit_type'] == 'search':

            search_text = request.POST['search_text']
            try:
                if request.POST['for_borrow']:
                    for_borrow = True
            except:
                for_borrow = False
            genre = request.POST['genre']
            year = request.POST['year']

            if year == '':
                year = None
            else:
                year = int(year)

            words = list(search_text.split(" "))
            s_friends = Friend.objects.filter(Q(receiver_user=request.user)).values('sender_user', 'state')
            r_friends = Friend.objects.filter(Q(sender_user=request.user)).values('receiver_user', 'state')
            viewable_items = {}
            borrowable_items = {}
            receivers = []
            receiver_states = []
            senders = []
            senders_states = []
            for dict_ in s_friends:
                senders.append(dict_['sender_user'])
                senders_states.append(dict_['state'])
            for dict_ in r_friends:
                receivers.append(dict_['receiver_user'])
                receiver_states.append(dict_['state'])

            if for_borrow:
                borrowable_items = Item.objects.filter(~Q(owner=request.user) &
                    ~Q(borrow=0) & ( Q(borrow=3) | ( (Q(borrow=1) & ( Q(owner_id__in=senders) & Q(owner__friend_sender_user__state=1))) |
                                                     ( Q(borrow=2) & ( Q(owner_id__in=senders) & Q(owner__friend_sender_user__state__gte=1)))) |
                                     ((Q(borrow=1) & ( Q(owner_id__in=receivers) & Q(owner__friend_receiver_user__state=1))) |
                                      ( Q(borrow=2) & (Q(owner_id__in=receivers) & Q(owner__friend_sender_user__state__gte=1))))))

                for word in words:
                    borrowable_items = borrowable_items.filter(Q(artist__contains=word) | Q(title__contains=word))
                    if genre != '':
                        borrowable_items = borrowable_items.filter(genre__contains=genre)
                        if year:
                            borrowable_items = borrowable_items.filter(year__gte=year)
            else:
                viewable_items = Item.objects.filter(~Q(owner=request.user) & ~Q(view=0) & ( Q(view=3) | ( (Q(view=1) & (
                        Q(owner_id__in=senders) & Q(owner__friend_sender_user__state=1))) | ( Q(view=2) & ( Q(owner_id__in=senders) & Q(owner__friend_sender_user__state__gte=1)))) |
                                         (( Q(view=1) & ( Q(owner_id__in=receivers) & Q(owner__friend_receiver_user__state=1))) |
                                          ( Q(view=2) & (Q(owner_id__in=receivers) & Q(owner__friend_sender_user__state__gte=1))))))
                for word in words:
                    viewable_items = viewable_items.filter(Q(artist__contains=word) | Q(title__contains=word))
                    if genre != '':
                        viewable_items = viewable_items.filter(genre__contains=genre)
                        if year:
                            viewable_items = viewable_items.filter(year__gte=year)

        elif request.POST['submit_type'] == 'borrow':
            BorrowRequest(user=request.user, item_id=request.POST['item_id'], request_date=datetime.now()).save()
            return redirect('/all_items/' )

        return render(request, 'all_items.html', {'borrowable_items': borrowable_items, 'viewable_items': viewable_items})


class ItemView(View):
    def get(self, request, item_id):
        if item_id:
            item = Item.objects.get(id=item_id)
            try:
                state = Friend.objects.get(Q(sender_user=request.user) & Q(receiver_user=item.owner) |
                                           Q(receiver_user=request.user) & Q(sender_user=item.owner)).state
            except:
                state = 0
            viewable = False
            detailable = False
            if (item.view >= state and state != 0) or item.view == 3:
                viewable = True
            if (item.detail >=state and state != 0) or item.detail == 3:
                detailable = True

            comments = Comment.objects.filter(item=item_id)

        return render(request, 'item.html', {'item': item, 'comments': comments})

    def post(self, request, item_id):
        item = Item.objects.get(id=item_id)
        if request.POST['submit_type'] == "save_comment":
            comment_text = request.POST['comment_text']
            Comment(user=request.user, item=item, text=comment_text, date=datetime.now()).save()

            comment_requests = WatchRequest.objects.filter(Q(item=item) & Q(watch_method=0))
            text = f'{request.user.first_name} {request.user.last_name} commented for the item with title {item.title}.'
            for cr in comment_requests:
                Notification.objects.create(sender_user=request.user, receiver_user=cr.user, item_id=item.id,
                                            text=text, date=datetime.now())

            return redirect('/item/' + str(item_id))
        elif request.POST['submit_type'] == "set_state":
            item.__setattr__(request.POST['state_names'], int(request.POST['states']))
            item.save()
            if item.detail > item.view:
                item.view = item.detail
                messages.warning(request,
                                 'Detail permission cannot have higher priority than view permission. View permission also be set ' +
                                 STATE_TYPE[int(item.detail)])

            if item.borrow > item.view:
                item.borrow = item.view
                messages.warning(request,
                                 'Borrow permission cannot have higher priority than view permission. Borrow permission also be set ' +
                                 STATE_TYPE[int(item.view)])

            # if item.detail > item.comment:
            #     item.comment = item.detail
            #     messages.warning(request,
            #                      'Detail permission cannot have higher priority than comment permission. Comment permission also be set ' +
            #                      STATE_TYPE[int(item.detail)])

            if request.POST['state_names'] == 'borrow':
                requests = WatchRequest.objects.filter(Q(item=item) & Q(watch_method=1) )
                text = f'{request.user.first_name} {request.user.last_name} changed borrow state to {STATE_TYPE[int(request.POST["states"])]} for the item with title {item.title}.'
                for cr in requests:
                    Notification.objects.create(sender_user=request.user, receiver_user=cr.user, item_id=item.id,
                                                text=text, date=datetime.now())
            if request.POST['state_names'] == 'comment':
                requests = WatchRequest.objects.filter(Q(item=item) & Q(watch_method=0))
                text = f'{request.user.first_name} {request.user.last_name} changed comment state to {STATE_TYPE[int(request.POST["states"])]} for the item with title {item.title}.'
                for cr in requests:
                    Notification.objects.create(sender_user=request.user, receiver_user=cr.user, item_id=item.id,
                                                text=text, date=datetime.now())

            item.save()
            return redirect('/item/' + str(item_id))
        elif request.POST['submit_type'] == 'comment_watching':
            WatchRequest.objects.create(user=request.user, item=item, watch_method=0)
            return redirect('/item/' + str(item_id))
        elif request.POST['submit_type'] == 'comment_unwatching':
            wr = WatchRequest.objects.get(user=request.user, item=item, watch_method=0)
            wr.delete()
            return redirect('/item/' + str(item_id))
        elif request.POST['submit_type'] == 'borrow_watching':
            WatchRequest.objects.create(user=request.user, item=item, watch_method=1)
            return redirect('/item/' + str(item_id))
        elif request.POST['submit_type'] == 'borrow_unwatching':
            wr = WatchRequest.objects.get(user=request.user, item=item, watch_method=1)
            wr.delete()
            return redirect('/item/' + str(item_id))
        elif request.POST['submit_type'] == 'make_announcement':
            Announcement.objects.create(item=item, friend_state=request.POST['states'], msg=request.POST['msg'], date=datetime.now())
            return redirect('/item/' + str(item_id))
        else:
            BorrowRequest(user=request.user, item_id=item_id, request_date=datetime.now()).save()
            return redirect('/item/' + str(item_id))


class AllUsersView(View):
    def get(self, request):
        users = User.objects.filter(~Q(id=request.user.id))
        return render(request, 'all_users.html',{'users': users} )

    def post(self, request):
        if request.POST['submit_type'] == "add_friend":
            Friend.objects.create(sender_user=request.user, receiver_user_id=request.POST['user_id'])
            return redirect('/all_users/')
        elif request.POST['submit_type'] == 'search_emails':
            email_list = request.POST['search_text']
            email_list = list(email_list.split(" "))
            users = User.objects.filter(email__in=email_list)
            return render(request, 'all_users.html', {'users': users})


class NotificationsView(View):
    def get(self, request):
        notifications = Notification.objects.filter(receiver_user=request.user).order_by('-date')
        return render(request, 'notifications.html', {'notifications': notifications} )

    def post(self, request):

        return render(request, 'notifications.html', {})


class AnnouncementsView(View):
    def get(self, request):
        announcements = Announcement.objects.all().order_by('-date')

        filtered_announcements = []
        for announcement in announcements:
            item = announcement.item
            try:
                Friend.objects.get(((Q(sender_user=item.owner) & Q(receiver_user=request.user)) | (Q(sender_user=request.user) & Q(receiver_user=item.owner))) &
                                   (~Q(state=0) & Q(state__lte=announcement.friend_state)))
            except:
                continue
            filtered_announcements.append(announcement)

        return render(request, 'announcements.html', {'announcements': filtered_announcements})

    def post(self, request):
        return render(request, 'announcements.html', {})

