import sqlite3


class DBConnection:

    def __init__(self, conn, curs):
        self.conn = conn
        self.curs = curs

    def insert(self, table_name, field_names, *data):
        f_string = f'INSERT INTO {table_name} {field_names} VALUES ( {",".join(["?"]*len(data))})'

        self.curs.execute(f_string, data)

        self.conn.commit()

    def update(self, table_name, updated_mem, filtered_mem, *data ):
        self.curs.execute(
            f'UPDATE {table_name} SET {updated_mem} = ? WHERE {filtered_mem} = ?;', data)
        self.conn.commit()



# db = DBConnection()