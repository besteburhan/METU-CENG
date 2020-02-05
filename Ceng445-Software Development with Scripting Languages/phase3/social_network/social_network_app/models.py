from django.db import models
from django.contrib.auth.models import User

WATCH_REQUEST_TYPES = {
    0: "COMMENT",
    1: "BORROW",
    2: "USER",

}
STATE = {
    0: "Pending",
    1: "Close Friend",
    2: "Friend",
}
STATE_TYPE = {
    0: "CLOSED",
    1: "CLOSEFRIEND",
    2: "FRIEND",
    3: "EVERYONE",
}


class Item(models.Model):
    owner = models.ForeignKey(User, on_delete=models.CASCADE)
    type = models.CharField(max_length=150)
    title = models.CharField(max_length=150)
    isbn = models.IntegerField(null=True, blank=True, unique=True)
    artist = models.CharField(max_length=256)
    genre = models.CharField(max_length=150)
    year = models.IntegerField()
    location = models.CharField(max_length=256)
    view = models.IntegerField(default=2)
    detail = models.IntegerField(default=2)
    borrow = models.IntegerField(default=2)
    comment = models.IntegerField(default=2)
    search = models.IntegerField(default=2)

    def __str__(self):
        return "owner : " + str(self.owner) + " title : " + str(self.id)  +self.title


class Friend(models.Model):
    sender_user = models.ForeignKey(User, on_delete=models.CASCADE, related_name='%(class)s_sender_user')
    receiver_user = models.ForeignKey(User, on_delete=models.CASCADE, related_name='%(class)s_receiver_user')
    state = models.IntegerField(default=0)

    class Meta:
        unique_together = ('sender_user', 'receiver_user',)


class Comment(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE, related_name='%(class)s_user')
    item = models.ForeignKey(Item, on_delete=models.CASCADE, related_name='%(class)s_item')
    text = models.TextField()
    date = models.DateTimeField()


class Announcement(models.Model):
    item = models.ForeignKey(Item, on_delete=models.CASCADE)
    friend_state = models.IntegerField()
    msg = models.TextField()
    date = models.DateTimeField(auto_now_add=True)


class WatchRequest(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE, related_name='user')
    item = models.ForeignKey(Item, on_delete=models.CASCADE, null=True)
    watch_method = models.IntegerField()
    followed_user = models.ForeignKey(User, on_delete=models.CASCADE, related_name='followed_user', null=True)


class Borrow(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE, related_name='%(class)s_user')
    item = models.ForeignKey(Item, on_delete=models.CASCADE, related_name='%(class)s_item')
    taken_date = models.DateTimeField()
    returned_date = models.DateTimeField()
    rate = models.IntegerField(null=True)
    is_returned = models.BooleanField(default=0)


class BorrowRequest(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    item = models.ForeignKey(Item, on_delete=models.CASCADE)
    request_date = models.DateTimeField()


class Notification(models.Model):
    sender_user = models.ForeignKey(User, on_delete=models.CASCADE, related_name='sender_user')
    receiver_user = models.ForeignKey(User, on_delete=models.CASCADE, related_name='receiver_user')
    item = models.ForeignKey(Item, on_delete=models.DO_NOTHING, null=True)
    text = models.TextField()
    date = models.DateTimeField()

