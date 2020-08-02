import sys
sys.setrecursionlimit(40000)


def convert_to_text(n):
    digits = []
    while n:
        digits.append(n % 256)
        n //= 256
    return ''.join(map(chr, reversed(digits)))


def pow_mod(c, t, n):
    if t < 2:
        return c % n
    return ((c ** (t % 2) % n) * pow_mod((c * c) % n, int(t // 2), n)) % n


i = 0
group = 0
bits = ""
freq = []
for line in open("ptrace.trc").readlines():

    # print(float(line))
    if float(line) > 5:
        group += 1
    elif group != 0:
        freq.append(group)
        group = 0

for f in freq:
    if f >= 120:
        bits += "1"
    else:
        bits += "0"

key = int(bits, 2)

cipher_text = input()
n = input()

cipher_text = int(cipher_text, 16)
n = int(n, 16)

m = pow_mod(cipher_text % n, key, n) % n

text = convert_to_text(m)
print(text)
