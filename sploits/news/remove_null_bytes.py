#!/usr/bin/python2

import sys

infile = sys.argv[1]
outfile = sys.argv[2]

fh = open(infile, "rb")
inbytes = fh.read()
fh.close()

outbytes = ''

for byte in inbytes :
    if byte == chr(0x00) :
        outbytes += chr(0x90)
    else :
        outbytes += byte

fh = open(outfile, "wb")
fh.write(outbytes)
fh.close()
