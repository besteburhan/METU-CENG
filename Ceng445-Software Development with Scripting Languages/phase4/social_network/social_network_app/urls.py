from django.urls import path
from social_network_app import views
from django.conf.urls import url

app_name = 'social_network_app'
urlpatterns = [
    path('register/', views.RegisterView.as_view(), name='register'),
    path('login/', views.LoginView.as_view(), name='login'),
    path('verification/', views.VerificationView.as_view(), name='verification'),
    path('logout/', views.LogoutView.as_view(), name='logout'),
    path('friendships/', views.FriendsView.as_view(), name='friendships'),
    path('users/<int:user_id>/', views.UsersView.as_view(), name='users'),
    path('borrow_requests/', views.BorrowRequestsView.as_view(), name='borrow_requests'),
    path('all_items/', views.AllItemsView.as_view(), name='all_items'),
    path('all_users/', views.AllUsersView.as_view(), name='all_users'),
    path('notifications/', views.NotificationsView.as_view(), name='notifications'),
    path('announcements/', views.AnnouncementsView.as_view(), name='announcements'),
    path('item/<int:item_id>/', views.ItemView.as_view(), name='item'),

    #     for AJAX calls
    url(r'^see_announcement/$', views.see_announcement, name='see-announcement'),
    url(r'^see_notification/$', views.see_notification, name='see-notification'),
]
