#!/usr/bin/python2

fh = open('/dev/urandom', 'rb')
raw = fh.read(8)
fh.close()

for i in range(len(raw) / 2) :
	byte0 = ord(raw[i*2]) ^ 0x77
	byte1 = ord(raw[i*2+1]) ^ 0x77
	key = hex(byte0) + hex(byte1)[2:]
	print 'key' + str(i) + ' : ' + key

print 'flag: ' + ''.join(map(lambda x: hex(ord(x))[2:], raw))