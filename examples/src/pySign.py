from numpy import cumsum
from sys import argv
import hashlib
from binascii import hexlify

ROLLSUM_CHAR_OFFSET = 31
length = 2048
header='727301360000080000000008'
header = header.decode('hex')


filename = argv[1]
fd_r = open(filename, 'rb')
fd_w = open(filename + '.sig', 'wb')


# d6 2b a1 27  -- weak sum
# 7e 3f dd 22 dd 5a 14 ec -- md4

def md4(data):
    res = hashlib.new('md4', data).digest()
    return res[:8]

def weakHash(data):
    length = len(data)
    data = [ord(i) for i in data]
    s2 = sum(cumsum(data))
    s1 = sum(data)
    s1 += length * ROLLSUM_CHAR_OFFSET
    s2 += ((length * (length + 1)) / 2) * ROLLSUM_CHAR_OFFSET
    res =  (s2 << 16) | (s1 & 0xffff)
    return res 



fd_w.write(hexlify(header).decode('hex'))
data = fd_r.read(length)
while data:
	d =  hex(weakHash(data))[5:-1] + hexlify(md4(data))  
	fd_w.write(d.decode('hex'))
	data = fd_r.read(length)

fd_w.close()
fd_r.close()
