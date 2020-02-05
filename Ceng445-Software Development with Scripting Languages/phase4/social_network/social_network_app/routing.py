from django.conf.urls import url
from . import consumers

websocket_urlpatterns = [
    url(r'^ws/announcement/$', consumers.AnnouncementConsumer),
    url(r'^ws/notification/$', consumers.NotificationConsumer),
]