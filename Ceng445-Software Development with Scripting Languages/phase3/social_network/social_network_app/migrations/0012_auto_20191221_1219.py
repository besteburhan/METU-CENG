# Generated by Django 3.0 on 2019-12-21 12:19

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('social_network_app', '0011_auto_20191221_1206'),
    ]

    operations = [
        migrations.AlterField(
            model_name='notification',
            name='item',
            field=models.ForeignKey(null=True, on_delete=django.db.models.deletion.CASCADE, to='social_network_app.Item'),
        ),
    ]
