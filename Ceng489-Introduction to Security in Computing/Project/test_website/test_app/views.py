from django.shortcuts import render
from django.views.generic import View
from django.contrib.auth.models import User
from django.db import connection
# Create your views here.


class Main(View):
    def get(self, request):
        return render(request, 'main.html', {"users":[]})

    def post(self, request):
        if 'first_name' in request.POST:
            cursor = connection.cursor()
            print(cursor.execute("SELECT * FROM auth_user WHERE first_name LIKE '' or 1=1;"))
            query = "SELECT * FROM auth_user WHERE first_name LIKE '{}%';".format(request.POST['first_name'])
            print(query)
            users = cursor.executescript(query).fetchall()
            print("users", users)
        if 'email' in request.POST:
            users = User.objects.filter(email__contains=request.POST['email'])
            print("users", users)
        return render(request, "main.html", {"users": users})
