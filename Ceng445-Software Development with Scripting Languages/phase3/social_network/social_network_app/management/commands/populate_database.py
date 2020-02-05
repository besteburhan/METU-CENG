import random
from random import randint
from django.utils import timezone
from django.core.management.base import BaseCommand
from social_network_app.models import *
import string

#

class Command(BaseCommand):
    help = 'populate database'

    def handle(self, *args, **options):
        # 10 user names

        names = ["Beste", "Beyza", "Edward", "Dimitri", "Janne",
                 "Evgeny", "Luis", "Olivia", "Amelia", "Valeria"]

        # 10 user surnames

        surnames = ["Burhan", "Butun", "Rose", "Smith", "Johnson",
                    "Grinko", "Williams", "Oliver", "Brown", "Black"]

        # 10 user email

        emails = ["besteburhan7@gmail.com", "beyzabtn3@gmail.com",
                  "edwardrose@gmail.com", "dimitrismith@gmail.com",
                  "jannejohnson@gmail.com", "engenygrinko@gmail.com",
                  "luiswilliams@gmail.com", "oliviaoliver@gmail.com",
                  "ameliabrown@gmail.com", "valeriablack@gmail.com"]



        # 20 item title

        title = ["To Kill a Mockingbird", "Harry Potter",
                 "1984", "Pride and Prejudice", "Diary of a young girl",
                 "Little prince", "Great gatspy", "Catcher in the rye",
                 "Animal Form", " The lord of the rings", "Jane Eyre",
                 "The Hunger Games", "Twilight", "The book Thief",
                 "Gone with the wind", "Bu sabahlarin bir anlami olmali",
                 "Sil bastan", "Cikita Muz", "Nane nane", "Ismail"]

        # 20 item artist

        artist = ["Harper Lee", "J.K. Rowling", "George Orwell",
                  "Jane Austen", "Anne Frank", "Antoine de Saint",
                  "F. Scott", "J.D. Salinger", "George Orwell",
                  "J.R.R. Tolkien", "Charlotte Bronte", "Suzanne Collins",
                  "Stephen Meyer", "Markus Zusak", "Margaret Mitchell",
                  "Vega", "Sebnem Ferah", "Ajdar", "Ajdar", "Grup Vitamin"]

        # 20 item year

        year = ["1996", "1997", "1998", "1999", "2000", "2001",
                "2002", "2003", "2004", "2005", "2006", "2007",
                "2008", "2009", "2010", "2011", "2012", "2013",
                "2014", "2015", "2016"]

        # 20 item genre

        genre = ["Action", "Anthology", "Classic", "Comic", "Crime",
                 "Drama", "Fairy Tale", "History", "Horror", "Romance",
                 "Sci-Fci", "Action", "Anthology", "Classic", "Comic",
                 "Crime", "Drama", "Fairy Tale", "History", "Horror"]

        loc = ["Shelf", "Table", "Library", "Home", "Room",
                 "Department", "Bag"]

        # Defining friendship state and item types

        STATE = ['NOFRIEND', 'CLOSEFRIEND', 'FRIEND']
        item_types = ["cd", "book", "dvd"]
        STATE_TYPE = ['CLOSED', 'CLOSEFRIENDS', 'FRIENDS', 'EVERYONE']

        user_objects = []
        item_objects = []

        # users

        for index in range(10):
            user = User(first_name=names[index], last_name=surnames[index], email=emails[index], username=emails[index],  is_active=1)
            user.set_password("123456")
            user.save()
            user_objects.append(user)


        # set_friendship

        for i in range(5):
            sender = user_objects[i]
            receiver = user_objects[i + 5]
            friend = Friend(sender_user=sender, receiver_user=receiver, state=randint(0, 2))
            friend.save()

        for index in range(20):
            item_objects.append(Item.objects.create(owner=user_objects[index % 10], type=item_types[randint(0, 2)],
                                     title=title[index], isbn=None, artist=artist[index], genre=genre[index], location=loc[index%7],
                                     year=year[index]))

        self.stdout.write(self.style.SUCCESS('Successfull'))