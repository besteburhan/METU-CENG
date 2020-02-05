from database import db
import string
from random import *
from functools import wraps


def verification_required(func):
    @wraps(func)
    def wrapper(self, *args, **kwargs):
        is_verified = db.connection.cursor().execute('select is_verified from Users where id=?', (self.id, )).fetchone()[0]
        if is_verified:
            return func(self, *args, **kwargs)
        else:
            print('You are not verified!')
            return 'You are not verified!'
    return wrapper


class User:

    WATCH_REQUEST_TYPES = {
        "COMMENT": 0,
        "BORROW": 1,
        "USER": 2
    }
    STATE = {
        "NOFRIEND": 0,
        "CLOSEFRIEND": 1,
        "FRIEND": 2
    }
    STATE_TYPE = {
        "CLOSED": 0,
        "CLOSEFRIEND": 1,
        "FRIEND": 2,
        "EVERYONE" : 3
    }

    def __init__(self, name, surname, email, password):

        self.cur = db.get_cursor()
        self.name = name
        self.surname = surname
        self.email = email
        self.password = password
        characters = string.digits
        verification_number = "".join(choice(characters) for c in range(6))

        db.insert("Users", ('name', 'surname', 'email', 'password', 'is_verified', 'verification_number'), name, surname, email, password, 0, verification_number)
        user = self.cur.execute('select u.id from Users u where u.email = \'{uemail}\''.format(uemail=self.email))
        self.id = user.fetchone()[0]
        print(self.id,  " ", self, ", user created but not verified yet. ")

    @staticmethod
    def verify(email, verification_number):

        cur = db.get_cursor()
        ver_number = cur.execute('select verification_number from Users where email = \'{uemail}\''.format(uemail=email)).fetchone()[0]
        if verification_number == ver_number:
            db.update("Users", "is_verified", "email", 1, email)
            print("{user} is verified.".format(user=cur.execute('select name from Users where email = \'{uemail}\''.format(uemail=email)).fetchone()[0]))
        else:
            print("The verification number is wrong")

    @verification_required
    def change_password(self, new_password, old_password=None):

        if old_password:
            if self.password == old_password:
                self.password = new_password
                db.update("Users", "password", "email", new_password, self.email)
            else:
                print("Please remember your old password correctly, otherwise I don't give any shit what you want to do!.")
        else:
            characters = string.ascii_letters + string.punctuation + string.digits
            generated_password = "".join(choice(characters) for x in range(randint(8, 16)))
            self.password = generated_password
            db.update("Users", "password", "email", generated_password, self.email)

    @verification_required
    def look_up(self, email_list):

        fetched_mails = self.cur.execute('select email from Users u where u.email in {elist}'.format(elist = email_list))
        fetched_mails = fetched_mails.fetchall()
        if not fetched_mails:
            return "There are no email from the email_list in the database"
        result = []
        for mail in fetched_mails:
            result.append(mail[0])
        db.connection.commit()
        return result

    @verification_required
    def friend(self, email):

        receiver = self.cur.execute('select u.id from Users u where u.email like \'{uemail}\''.format(uemail=email))
        receiver_id = receiver.fetchone()
        db.insert("Friends", ('sender_user', 'receiver_user', 'state'), self.id, int(receiver_id[0]), self.STATE["NOFRIEND"])

    @verification_required
    def set_friend(self, user, state):

        self.cur.execute(
            'update Friends set state = {fstate} where (sender_user = {fuserid1} and receiver_user = {fuserid2}) or (sender_user = {fuserid2} and receiver_user = {fuserid1})'
            .format(fstate=self.STATE[state], fuserid1=self.id, fuserid2=user.id))
        db.connection.commit()

    @verification_required
    def list_items(self, user):

        try:
            friend_state = self.cur.execute("select state from Friends where (sender_user = {self_id} and receiver_user = {user_id}) or (sender_user = {user_id} and receiver_user = {self_id})"
                                            .format(self_id=self.id, user_id=user.id)).fetchone()[0]
        except:
            friend_state = 0
        if friend_state == 0:
            item_list = self.cur.execute("select i.id from Items i where i.owner = {user_id} and i.view = {state_type}"
                                        .format(user_id=user.id, state_type=friend_state))
        else:
            item_list = self.cur.execute("select i.id from Items i where i.owner = {user_id} and i.view >= {state_type}"
                                         .format(user_id=user.id, state_type=friend_state))
        item_list = item_list.fetchall()
        result = []
        for item in item_list:
            result.append(item[0])
        return result

    @verification_required
    def watch(self, user, watch_method):

        friend_state = self.cur.execute(
            "select state from Friends where (sender_user = {self_id} and receiver_user = {user_id}) or (sender_user = {user_id} and receiver_user = {self_id})"
            .format(self_id=self.id, user_id=user.id)).fetchone()[0]

        if watch_method is None:
            self.cur.execute("delete from WatchRequests where user_id = {user_id} and followed_id = {followed_id}"
                             .format(user_id=self.id, followed_id=user.id))
            print("{follower} stopped following {followed}".format(follower=self, followed=user))
        elif friend_state >= 1:
            db.insert("WatchRequests", ("user_id", "watch_method", "followed_id"), self.id, self.WATCH_REQUEST_TYPES["USER"], user.id)
            print("{follower} watching {followed}.".format(follower=self, followed=user))
        else:
            print("{follower} and {followed} are not friend. Watching denied".format(follower=self, followed=user))

    def __repr__(self):
        return self.name + " " + self.surname

