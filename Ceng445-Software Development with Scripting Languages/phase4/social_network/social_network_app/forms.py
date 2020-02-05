from django.contrib.auth.models import User
from social_network_app.models import *
from  django import forms


class UserForm(forms.ModelForm):
    first_name = forms.CharField(widget=forms.TextInput(attrs={'placeholder': 'First Name'}), label='', required=True)
    last_name = forms.CharField(widget=forms.TextInput(attrs={'placeholder': 'Last Name'}), label='', required=True)
    email = forms.EmailField(widget=forms.TextInput(attrs={'placeholder': 'Email Address'}), label='', required=True)
    password = forms.CharField(widget=forms.PasswordInput(attrs={'placeholder': 'Password'}), label='', required=True)

    class Meta:
        model = User
        fields = ('first_name', 'last_name', 'email', 'password')


class LoginForm(forms.Form):
    email = forms.EmailField(widget=forms.TextInput(attrs={'placeholder': 'Email Address'}), label='', max_length=254, required=True)
    password = forms.CharField(widget=forms.PasswordInput(attrs={'placeholder': 'Password'}),  label='', max_length=128, required=True)


class VerificationForm(forms.Form):
    email = forms.EmailField(widget=forms.TextInput(attrs={'placeholder': 'Email Address'}), label='', max_length=254, required=True)
    password = forms.CharField(widget=forms.PasswordInput(attrs={'placeholder': 'Password'}),  label='', max_length=128, required=True)
    verification = forms.CharField(widget=forms.TextInput(attrs={'placeholder': 'Verification Number'}),  label='', max_length=128, required=True)



