# Generated by Django 3.0 on 2019-12-21 14:26

from django.conf import settings
from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        migrations.swappable_dependency(settings.AUTH_USER_MODEL),
        ('social_network_app', '0012_auto_20191221_1219'),
    ]

    operations = [
        migrations.AlterField(
            model_name='watchrequest',
            name='followed_user',
            field=models.ForeignKey(null=True, on_delete=django.db.models.deletion.CASCADE, related_name='followed_user', to=settings.AUTH_USER_MODEL),
        ),
    ]