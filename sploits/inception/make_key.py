#!/usr/bin/python2

#plaintext:  0x9b 0x3a 0xf8 0xf8 0x53 0xbc 0x79 0xec
#ciphertext: 0xec 0x42 0x81 0x82 0x28 0xc0 0x4  0x92


import sys

fh = open('/dev/urandom', 'rb')
plaintext = fh.read(8)
fh.close()

plaintext = map(lambda x: ord(x), plaintext)

ciphertext = []
xx = 0x77
for p in plaintext :
	ciphertext.append(p ^ xx)
	xx += 1

print 'plaintext: ', ' '.join(map(lambda x: hex(x), plaintext))
print 'ciphertext:', ' '.join(map(lambda x: hex(x), ciphertext))