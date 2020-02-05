import string
from random import *

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
        "EVERYONE": 3
    }

    def __init__(self, database_obj, email, password):
        self.cur = database_obj.curs
        user = self.cur.execute('select * from Users u where u.email = \'{uemail}\''.format(uemail=email)).fetchone()
        self.id = user[0]
        self.name = user[1]
        self.surname = user[2]
        self.email = user[3]
        self.password = user[4]
        self.is_verified = user[5]
        self.verification_number = user[6]

    @classmethod
    def sign_up(cls, database_obj, params):
        characters = string.digits
        verification_number = '111111'
        database_obj.insert("Users",
                                     ('name', 'surname', 'email', 'password', 'is_verified', 'verification_number'),
                                     params[0], params[1], params[2], params[3], 0, verification_number)

        cur = database_obj.curs
        id = cur.execute('select u.id from Users u where u.email = \'{uemail}\''.format(uemail=params[2])).fetchone()[0]
        print(id, " ", params[0], ", user created but not verified yet. ")
        return (id, "is signed up with the verification number: '{ver_num}'".format(ver_num=verification_number))

    @classmethod
    def login(cls, database_obj, params):
        cur = database_obj.conn.cursor()

        query = cur.execute('select count(*), is_verified from Users where email = \'{uemail}\''.format(uemail=params[0])).fetchone()
        count = query[0]
        is_verified = query[1]

        if count == 0:
            return ('-', " Wrong email.")
        else:
            id = cur.execute('select u.id from Users u where u.email = \'{uemail}\''.format(uemail=params[0])).fetchone()[0]
            return ('+',id, " Logged in.", is_verified)

    def verify(self, database_obj, params):
        (email, verification_number) = params
        cur = self.cur
        ver_number = self.verification_number
        if verification_number == ver_number:
            database_obj.update("Users", "is_verified", "email", 1, email)
            return ('+', "{user} is verified.".format(
                user=cur.execute('select name from Users where email = \'{uemail}\''.format(uemail=email)).fetchone()[
                    0]))
        else:
            return ('-',"The verification number is wrong")

    def change_password(self, database_obj, params):
        new_password, old_password = params
        if old_password:
            if self.password == old_password:
                self.password = new_password
                database_obj.update("Users", "password", "email", new_password, self.email)
            else:
                return "Your old password is wrong."
        else:
            characters = string.ascii_letters + string.punctuation + string.digits
            generated_password = "".join(choice(characters) for x in range(randint(8, 16)))
            self.password = generated_password
            database_obj.update("Users", "password", "email", generated_password, self.email)
            return "{user} Your generated password is '{gen_pass}'.".format(user = self, gen_pass=generated_password)
        return "Your password is changed."

    def look_up(self, database_obj, email_list):
        email_list = tuple(email_list[0][1:-1].split(","))
        fetched_mails = self.cur.execute('select email from Users u where u.email in {elist}'.format(elist=email_list))
        fetched_mails = fetched_mails.fetchall()
        if not fetched_mails:
            return "There are no email from the email_list in the database"
        result = []
        for mail in fetched_mails:
            result.append(mail[0])
        database_obj.conn.commit()
        return result

    def friend(self, database_obj, params):
        email = params[0]
        receiver = self.cur.execute('select u.id from Users u where u.email like \'{uemail}\''.format(uemail=email))
        receiver_id = receiver.fetchone()[0]
        database_obj.insert("Friends", ('sender_user', 'receiver_user', 'state'), self.id, int(receiver_id),
                  self.STATE["NOFRIEND"])
        return "You sent a friendship request to '{id}'".format(id=receiver_id)

    def set_friend(self, database_obj, params):
        email, state = params
        user_id = self.cur.execute('select u.id from Users u where u.email = \'{uemail}\''.format(uemail=email)).fetchone()[0]

        self.cur.execute(
            'update Friends set state = {fstate} where (sender_user = {fuserid1} and receiver_user = {fuserid2}) or (sender_user = {fuserid2} and receiver_user = {fuserid1})'
                .format(fstate=self.STATE[state], fuserid1=self.id, fuserid2=user_id))
        database_obj.conn.commit()
        return "You set the friendship with '{user_id}' as '{state}'".format(user_id=user_id, state=state)

    def list_items(self, database_obj, email):
        user_id = self.cur.execute("select id from Users where email = \'{email}\'".format(email=email[0])).fetchone()[0]
        try:
            friend_state = self.cur.execute(
                "select state from Friends where (sender_user = {self_id} and receiver_user = {user_id}) or (sender_user = {user_id} and receiver_user = {self_id})"
                .format(self_id=self.id, user_id=user_id)).fetchone()[0]
        except:
            friend_state = 0
        if friend_state == 0:
            item_list = self.cur.execute("select i.id from Items i where i.owner = {user_id} and i.view = {state_type}"
                                         .format(user_id=user_id, state_type=3))
        else:
            item_list = self.cur.execute("select i.id from Items i where i.owner = {user_id} and i.view >= {state_type}"
                                         .format(user_id=user_id, state_type=friend_state))
        item_list = item_list.fetchall()
        result = []
        for item in item_list:
            result.append(item[0])
        if not result:
            return "There is no item that you can view.\n"
        return result


    def watch(self, database_obj, params):
        email, watch_method = params
        if watch_method == '':
            watch_method = None
        query = \
        self.cur.execute('select u.id, u.name from Users u where u.email = \'{uemail}\''.format(uemail=email)).fetchone()

        user_id = query[0]
        name = query[1]
        try:
            friend_state = self.cur.execute(
                "select state from Friends where (sender_user = {self_id} and receiver_user = {user_id}) or (sender_user = {user_id} and receiver_user = {self_id})"
                    .format(self_id=self.id, user_id=user_id)).fetchone()[0]
        except:
            friend_state = 0

        if watch_method is None:
            self.cur.execute("delete from WatchRequests where user_id = {user_id} and followed_id = {followed_id}"
                             .format(user_id=self.id, followed_id=user_id))
            return "{follower} stopped following {followed}".format(follower=self, followed=name)
        elif friend_state >= 1:
            database_obj.insert("WatchRequests", ("user_id", "watch_method", "followed_id"), self.id,
                      self.WATCH_REQUEST_TYPES["USER"], user_id)
            return "{follower} watching {followed}.".format(follower=self, followed=name)
        else:
            return "{follower} and {followed} are not friend. Watching denied".format(follower=self, followed=name)

    def list_borrowable_items(self, database_obj, params):
        f_string = f'SELECT owner, id, title ' + \
                   f'FROM Items WHERE ' + \
                   f'borrow!=0 and (borrow=3 or (owner in ' + \
                   f'(SELECT sender_user from Friends where receiver_user={"?"}  and Items.borrow>=state)  or ' + \
                   f'owner in (SELECT receiver_user from Friends where sender_user={"?"}  and Items.borrow>=state))) ' + \
                   f';'
        data = [self.id] * 2
        list_items = database_obj.curs.execute(f_string, data).fetchall()
        return list_items

    def __repr__(self):
        return self.name + " " + self.surname
