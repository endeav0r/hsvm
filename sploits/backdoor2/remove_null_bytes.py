#!/usr/bin/python2

import sys

inbytes = sys.stdin.read()

outbytes = ''

for byte in inbytes :
    if byte == chr(0x00) :
        outbytes += chr(0x90)
    else :
        outbytes += byte

sys.stdout.write(outbytes)