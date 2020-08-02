import csv
import time
import sys
sys.setrecursionlimit(40000)


def pow_mod(c, t, n):
    if t < 2:
        return c % n
    return ((c ** (t % 2) % n) * pow_mod((c * c) % n, int(t//2), n)) % n


def inverse(a, n):
    t = 0
    newt = 1
    r = n
    newr = a
    while newr != 0:
        quotient = r // newr
        (t, newt) = (newt, t - quotient * newt)
        (r, newr) = (newr, r - quotient * newr)
    if r > 1:
        return "a is not invertible"
    if t < 0:
        t = t + n
    return t


def convert_to_text(n):
    digits = []
    while n:
        digits.append(n % 256)
        n //= 256
    return ''.join(map(chr, reversed(digits)))


with open('crackme.csv') as f:
    csv_file = csv.reader(f, delimiter=',')
    key_dict = {}
    for row in csv_file:
        key_dict[row[0]] = row[1]

n = int(key_dict['n'], 16)
c_1 = int(key_dict['c1'], 16)
c_2 = int(key_dict['c2'], 16)
e_1 = int(key_dict['e1'], 16)
e_2 = int(key_dict['e2'], 16)

y = inverse(e_1, e_2)
x = (1 - e_1 * y) // e_2

if x < 0:
    c2_y = pow_mod(inverse(c_2, n) % n, -x, n)
else:
    c2_y = pow_mod(c_2 % n, x, n)

if y < 0:
    c1_x = pow_mod(inverse(c_1, n) % n, -y, n)
else:
    c1_x = pow_mod(c_1 % n, y, n)


c1_mul_c2 = (c1_x * c2_y) % n

text = convert_to_text(c1_mul_c2)
print(text)
