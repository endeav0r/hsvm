#!/usr/bin/python2

import sys

raw = sys.stdin.read()

rawbytes = map(lambda x: ord(x), raw)

xorbytes = []

for r in rawbytes :
	xorbytes.append(r ^ 0x99)

sys.stdout.write(''.join(map(lambda x: chr(x), xorbytes)))