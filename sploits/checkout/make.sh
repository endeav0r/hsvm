gcc -Wall -c -o src/rc4.o src/rc4.c
gcc -Wall -o encrypt src/encrypt.c src/rc4.o
gcc -Wall -o decrypt src/decrypt.c src/rc4.o
