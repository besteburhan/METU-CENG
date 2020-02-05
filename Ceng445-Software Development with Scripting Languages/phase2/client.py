#!/usr/bin/python3.7

from socket import socket, AF_INET, SOCK_STREAM
from threading import Thread
import pickle
from datetime import datetime


class Client:
    WATCH_REQUEST_TYPES = {
        0: "COMMENT",
        1: "BORROW",
        2: "USER"
    }
    STATE = {
        0: "NOFRIEND",
        1: "CLOSEFRIEND",
        2: "FRIEND"
    }
    STATE_TYPE = {
        0: "CLOSED",
        1: "CLOSEFRIEND",
        2: "FRIEND",
        3: "EVERYONE"
    }
    server_addr = '127.0.0.1'

    request_port = 2145
    notification_port = 1251
    meta_data = None
    is_login = False

    @classmethod
    def all_acts(cls):
        print('You can select the request that you want to make:  ')
        for act in cls.meta_data.keys():
            print(act, end='  |  ')

    @classmethod
    def request_handler(cls, request_sock, request_type):
        if request_type == 'verify':
            email = input("Email: ")
            verification_number = input("Verification number: ")
            params = (request_type, email, verification_number)
            params = pickle.dumps(params)
            request_sock.send(params)
            msg = request_sock.recv(1024)
            msg = pickle.loads(msg)
            if msg[0] == '+':
                Client.meta_data = {
                    'change password' : cls.request_handler,
                    'friend' : cls.request_handler,
                    'set friend': cls.request_handler,
                    'lookup' : cls.request_handler,
                    'list_items' : cls.request_handler,
                    'watch': cls.request_handler,
                    'add_item' : cls.request_handler,
                    'borrowed_req': cls.request_handler,
                    'borrowed_by' : cls.request_handler,
                    'list_borrowable_items': cls.request_handler,
                    'returned': cls.request_handler,
                    'make_comment' : cls.request_handler,
                    'list_comments': cls.request_handler,
                    'rate' : cls.request_handler,
                    'get_rating' : cls.request_handler,
                    'locate': cls.request_handler,
                    'setstate': cls.request_handler,
                    'search': cls.request_handler,
                    'item_watch': cls.request_handler, #?
                    'view_info': cls.request_handler,
                    'detailed_info': cls.request_handler,
                    'announce': cls.request_handler,
                    'delete': cls.request_handler

                }
            print(msg[1])
        elif request_type == 'change password':
            old_password = input("Old Password (If you don't know your old password, skip it: ")
            new_password = input("New Password: ")
            if old_password == '':
                old_password = None
            params = ('change_password', new_password, old_password)
        elif request_type == "lookup":
            email_list = input("Email List: ")
            params = (request_type, email_list)
        elif request_type == 'list_items':
            email = input("which user's items would you like to see, Email:")
            params = (request_type, email)
        elif request_type == 'friend':
            email = input("User's email who you want to send friendship request: ")
            params = ('friend', email)
        elif request_type == 'set friend':
            email = input("User's email whose you want to set friendship state: ")
            state = input("NO FRIEND: 0, CLOSEFRIEND:1, FRIEND: 2  :")
            params = ('set_friend', email, cls.STATE[int(state)])
        elif request_type == 'watch':
            email = input("User's email whose you want to watch: ")
            state = input("USER: 2 or skip it for remove watch request :")
            params = ('watch', email, cls.WATCH_REQUEST_TYPES[int(state)])
        elif request_type == 'borrowed_req':
            item_id = input("Item ID that you want to borrow: ")
            params = ('borrowed_req', item_id)
        elif request_type == 'borrowed_by':
            item_id = input("Item ID that you want to lend: ")
            email = input("User's email that you want to accept borrow request: ")
            return_date = input("Return date: ")
            params = ('borrowed_by', item_id, email, return_date)
        elif request_type == 'returned':
            item_id = input("Item ID that you want to return: ")
            location = input("Location: ")
            params = ('returned', item_id, location)
        elif request_type == 'add_item':
            item_type = input("Item Type: ")
            title = input("Title: ")
            uniqid = input("Isbn Number: ")
            artist = input("Artist: ")
            genre = input("Genre: ")
            year = input("Year: ")
            params = ('add_item', item_type, title, uniqid, artist, genre, year)
        elif request_type == 'list_borrowable_items':
            params = ('list_borrowable_items',)
        elif request_type == 'list_comments':
            item_id = input("Item ID about what you want to list comments: ")
            params = ('list_comments', item_id)
        elif request_type == 'rate':
            item_id = input("Item ID that you want to rate: ")
            rate = input("Rate: ")
            params = ('rate', item_id, rate)
        elif request_type == 'get_rating':
            item_id = input("Item ID that you want to get total rating: ")
            params = ('get_rating', item_id)
        elif request_type == 'make_comment':
            item_id = input("Item ID that you want to make comment: ")
            comment = input("Comment: ")
            params = ('make_comment', item_id, comment)
        elif request_type == 'locate':
            item_id = input("Item ID of which you want to change location: ")
            location = input("Location: ")
            params = ('locate', item_id, location)
        elif request_type == 'view_info':
            item_id = input("Item ID about which you want to view information: ")
            params = ('view_info', item_id)
        elif request_type == 'detailed_info':
            item_id = input("Item ID about which you want to view detailed information: ")
            params = ('detailed_info', item_id)
        elif request_type == 'delete':
            item_id = input("Item ID that will be deleted: ")
            params = ('delete', item_id)
        elif request_type == 'setstate':
            item_id = input("Item ID of which you want to set state: ")
            state_type = input("Choose state type : view|detail|borrow|comment|search : ")
            state = input("Choose state: CLOSED:0 | CLOSEFRIEND:1 | FRIEND:2 | EVERYONE:3 : ")
            params = ('setstate', item_id, state_type, cls.STATE_TYPE[int(state)])
        elif request_type == 'search':
            search_text = input("Search: ")
            genre = input("Genre: ")
            year = input("Year: ")
            for_borrow = input("For borrow:1 otherwise skip: ")
            params = ('search', search_text, genre, year, for_borrow)
            print("(owner, item_id)")
        elif request_type == 'item_watch':
            item_id = input("Item ID of which you want to set state: ")
            state = input("Follow -> COMMENT: 0, BORROW:1  ")
            params = ('item_watch', item_id, cls.WATCH_REQUEST_TYPES[int(state)])
        elif request_type=='announce':
            item_id = input("Item ID that you want to announce about: ")
            owner_type = input("Who do you want to announce: CLOSEFRIEND: 1 | FRIEND: 2  : ")
            msg = input("Message: ")
            params=('announce', item_id, owner_type, msg)


        if request_type != 'verify':
            params = pickle.dumps(params)
            request_sock.send(params)
            msg = request_sock.recv(1024)
            msg = pickle.loads(msg)
            if msg:
                print(msg)

    @classmethod
    def notification(cls, user_id):

        sock = socket(AF_INET, SOCK_STREAM)
        sock.connect((cls.server_addr, cls.notification_port))
        str_user_id = user_id
        user_id = pickle.dumps(user_id)
        sock.send(user_id)
        while True:
            notification_msg = sock.recv(1024)
            if notification_msg == b'':
                print('Server closed')

            if notification_msg == b'CLS':
                break
            notification_msg = pickle.loads(notification_msg)
            print(f'\n\n*****\n notification received by {str_user_id}: {notification_msg} \n*****')

    @classmethod
    def login(cls, request_sock, request_type):
        if not Client.is_login:
            email = input('Email: ')
            passw = input('Password: ')

            params = ('login', email, passw)
            params = pickle.dumps(params)
            request_sock.send(params)

            msg = request_sock.recv(1024)
            msg = pickle.loads(msg)

            if msg[0] != '-':
                notification_thread = Thread(target=cls.notification, args=(msg[1],))  # msh[0] is user_id
                notification_thread.start()
                cls.is_login = True
                if msg[-1] == 0:
                    cls.meta_data = {
                        'verify': cls.request_handler,
                    }
                else:
                    cls.meta_data = {
                        'change password' : cls.request_handler,
                        'friend' : cls.request_handler,
                        'set friend': cls.request_handler,
                        'lookup' : cls.request_handler,
                        'list_items' : cls.request_handler,
                        'watch': cls.request_handler,
                        'add_item': cls.request_handler,
                        'borrowed_req': cls.request_handler,
                        'borrowed_by' : cls.request_handler,
                        'returned': cls.request_handler,
                        'list_borrowable_items': cls.request_handler,
                        'make_comment': cls.request_handler,
                        'list_comments': cls.request_handler,
                        'rate' : cls.request_handler,
                        'get_rating': cls.request_handler,
                        'locate': cls.request_handler,
                        'setstate': cls.request_handler,
                        'search': cls.request_handler,
                        'item_watch': cls.request_handler, #?
                        'view_info': cls.request_handler,
                        'detailed_info': cls.request_handler,
                        'announce': cls.request_handler,
                        'delete': cls.request_handler
                    }

    @classmethod
    def sign_up(cls, request_sock, request_type):
        name = input('Name: ')
        surname = input('Surname: ')
        email = input('Email: ')
        password = input('Password: ')

        params = ('sign_up', name, surname, email, password)
        params = pickle.dumps(params)
        request_sock.send(params)
        msg = request_sock.recv(1024)
        msg = pickle.loads(msg)
        print('\n', msg)


    @classmethod
    def connect(cls):
        request_socket = socket(AF_INET, SOCK_STREAM)
        request_socket.connect((cls.server_addr, cls.request_port))
        Client.meta_data = {
            'login': cls.login,
            'sign_up': cls.sign_up,
        }

        while True:
            # cls.all_acts()
            request_type = input('\nRequest: ')
            print("Sec:Milisec -> ",datetime.now().strftime("%S:%f"), request_type)
            if request_type in cls.meta_data:
                cls.meta_data[request_type](request_socket, request_type)


if __name__ == "__main__":
    client = Client()
    client.connect()
