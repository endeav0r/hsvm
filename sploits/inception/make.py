#!/usr/bin/python2

import sys

def encode (data) :
	tmp = map(lambda x: ord(x), data)
	tmp = map(lambda x: x ^ 0x88, tmp)
	tmp = map(lambda x: chr(x), tmp)
	return ''.join(tmp)

def SET (op0, op1) :
	return chr(0) + chr(op0) + chr(op1)

def ADD (op0, op1) :
	return chr(1) + chr(op0) + chr(op1)

def SUB (op0, op1) :
	return chr(2) + chr(op0) + chr(op1)

def XOR (op0, op1) :
	return chr(3) + chr(op0) + chr(op1)

def CMP (op0, op1) :
	return chr(4) + chr(op0) + chr(op1)

def JE (op0) :
	return chr(5) + chr(op0) + chr(0)

def JMP (op0) :
	return chr(6) + chr(op0) + chr(0)

def RET (op0) :
	return chr(7) + chr(op0) + chr(0)

def IN (op0) :
	return chr(8) + chr(op0) + chr(0)

def LOD (op0, op1) :
	return chr(9) + chr(op0) + chr(op1)

program =  SET(0x0, 0xec)
program += SET(0x1, 0x42)
program += SET(0x2, 0x81)
program += SET(0x3, 0x82)
program += SET(0x4, 0x28)
program += SET(0x5, 0xc0)
program += SET(0x6, 0x04)
program += SET(0x7, 0x92)
program += SET(0x8, 8)
program += SET(0x9, 0x77)
program += SET(0xa, 1)
program += SET(0xb, 0)
program += SET(0xc, 0)
program += SET(0xd, 0)

# LOOP
program += IN(0xb)
program += LOD(0xf, 0xd)
program += ADD(0xd, 0xa)
program += XOR(0xf, 0x9) # change to decryption
program += ADD(0x9, 0xa)
program += CMP(0xf, 0xb)
program += JE(1)
program += JMP(4)
program += SUB(0x8, 0xa)
program += CMP(0x8, 0xc)
program += JE(2)
program += JMP(-12%256)
# FAIL
program += RET(0xc)
# PASS
program += RET(0xa)

encoded = encode(program)

for i in range(len(encoded) / 2) :
	short  = ord(encoded[i * 2]) << 8
	short |= ord(encoded[i * 2 + 1])
	print 'prog' + str(i) + ': ' + hex(short)