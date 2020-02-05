from subprocess import Popen, PIPE, TimeoutExpired
from typing import List
import threading
from threading import Thread
from time import sleep

some_expression_list = [
    [
        ['sign_up', 'Beyza','Butun','beyzabtn3@gmail.com','old'],
        ['login','beyzabtn3@gmail.com','old'],
        ['verify','beyzabtn3@gmail.com','111111'],
        ['add_item', 'first_item', 'title_first', '', 'artist_first', 'genre_first', '1995'],
        ['change password', 'old', 'new'],
        ['lookup', '[wrong@g.com,beyzabtn3@gmail.com]'],
        ['add_item', 'item type', 'title', '', 'artist', 'genre', '1995'],
        ['add_item', 'item type2', 'title2', '', 'artist2', 'genre2', '1995'],
        ['add_item', 'item type3', 'title3', '', 'artist3', 'genre3', '1995'],
        ['add_item', 'item type4', 'title4', '', 'artist4', 'genre4', '1995'],
        ['set friend', 'b@b.com', '1'],
        ['add_item', 'item type41', 'title41', '', 'artist41', 'genre41', '1995'],
        ['add_item', 'item type42', 'title42', '', 'artist42', 'genre42', '1985'],
        ['borrowed_by', '1', 'b@b.com', ''],
        ['add_item', 'item noct', 'titlenoct', '', 'artistnoct', 'genrenoct', '1985'],
        ['locate', '1', 'shelf'],
        ['set friend', 'd@k.com', '2'],
        ['get_rating', '1'],
        ['list_comments', '1'],
        ['announce', '1', '2','I will delete the item 1 soon!!'],
        ['add_item', 'item not', 'titlenot', '', 'artistnot', 'genrenot', '1985'],
        ['setstate', '1','comment', '3'],
        ['delete', '1']
    ],
    [
        ['sign_up', 'Beste','Burhan','b@b.com','old'],
        ['login','b@b.com','old'],
        ['verify','b@b.com','111111'],
        ['change password','old', 'new'],
        ['friend', 'beyzabtn3@gmail.com'],
        ['add_item', 'item type11', 'title31', '', 'artist31', 'genre31', '1995'],
        ['add_item', 'item type41', 'title41', '', 'artist41', 'genre41', '1995'],
        ['add_item', 'item type51', 'title51', '', 'artist51', 'genre51', '1995'],
        ['lookup', '[b@b.com,fgsdf@gmail.com]'],
        ['add_item', 'item type71', 'title71', '', 'artist71', 'genre71', '1995'],
        ['add_item', 'item type81', 'title91', '', 'artistk1', 'genrek1', '1995'],
        ['watch', 'beyzabtn3@gmail.com', '2'],
        ['add_item', 'item typdsfe71', 'titdsle71', '', 'artsdfist71', 'genredsf1', '1995'],
        ['add_item', 'item typsdfe81', 'titlsdfe91', '', 'artissdftk1', 'genresdfk1', '1995'],
        ['list_items', 'beyzabtn3@gmail.com'],
        ['borrowed_req', '1'],
        ['set friend', 'd@k.com', '1'],
        ['add_item', 'book', 'Zorba', '', 'Nikos', 'genre14', '1995'],
        ['returned', '1', ''],
        ['rate', '1', '10'],
        ['make_comment', '1', "Great book."],
        ['view_info', '1'],
        ['detailed_info', '1'],
        ['search', 'title', 'gen', '1990', ''],
        ['search', 'title', 'gen', '1990', '1'],



    ],
    [
        ['sign_up', 'Duru','Kilic','d@k.com','old'],
        ['login','d@k.com','old'],
        ['verify','d@k.com','111111'],
        ['change password','old', 'new'],
        ['friend', 'beyzabtn3@gmail.com'],
        ['friend', 'b@b.com'],
        ['add_item', 'item type11', 'title31', '', 'artist31', 'genre31', '1995'],
        ['add_item', 'item type41', 'title41', '', 'artist41', 'genre41', '1995'],
        ['add_item', 'item type51', 'title51', '', 'artist51', 'genre51', '1995'],
        ['lookup', '[d@k.com,fgsdf@gmail.com]'],
        ['add_item', 'item type71', 'title71', '', 'artist71', 'genre71', '1995'],
        ['add_item', 'item type81', 'title91', '', 'artistk1', 'genrek1', '1995'],
        ['item_watch', '2', '1'],
        ['item_watch', '1', '0'],
        ['add_item', 'item typdsfe71', 'titdsle71', '', 'artsdfist71', 'genredsf1', '1995'],
        ['add_item', 'item typsdfe81', 'titlsdfe91', '', 'artissdftk1', 'genresdfk1', '1995'],
        ['list_items', 'beyzabtn3@gmail.com'],
        ['borrowed_req', '1'],
        ['add_item', 'book', 'Zorba', '', 'Nikos', 'genre14', '1995'],
        ['returned', '1', ''],
        ['rate', '1', '10'],
        ['make_comment', '1', "Great book."],
        ['view_info', '1'],
        ['detailed_info', '1'],


    ]
]


def client_test(expr_set: List[List[str]]):
    f = open(str(threading.current_thread().ident) + ".txt", "w+")
    pipe = Popen(
        ['./client.py'],
        stdin=PIPE,
        stdout=f,
        text=True
    )
    for expr in expr_set:
        print('request: ',' '.join(expr), '\n')
        for exp in expr:
            print(exp, file=pipe.stdin, flush=True)

        sleep(.5)
    sleep(7.5)
    f.close()
    pipe.kill()

threads = []
for expr_set in some_expression_list:
    t = Thread(target=client_test, args=(expr_set,))
    threads.append(t)
    t.start()
