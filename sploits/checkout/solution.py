#!/usr/bin/python2

import socket

SERVER = 'localhost'
PORT   = 8000

fh = open('encrypted', 'rb')
raw = fh.read()
fh.close()

lenbytes = [0x00, 0x00, 0x00, len(raw) - 8]

def query (message) :
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.connect((SERVER, PORT))
	sock.sendall(message)
	result = sock.recv(1024)
	sock.close()
	return result

def testbit (bit) :
	tmp = raw
	bytemask = 1 << (7 - (bit % 8))
	lenbyte  = (bit / 8) % 4
	msgbyte  = (bit / 8) + 4
	#print 'bytemask', hex(bytemask)
	#print 'lenbyte', lenbyte
	#print 'msgbyte', msgbyte


	tmpbytes = map(lambda x: ord(x), tmp)
	#print ''.join(map(lambda x: hex(x)[2:], tmpbytes))
	tmpbytes[lenbyte] ^= bytemask
	tmpbytes[msgbyte] ^= bytemask
	#print ''.join(map(lambda x: hex(x)[2:], tmpbytes))
	tmpbytes = map(lambda x: chr(x), tmpbytes)
	tmp = ''.join(tmpbytes)
	return query(tmp)

def lenbitset (bit) :
	bytemask = 1 << (7 - (bit % 8))
	lenbyte  = (bit / 8) % 4
	if lenbytes[lenbyte] & bytemask > 0 :
		return True
	else :
		return False

plaintext_bits = []
for b in range((len(raw) - 8) * 8) :
	result = testbit(b)
	if 'error' in result :
		if lenbitset(b) :
			plaintext_bits.append(1)
		else :
			plaintext_bits.append(0)
	else :
		if lenbitset(b) :
			plaintext_bits.append(0)
		else :
			plaintext_bits.append(1)

plaintext_bytes = []
for i in range(len(plaintext_bits) / 8) :
	byte = 0
	for b in range(8) :
		byte |= plaintext_bits[(i*8) + b] << (7 - b)
	plaintext_bytes.append(byte)

print 'recovered bits: ', ''.join(map(lambda x: str(x), plaintext_bits))
print 'string:', ''.join(map(lambda x: chr(x), plaintext_bytes))