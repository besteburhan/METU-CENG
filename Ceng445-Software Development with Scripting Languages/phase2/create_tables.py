import sqlite3

conn = sqlite3.connect('data.db')
curs = conn.cursor()

curs.execute("DROP TABLE IF EXISTS Users;")
curs.execute('DROP TABLE IF EXISTS Items;')
curs.execute('DROP TABLE IF EXISTS BorrowRequests;')
curs.execute('DROP TABLE IF EXISTS WatchRequests;')
curs.execute('DROP TABLE IF EXISTS Announcements;')
curs.execute('DROP TABLE IF EXISTS Friends;')
curs.execute('DROP TABLE IF EXISTS Comments;')
curs.execute('DROP TABLE IF EXISTS Borrows;')
curs.execute('DROP TABLE IF EXISTS Notifications;')
curs.execute("CREATE TABLE IF NOT EXISTS Users(" +
             "id integer PRIMARY KEY AUTOINCREMENT," +
             "name char(50)," +
             "surname char(50)," +
             "email char(256) UNIQUE," +
             "password char(256)," +
             "is_verified integer," +
             "verification_number char(50)" +
             ");")
curs.execute("CREATE TABLE IF NOT EXISTS Items(" +
             "id integer PRIMARY KEY AUTOINCREMENT," +
             "owner integer," +
             "type char(150)," +
             "title char(256)," +
             "uniqueid integer UNIQUE," +
             "artist char(256)," +
             "genre char(150)," +
             "year integer," +
             "location char(256)," +
             "view integer ," +
             "detail integer ," +
             "borrow integer ," +
             "comment integer ," +
             "search integer ," +
             "FOREIGN KEY (owner) REFERENCES Users(id)" +
             ");")
curs.execute("CREATE TABLE IF NOT EXISTS Friends(" +
             "sender_user integer," +
             "receiver_user integer," +
             "state integer ," +
             "PRIMARY KEY(sender_user, receiver_user)," +
             "FOREIGN KEY (sender_user) REFERENCES Users(id)," +
             "FOREIGN KEY (receiver_user) REFERENCES Users(id)" +
             ");")
curs.execute("CREATE TABLE IF NOT EXISTS Comments(" +
             "id integer PRIMARY KEY AUTOINCREMENT," +
             "user_id  integer," +
             "item_id integer ," +
             "comment text," +
             "date text," +
             "FOREIGN KEY (user_id) REFERENCES Users(id)," +
             "FOREIGN KEY (item_id) REFERENCES Items(id)" +
             ");")
curs.execute("CREATE TABLE IF NOT EXISTS Announcements(" +
             "id integer PRIMARY KEY AUTOINCREMENT," +
             "item_id integer ," +
             "friend_state integer ," +
             "msg text, " +
             "FOREIGN KEY (item_id) REFERENCES Items(id)" +
             ");")
curs.execute("CREATE TABLE IF NOT EXISTS WatchRequests(" +
             "id integer PRIMARY KEY AUTOINCREMENT," +
             "user_id integer," +
             "item_id integer ," +
             "watch_method integer ," +
             "followed_id integer," +
             "FOREIGN KEY (user_id) REFERENCES Users(id)," +
             "FOREIGN KEY (followed_id) REFERENCES Users(id), " +
             "FOREIGN KEY (item_id) REFERENCES Items(id)" +
             ");")
curs.execute("CREATE TABLE IF NOT EXISTS Borrows(" +
             "id integer PRIMARY KEY AUTOINCREMENT," +
             "user_id integer," +
             "item_id integer ," +
             "taking_date text," +
             "return_date text," +
             "rate text," +
             "is_returned integer," +
             "FOREIGN KEY (user_id) REFERENCES Users(id)," +
             "FOREIGN KEY (item_id) REFERENCES Items(id)" +
             ");")
curs.execute("CREATE TABLE IF NOT EXISTS BorrowRequests(" +
             "user_id integer," +
             "item_id integer ," +
             "request_date text," +
             "PRIMARY KEY(user_id, item_id)," +
             "FOREIGN KEY (user_id) REFERENCES Users(id)," +
             "FOREIGN KEY (item_id) REFERENCES Items(id)" +
             ");")
curs.execute("CREATE TABLE IF NOT EXISTS Notifications(" +
             "id integer PRIMARY KEY AUTOINCREMENT," +
             "sender integer," +
             "receiver integer," +
             "item_id integer ," +
             "notification_text text ," +
             "notification_date text," +
             "FOREIGN KEY (sender) REFERENCES Users(id)," +
             "FOREIGN KEY (receiver) REFERENCES Users(id)," +
             "FOREIGN KEY (item_id) REFERENCES Items(id)" +
             ");")
conn.commit()
conn.close()