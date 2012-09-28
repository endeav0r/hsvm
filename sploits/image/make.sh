#!/bin/bash

../../assembler -o stage_1 stage_1.s
../../assembler -o stage_2 stage_2.s

./remove_null_bytes.py stage_1 stage_1_cleaned

