# Generated by Django 3.0 on 2019-12-21 17:08

import datetime
from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('social_network_app', '0013_auto_20191221_1426'),
    ]

    operations = [
        migrations.AddField(
            model_name='announcement',
            name='date',
            field=models.DateTimeField(default=datetime.datetime(2019, 12, 21, 17, 8, 3, 579154)),
        ),
    ]