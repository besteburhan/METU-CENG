from isbnlib import meta
from datetime import datetime, timedelta


class Item:
    WATCH_REQUEST_TYPES = {
        "COMMENT": 0,
        "BORROW": 1,  # watch for status change
        "USER": 2
    }

    STATE = {
        'NOFRIEND': 0,
        'CLOSEFRIEND': 1,
        'FRIEND': 2
    }
    STATE_TYPE = {
        'CLOSED': 0,
        'CLOSEFRIENDS': 1,
        'FRIENDS': 2,
        'EVERYONE': 3,
    }

    def __init__(self, database_obj, item_id):
        self.cur = database_obj.curs
        item_query = self.cur.execute("select * from Items where id = {id}".format(id=item_id)).fetchone()
        try:
            self.id = item_query[0]
            self.owner = item_query[1]
            self.type = item_query[2]
            self.title = item_query[3]
            self.uniqid = item_query[4]
            self.artist = item_query[5]
            self.genre = item_query[6]
            self.year = item_query[7]
            self.location = item_query[8]
            self.view = item_query[9]
            self.detail = item_query[10]
            self.borrow = item_query[11]
            self.comment = item_query[12]
            self.search = item_query[13]
        except:
            self.id = -1

    @classmethod
    def add_item(cls, database_obj, owner, params):
        item_type, title, uniqid, artist, genre, year = params
        is_verified = False
        try:
            is_verified = database_obj.curs.execute('select is_verified from Users where id=?', (owner.id,)).fetchone()[
                0]
        except:
            return (('',[]),"There is no such user.")
        if not is_verified:
            return (('',[]),"Owner is not verified, item can't be created!")
        else:
            user_id = database_obj.curs.execute("select id from Users where email like \'{m}\'".format(m=owner.email))
            user_id = user_id.fetchone()[0]
            item_type = item_type
            genre = genre
            location = None
            view = 2
            borrow = 2
            detail = 2
            comment = 2
            search = 2
            if uniqid is '':
                uniqid = None
                title = title
                artist = artist
                year = year
                database_obj.insert("Items", ('owner', 'type', 'title', 'uniqueid', 'artist', 'genre', 'year', 'view',
                                              'detail', 'search', 'borrow', 'comment'), user_id, item_type, title,
                                    uniqid, artist, genre, year, 2, 2, 2, 2, 2)
                id = database_obj.curs.execute("select last_insert_rowid()").fetchone()[0]
            else:
                metadata = None
                try:
                    metadata = meta(isbn=uniqid)
                except Exception as ex:
                    print(ex)
                if metadata is not None:
                    title = metadata['Title']
                    year = metadata['Year']
                    artist = metadata['Authors'][0]
                    database_obj.insert("Items",
                                        ('owner', 'type', 'title', 'uniqueid', 'artist', 'genre', 'year', 'view',
                                         'detail', 'search', 'borrow', 'comment'), user_id,
                                        item_type, title, uniqid, artist, genre, year, 2, 2, 2, 2, 2)
                    id = database_obj.curs.execute("select last_insert_rowid()").fetchone()[0]

            users_res = []
            try:
                users_watching = database_obj.curs.execute(
                    "select user_id from WatchRequests where followed_id={owner} and watch_method={wmethod}".format(
                        owner=owner.id, wmethod=cls.WATCH_REQUEST_TYPES["USER"])).fetchall()
                for user in users_watching:
                    database_obj.insert("Notifications",
                                        ("sender", "receiver", "item_id", "notification_text", "notification_date"),
                                        owner.id, user[0], id,
                                        "{owner} add new item : \'{item}\' .".format(owner=owner.id, item=title),
                                        datetime.now())
                    users_res.append(user[0])
            except:
                pass
            return (("{owner} add new item :\'{item}\' .".format(owner=owner.id, item=title), users_res),
                    "Item {id} is added successfuly.".format(id=id))

    def borrowed_req(self, database_obj, user, params):
        already_requested = None
        try:
            already_requested = self.cur.execute("select user_id from BorrowRequests where item_id={item} and user_id={user}".format(item=self.id, user=user.id)).fetchone()[0]
        except:
            database_obj.insert("BorrowRequests", ('user_id', 'item_id', 'request_date'), user.id, self.id, datetime.now())
        queue = self.cur.execute("select count(*) from BorrowRequests where item_id={self_id}"
                                 .format(self_id=self.id)).fetchone()[0]
        if already_requested is not None:  # true
            return "{user} already requested for this item, and your place in borrow request list: '{queue}'".format(
                user=user, queue=queue)
        return "'{user}' Your place in borrow request list: '{queue}'".format(user=user, queue=queue)

    def borrowed_by(self, database_obj, params):
        email, return_date = params
        if return_date == '':
            return_date = 2
        taking_date = datetime.now()
        return_date = taking_date + timedelta(weeks=return_date)
        query = self.cur.execute("select id, name from Users where email = \'{email}\'".format(email=email)).fetchone()
        user_id = query[0]
        name = query[1]

        already_borrowed = None
        try:
            already_borrowed = self.cur.execute(f"select id from Borrows where is_returned=0 and item_id={'?'}", [self.id]).fetchone()[0]

        except:
            database_obj.insert("Borrows", ('user_id', 'item_id', 'taking_date', 'return_date', 'is_returned'), user_id,
                      self.id, datetime.now(), return_date, 0)
            self.cur.execute('delete from BorrowRequests where user_id={user} and item_id={item} ;'
                             .format(user=user_id, item=self.id ))
            database_obj.conn.commit()
        if already_borrowed is not None:
            return "This item is already borrowed."
        return "'{name}' borrowed the item: '{id}'".format(name=name, id=self.id)

    def returned(self, database_obj, params):
        location = params
        if location=='':
            location = None
        self.cur.execute("update Borrows set is_returned=1 where is_returned=0 and item_id={id};".format(id=self.id))
        self.location = location
        self.cur.execute("update Items set location=\'{loc}\' where id={id};".format(loc=location, id=self.id))
        # TODO:
        # send notification to users who watches the item to borrow
        # delete watchrequest after notification
        database_obj.conn.commit()
        try:
            users_watching = self.cur.execute(
                "select user_id from WatchRequests where item_id={item} and watch_method={wmethod}"
                    .format(item=self.id, wmethod=self.WATCH_REQUEST_TYPES["BORROW"])).fetchall()
            self.cur.execute(
                "delete from WatchRequests where item_id={item} and  watch_method={wmethod}"
                    .format(item=self.id, wmethod=self.WATCH_REQUEST_TYPES["BORROW"]))
            database_obj.conn.commit()
        except:
            users_watching = []
        if users_watching == []:
            return "You returned the item '{id}'".format(id=self.id)
        for user in users_watching[0]:
            database_obj.insert("Notifications",
                        ("sender", "receiver", "item_id", "notification_text", "notification_date"),
                        self.owner.id, user, self.id,
                        " \'{item}\' item is returned.".format(item=self),
                        datetime.now())
        return "You returned the item '{id}'".format(id=self.id)

    def make_comment(self, database_obj, user, params):
        comment_text = params[0]
        try:
            friend_state = self.cur.execute(
                "select state from Friends where (sender_user = {self_id} and receiver_user = {user_id}) or (sender_user = {user_id} and receiver_user = {self_id})"
                    .format(self_id=self.owner, user_id=user.id)).fetchone()[0]
        except:
            friend_state = 0
        comment = self.cur.execute(
            "select comment from Items where id={item}"
                .format(item=self.id)).fetchone()[0]

        if (self.owner == user.id) or ((comment >= friend_state and friend_state is not 0) or (comment is 3)):
            database_obj.insert("Comments", ('user_id', 'item_id', 'comment', 'date'), user.id, self.id, comment_text,
                                datetime.now())

            users_res = []
            try:
                users_watching = self.cur.execute(
                    "select user_id from WatchRequests where item_id={item} and watch_method={wmethod}".format(
                        item=self.id, wmethod=self.WATCH_REQUEST_TYPES["COMMENT"])).fetchall()
                for watcher in users_watching:
                    database_obj.insert("Notifications",
                                        ("sender", "receiver", "item_id", "notification_text", "notification_date"),
                                        user.id, watcher[0], self.id,
                                        "{user} commented for \'{item}\' item.".format(user=user.id, item=self),
                                        datetime.now())
                    users_res.append(watcher[0])
            except:
                pass

            return (("{user} commented for {owner}\'s \'{item}\' item.".format(owner=self.owner, user=user, item=self),
                     users_res), "Comment is made")
        else:
            return (('', []), "Friend_state of {user} with owner of item: ".format(user=user) + list(self.STATE.keys())[
                friend_state] + \
                    " Comment permission for this item: " + list(self.STATE_TYPE.keys())[comment] + \
                    " {user} has no permission to comment for this item.".format(user=user))

    def list_comments(self, database_obj, params):
        fetched_comments = self.cur.execute(
            'select user_id, comment from Comments where item_id={item} order by datetime(date);'
                .format(item=self.id)).fetchall()
        return fetched_comments

    def rate(self, database_obj, user, params):
        rating = params[0]
        self.cur.execute(
            'update Borrows set rate={rate} where item_id={item} and user_id ={user} and is_returned=1 ; '
                .format(user=user.id, item=self.id, rate=int(rating)))
        database_obj.conn.commit()
        return "You rated item {id} with {rate} successfully".format(id=self.id, rate=int(rating))

    def get_rating(self, database_obj, params):
        avg_rating = self.cur.execute(
            'select avg(rate) from Borrows where item_id={item} and rate is not null;'
                .format(item=self.id)).fetchone()[0]
        return "Average rating: {avg_rating}".format(avg_rating=avg_rating)

    def locate(self, database_obj, params):
        location = params[0]
        self.cur.execute(
            "update Items set location=\'{loc}\' where id={id};".format(loc=location, id=self.id))
        database_obj.conn.commit()
        return "You changed the location of item {id} as {loc}".format(id=self.id, loc=location)

    def setstate(self, database_obj, params):

        state_type, state = params
        setattr(self, state_type, state)
        self.cur.execute("update Items set {state_type}={state} where id={item};"
                         .format(item=self.id, state_type=state_type, state=self.STATE_TYPE[state]))
        database_obj.conn.commit()
        query = self.cur.execute("select i.view, i.detail, i.comment from Items i where i.id={item};"
                                 .format(item=self.id)).fetchone()
        state_view = query[0]
        state_detail = query[1]
        state_comment = query[2]
        if state_type == 'detail':
            if state_detail > state_view:
                self.cur.execute("update Items set {state_type}={state} where id={item};"
                                 .format(item=self.id, state_type="view", state=state_detail))
            if state_detail > state_comment:
                self.cur.execute("update Items set {state_type}={state} where id={item};"
                                 .format(item=self.id, state_type="comment", state=state_detail))
        if state_type == 'view':
            if state_detail > state_view:
                self.cur.execute("update Items set {state_type}={state} where id={item};"
                                 .format(item=self.id, state_type="detail", state=state_view))
                if state_view > state_comment:
                    self.cur.execute("update Items set {state_type}={state} where id={item};"
                                     .format(item=self.id, state_type="comment", state=state_view))

        if state_type == "borrow":
            users_res = []
            try:
                users_watching = self.cur.execute(
                    "select user_id from WatchRequests where item_id={item} and watch_method={wmethod}"
                        .format(item=self.id, wmethod=self.WATCH_REQUEST_TYPES["BORROW"])).fetchall()

                for watcher in users_watching:
                    database_obj.insert("Notifications",
                                        ("sender", "receiver", "item_id", "notification_text", "notification_date"),
                                        self.owner, watcher[0], self.id,
                                        "{user} changed borrow permission for \'{item}\' item to {state}.".format(
                                            user=self.owner, item=self, state=state), datetime.now())
                    users_res.append(watcher[0])

            except:
                pass

            return (("Borrow permission is changed for \'{item}\' item to {state}."
                     .format(item=self, state=state), users_res),
                    "You set {state_type} as {state} for item {id}.".format(state_type=state_type, state=state,
                                                                            id=self.id))

        if state_type == "comment":

            users_res = []
            try:
                users_watching = self.cur.execute(
                    "select user_id from WatchRequests where item_id={item} and watch_method={wmethod}"
                        .format(item=self.id, wmethod=self.WATCH_REQUEST_TYPES["COMMENT"])).fetchall()

                for watcher in users_watching:
                    database_obj.insert("Notifications",
                                        ("sender", "receiver", "item_id", "notification_text", "notification_date"),
                                        self.owner, watcher[0], self.id,
                                        "{user} changed comment permission for \'{item}\' item to {state}.".format(
                                            user=self.owner, item=self, state=state), datetime.now())
                    users_res.append(watcher[0])

            except:
                pass

            return (("Comment permission is changed for \'{item}\' item to {state}."
                     .format(item=self, state=state), users_res),
                    "You set {state_type} as {state} for item {id}.".format(state_type=state_type, state=state,
                                                                            id=self.id))
        else:
            return ('', []), "{state_type} is changed to {state}".format(state_type=state_type, state=state)

    @classmethod
    def search(cls, database_obj, user, params):
        search_text, genre, year, for_borrow = params

        if year == '':
            year = None
        else:
            year = int(year)
        if for_borrow == '':
            for_borrow = False
        else:
            for_borrow= True

        db_cur = database_obj.curs
        words_text = list(search_text.split(" "))
        words = []
        for i in words_text:
            words.append("%" + i + "%")
        # in sqlite default like statement is case insensitive already
        if year:
            if for_borrow:
                f_string = f'SELECT owner, id ' + \
                           f'FROM Items WHERE ' + \
                           f'borrow!=0 and (borrow=3 or (owner in ' + \
                           f'(SELECT sender_user from Friends where receiver_user={"?"}  and Items.borrow>=state)  or ' + \
                           f'owner in (SELECT receiver_user from Friends where sender_user={"?"}  and Items.borrow>=state))) ' + \
                           f'and (({" or ".join(["artist like ?"] * len(words_text))}) or ' + \
                           f'({" or ".join(["title like ?"] * len(words_text))})) and datetime(year)>={"?"} '+\
                           f'and genre like {"?"} ;'
                data = [user.id]*2 + words + words + [year] + ['%'+ genre+'%']
                list_user_item = db_cur.execute(f_string, data).fetchall()

            else:
                f_string = f'SELECT owner, id ' + \
                           f'FROM Items WHERE ' + \
                           f'(({" or ".join(["artist like ?"] * len(words))}) or ' + \
                           f'({" or ".join(["title like ?"] * len(words))})) and ' + \
                           f'datetime(year)>={"?"} and genre like {"?"} ;'
                data = words + words + [year] + ['%'+genre+'%']

                list_user_item = db_cur.execute(f_string, data).fetchall()
        else:
            if for_borrow:
                f_string = f'SELECT owner, id ' + \
                           f'FROM Items WHERE ' + \
                           f'borrow!=0 and (borrow=3 or ' + \
                           f'(owner in (SELECT sender_user from Friends where receiver_user={"?"} and Items.borrow>=state) or ' +\
                           f'owner in (SELECT receiver_user from Friends where sender_user={"?"}  and Items.borrow>=state))) ' +\
                           f'and (({" or ".join(["artist like ?"] * len(words_text))}) or ' +\
                           f'({" or ".join(["title like ?"] * len(words_text))})) and genre like {"?"} ;'
                # arkadaş değiller ama ödünç alma herkese açık
                data = [user.id]*2 + words + words + ['%'+genre+'%']
                list_user_item = db_cur.execute(f_string, data).fetchall()
            else:
                f_string = f'SELECT  owner, id  ' + \
                           f'FROM Items WHERE ' + \
                           f'(({" or ".join(["artist like ?"] * len(words))}) or ' + \
                           f'({" or ".join(["title like ?"] * len(words))})) and ' + \
                           f'genre like {"?"} ;'
                data = words + words + ['%'+genre+'%']
                list_user_item = db_cur.execute(f_string, data).fetchall()

        return list_user_item

    def _watch(self, database_obj, user, params):
        watch_method = params[0]
        try:
            friend_state = self.cur.execute(
                "select state from Friends where (sender_user = {self_id} and receiver_user = {user_id}) or (sender_user = {user_id} and receiver_user = {self_id})"
                    .format(self_id=self.owner, user_id=user.id)).fetchone()[0]
        except:
            friend_state = 0
        if watch_method == 'COMMENT':
            detail = self.cur.execute(
                "select detail from Items where id={item}".format(item=self.id)).fetchone()[0]
            if detail == 0 or detail < friend_state:
                return "You have no permission to follow comments."
        else:
            borrow = self.cur.execute(
                "select borrow from Items where id={item}".format(item=self.id)).fetchone()[0]
            if borrow == 0 or borrow < friend_state:
                return "You have no followng permission to borrow."
        database_obj.insert('WatchRequests', ('user_id', 'item_id', 'watch_method'), user.id, self.id, self.WATCH_REQUEST_TYPES[watch_method])
        return "'{user}' watches {id} {title} to '{wmethod}'.".format(user=user, id=self.id, title=self.title, wmethod=watch_method)

    def view_info(self, database_obj, user, params):
        try:
            friend_state = self.cur.execute(
                "select state from Friends where (sender_user = {self_id} and receiver_user = {user_id}) or (sender_user = {user_id} and receiver_user = {self_id})"
                    .format(self_id=self.owner, user_id=user.id)).fetchone()[0]
        except:
            friend_state = 0
        view = self.cur.execute(
                "select i.view from Items i where i.id={item}"
                .format(item=self.id)).fetchone()[0]

        if (view >= friend_state and friend_state is not 0) or (view is 3):
            sum_info = self.cur.execute(
                     "select type, title, artist, genre from Items where id = {id}"
                .format(id=self.id)).fetchall()
            return sum_info

        else:
            return "Friend_state of {user} with owner of item: ".format(user=user) + list(self.STATE.keys())[
                friend_state] + \
            " Comment permission for this item: " + list(self.STATE_TYPE.keys())[view] + \
            " {user} has no permission to view information for this item.".format(user=user)

    def detailed_info(self, database_obj, user, params):  #location none?
        try:
            friend_state = self.cur.execute(
                "select state from Friends where (sender_user = {self_id} and receiver_user = {user_id}) or (sender_user = {user_id} and receiver_user = {self_id})"
                    .format(self_id=self.owner, user_id=user.id)).fetchone()[0]
        except:
            friend_state = 0
        detail = self.cur.execute(
                "select i.detail from Items i where i.id={item}"
                .format(item=self.id)).fetchone()[0]

        if (detail >= friend_state and friend_state is not 0) or (detail is 3):
            detailed_info = self.cur.execute(
                "select i.owner, i.type, i.title, i.artist, i.genre, i.year, i.location from Items i where i.id = {self_id}"
                    .format(self_id=self.id)).fetchall()
            return detailed_info

        else:
            return "Friend_state of {user} with owner of item: ".format(user=user) + list(self.STATE.keys())[
                friend_state] + \
                   " Comment permission for this item: " + list(self.STATE_TYPE.keys())[detail] + \
                   " {user} has no permission to view detailed information for this item.".format(user=user)

    def announce(self, database_obj, user, params):
        owner_type, msg = params
        if self.owner!=user.id:
            return "You don't have permission to make announcement about the item."
        else:
            database_obj.insert("Announcements", ("item_id", "friend_state", "msg"), self.id, owner_type, msg)
            result = []
            try:
                query_1 = self.cur.execute(
                    "select sender_user from Friends where receiver_user={self_id} and state <= {owner_type} and state!=0 "
                    .format(self_id=self.owner, owner_type=owner_type)).fetchall()
                query_2 = self.cur.execute(
                    "select receiver_user from Friends where sender_user={self_id} and state <= {owner_type} and state!=0 "
                        .format(self_id=self.owner, owner_type=owner_type)).fetchall()
                for i in query_1:
                    result.append(i[0])
                for i in query_2:
                    result.append(i[0])
            except:
                pass
            return ((msg, result), 'All announcements are made.')

    def delete(self, database_obj, params):
        user_ids = []
        try:
            query = self.cur.execute(
                "select user_id from BorrowRequests where item_id = {item_id}".format(item_id=self.id)).fetchall()
            for i in query:
                user_ids.append(i[0])
        except:
            pass
        self.cur.execute("delete from Items where id = {item_id}".format(item_id=self.id))
        self.cur.execute("delete from BorrowRequests where item_id = {item_id}".format(item_id=self.id))
        self.cur.execute("delete from WatchRequests where item_id = {item_id} and (watch_method={breq} or watch_method={creq})"
                         .format(item_id=self.id, breq=self.WATCH_REQUEST_TYPES["BORROW"], creq=self.WATCH_REQUEST_TYPES['COMMENT']))
        database_obj.conn.commit()
        return (("Item {i_id} {title} that you follow are deleted.".format(i_id=self.id, title=self.title), user_ids),
                "Item {id} is deleted.".format(id=self.id))

    def __repr__(self):
        return self.title

