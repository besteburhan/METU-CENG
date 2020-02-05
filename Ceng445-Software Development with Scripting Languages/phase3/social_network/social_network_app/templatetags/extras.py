from social_network_app.models import *
from django import template
from django.db.models import Q, Avg

register = template.Library()


@register.simple_tag
def get_state_type(item, attr):
    return STATE_TYPE[item.__getattribute__(attr)]


@register.simple_tag
def get_state(state_index):
    return STATE[state_index]


@register.simple_tag
def check_detail_permission(user, item):
    try:
        friend_state = Friend.objects.get((Q(receiver_user=user) & Q(sender_user=item.owner)) | (Q(sender_user=user) & Q(receiver_user=item.owner))).state
    except:
        friend_state = 0
    if (item.detail >= friend_state and friend_state != 0) or item.detail == 3:
        return True
    return False


@register.simple_tag
def check_view_permission(user, item):
    if item.owner == user:
        return True
    try:
        friend_state = Friend.objects.get((Q(receiver_user=user) & Q(sender_user=item.owner)) | (Q(sender_user=user) & Q(receiver_user=item.owner))).state
    except:
        friend_state = 0
    if (item.view >= friend_state and friend_state != 0) or item.view == 3:
        return True
    return False

@register.simple_tag
def check_borrow_permission(user, item):
    if user == item.owner:
        return False
    try:
        borrow_requests = Borrow.objects.get(item=item, is_returned=0)
        return False
    except:

        try:
            friend_state = Friend.objects.get((Q(receiver_user=user) & Q(sender_user=item.owner)) | (Q(sender_user=user) & Q(receiver_user=item.owner))).state
        except:
            friend_state = 0
        if (item.borrow >= friend_state and friend_state != 0) or item.borrow == 3:
            try:
                BorrowRequest.objects.get(user=user, item=item)
                return False
            except:
                return True
        return False

@register.simple_tag
def check_comment_permission(user, item):
    if user == item.owner:
        return True
    try:
        friend_state = Friend.objects.get((Q(receiver_user=user) & Q(sender_user=item.owner)) | (Q(sender_user=user) & Q(receiver_user=item.owner))).state
    except:
        friend_state = 0
    if (item.comment >= friend_state and friend_state != 0) or item.comment == 3:
        return True
    return False


@register.simple_tag
def get_location_item(user, item):
    try:
        borrowed = Borrow.objects.get(is_returned=0, item=item)
        print("extrasss", borrowed)
        return "borrowed"  # borrowed
    except:
        if user == item.owner:
            return item.location
        try:
            friend_state = Friend.objects.get((Q(receiver_user=user) & Q(sender_user=item.owner)) | (Q(sender_user=user) & Q(receiver_user=item.owner))).state
        except:
            friend_state = 0
        if (item.detail >= friend_state and friend_state != 0) or item.detail == 3:
            return item.location  # has detail permission
        return "no permission"

@register.simple_tag
def get_order_in_borrow_requests(user, item):
    try:
        borrow_request = BorrowRequest.objects.get(user=user, item=item)
    except:
        return False
    count = BorrowRequest.objects.filter(Q(item=item) & Q(request_date__lt=borrow_request.request_date)).count()
    return count+1


@register.simple_tag
def check_user_watch_request(user1, user2):
    try:
       friend = Friend.objects.get((Q(sender_user=user1) & Q(receiver_user=user2) & Q(state__gte=1)) | (Q(sender_user=user2) & Q(receiver_user=user1) & Q(state__gte=1)))
       return True
    except:
        return False


@register.simple_tag
def is_watching_user(user, followed_user):
    try:
       wr = WatchRequest.objects.get(Q(user=user) & Q(followed_user=followed_user) )
       return True
    except:
        return False


@register.simple_tag
def is_watching_comment(user, item):
    try:
       wr = WatchRequest.objects.get(Q(user=user) & Q(item=item) & Q(watch_method=0) )
       return True
    except:
        return False


@register.simple_tag
def is_watching_borrow(user, item):
    try:
       wr = WatchRequest.objects.get(Q(user=user) & Q(item=item) & Q(watch_method=1))
       return True
    except:
        return False


@register.simple_tag
def get_borrow_requests(item):
    try:
        borrow_requests = BorrowRequest.objects.filter(item=item).order_by('request_date')
    except:
        return {}
    return borrow_requests


@register.simple_tag
def is_rateable(user, item, borrow_id):
    try:
        borrows = Borrow.objects.filter(user=user, item=item, is_returned=1)
        return True
    except:
        return False


@register.simple_tag
def get_rating( item ):
    try:
        avg_rating = Borrow.objects.filter(Q(item=item) & Q(is_returned=1) & ~Q(rate=None)).aggregate(Avg('rate'))
        print(avg_rating)
        return avg_rating['rate__avg']
    except:
        return 0


@register.simple_tag
def check_friend_request_availability( user_id1, user_id2 ):
    try:
        is_available = Friend.objects.get((Q(sender_user_id=user_id1) & Q(receiver_user_id=user_id2)) | (Q(receiver_user=user_id1) & Q(sender_user=user_id2)))
        return False
    except:
        return True




