#!/usr/bin/python3.7

from socket import socket, AF_INET, SOCK_STREAM
from typing import List, Tuple
import create_tables
import user
import item
import database
import sqlite3
import pickle
import threading
from threading import Thread, Lock


class Server:
    request_port = 2145
    notification_port = 1251
    notification_waiting_clients = dict()
    notification_sock = socket(AF_INET, SOCK_STREAM)
    mutex = Lock()
    meta_data = {
        'user': {
            'sign_up': user.User.sign_up,
            'login': user.User.login,
            'verify': user.User.verify,
            'change_password': user.User.change_password,
            'friend' : user.User.friend,
            'set_friend' : user.User.set_friend,
            'lookup': user.User.look_up,
            'list_items': user.User.list_items,
            'watch': user.User.watch,
            'list_borrowable_items': user.User.list_borrowable_items,
        },
        'item': {
            'add_item': item.Item.add_item,
            'borrowed_req': item.Item.borrowed_req,
            'borrowed_by': item.Item.borrowed_by,
            'returned' : item.Item.returned,
            'make_comment' : item.Item.make_comment,
            'list_comments' : item.Item.list_comments,
            'rate' : item.Item.rate,
            'get_rating' : item.Item.get_rating,
            'locate' : item.Item.locate,
            'setstate' : item.Item.setstate,
            'search' : item.Item.search,
            'item_watch' : item.Item._watch,
            'view_info' : item.Item.view_info,
            'detailed_info' : item.Item.detailed_info,
            'announce' : item.Item.announce,
            'delete' : item.Item.delete
        }
    }

    user_required_functions = ('borrowed_req', 'rate', 'make_comment', 'search', 'item_watch', 'view_info', 'detailed_info', 'announce')

    @classmethod
    def send_notification(cls, notification_msg: str, user_ids):
        # if uid in cls.notification_waiting_clients:
        notification_msg = pickle.dumps(notification_msg)
        for user_id in user_ids:
            if user_id in cls.notification_waiting_clients:
                try:
                    cls.notification_waiting_clients[user_id].send(notification_msg)
                except:
                    continue

    @classmethod
    def notification_handler(cls, notification_sock, addr: Tuple):
        user_id = notification_sock.recv(128)
        user_id = pickle.loads(user_id)
        print(f'A Client is registered to be notified with user id : {user_id} with address: {addr}')
        cls.notification_waiting_clients[user_id] = notification_sock
        # cls.send_notification("hola", user_id)

    @classmethod
    def request_handler(cls, request_sock):
        db = sqlite3.connect('data.db')
        database_obj = database.DBConnection(db, db.cursor())
        while True:
            request = request_sock.recv(1024)

            if request == b'':
                print('empty req. aborting.')
                break

            request_type = pickle.loads(request)

            if request_type[0] not in cls.meta_data['user'] and request_type[0] not in cls.meta_data['item'] :
                print('no req')
                continue

            if request_type[0] in cls.meta_data['user']:
                func = cls.meta_data['user'][request_type[0]]
                if request_type[0] != 'login' and request_type[0] != 'sign_up':
                    cls.mutex.acquire()
                    msg = func(client, database_obj, request_type[1:])
                    cls.mutex.release()
                    msg = pickle.dumps(msg)
                else:
                    cls.mutex.acquire()
                    msg = func(database_obj, request_type[1:])
                    cls.mutex.release()
                    if msg[0] == '+':
                        client = user.User(database_obj, email=request_type[1], password=request_type[2])
                    msg = pickle.dumps(msg)
            else:
                func = cls.meta_data['item'][request_type[0]]
                if request_type[0] != 'add_item' and request_type[0] != 'search':
                    item_id = request_type[1]
                    cls.mutex.acquire()
                    _item = item.Item(database_obj, item_id)
                    cls.mutex.release()
                    if _item.id == -1:
                        msg = "The item does not exist anymore."
                        msg = pickle.dumps(msg)
                    elif request_type[0] in cls.user_required_functions:
                        cls.mutex.acquire()
                        msg = func(_item, database_obj, client, request_type[2:])
                        cls.mutex.release()
                        if request_type[0] == 'announce':
                            text, result = msg[0]
                            msg = msg[1]
                            cls.send_notification(text, result)
                        elif request_type[0] == 'make_comment':
                            text, result = msg[0]
                            msg = msg[1]
                            cls.send_notification(text, result)


                        msg = pickle.dumps(msg)
                    else:
                        cls.mutex.acquire()
                        msg = func(_item, database_obj, request_type[2:])
                        cls.mutex.release()
                        if request_type[0] == 'delete':
                            text, result = msg[0]
                            msg = msg[1]
                            cls.send_notification(text, result)
                        elif request_type[0] == 'setstate':
                            text, result = msg[0]
                            msg = msg[1]
                            cls.send_notification(text, result)

                        msg = pickle.dumps(msg)
                elif request_type[0] == 'add_item':   # add item
                    cls.mutex.acquire()
                    msg = func(database_obj, client, request_type[1:])
                    cls.mutex.release()
                    text, result = msg[0]
                    msg = msg[1]
                    cls.send_notification(text, result)
                    msg = pickle.dumps(msg)
                else:   #  search_item
                    cls.mutex.acquire()
                    msg = func(database_obj, client, request_type[1:])
                    cls.mutex.release()
                    msg = pickle.dumps(msg)



            request_sock.send(msg)

        db.close()

    @classmethod
    def start_server(cls):
        try:
            request_handler_sock = socket(AF_INET, SOCK_STREAM)
            request_handler_sock.bind(('', cls.request_port))
            request_handler_sock.listen()
        except Exception as e:
            print(f'request->{e !r}')
            return
        while True:

            conn, peer = request_handler_sock.accept()
            req_handler = Thread(target=cls.request_handler, args=(conn,))
            req_handler.start()
            print("New client -> thread : ", req_handler.ident)

    @classmethod
    def start_notification(cls):
        try:
            notification_handler_sock = socket(AF_INET, SOCK_STREAM)
            notification_handler_sock.bind(('', cls.notification_port))
            notification_handler_sock.listen()
        except Exception as e:
            print(f'notify->{e !r}')
            return

        while True:
            conn, peer = notification_handler_sock.accept()
            not_handler = Thread(target=cls.notification_handler, args=(conn, peer))
            not_handler.start()
            print("Notification thread -> ", not_handler.ident)


    @classmethod
    def start(cls):

        start_server = Thread(target=cls.start_server)
        start_server.start()
        start_notification = Thread(target=cls.start_notification)
        start_notification.start()
        #
        # start_server.join()
        # start_notification.join()

if __name__ == "__main__":
    server = Server()
    pool = server.start()

    # pool.shutdown(wait=True)
