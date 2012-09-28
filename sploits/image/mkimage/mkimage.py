#!/usr/bin/python2

import sys

fh = open(sys.argv[1], 'r')
raw = fh.read()
fh.close()

i = 0
out = ''
while i < len(raw) :
	if i + 1 < len(raw) and raw[i] == raw[i + 1] :
		count = 1
		while count + i < len(raw) :
			if count == 126 :
				break
			if raw[i] == raw[i + count] :
				count += 1
			else :
				break
		out += chr((count) | 0x80) + raw[i]
		i += count
	else :
		out += raw[i]
		i += 1

size = len(raw)
sizechr = chr(size / 256) + chr(size % 256)

sys.stdout.write(sizechr + out + chr(0x00))