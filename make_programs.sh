./assembler -o key programs/key.s stdlib/stdio.s
./assembler -o backdoor programs/backdoor.s stdlib/stdio.s
./assembler -o backdoor1.5 programs/backdoor1.5.s stdlib/stdio.s stdlib/string.s
./assembler -o backdoor2 programs/backdoor2.s stdlib/stdio.s
./assembler -o news programs/news.s stdlib/string.s stdlib/stdio.s
./assembler -o players programs/players.s programs/player.s stdlib/stdio.s stdlib/string.s stdlib/heap.s
./assembler -o hidden_key programs/hidden_key.s stdlib/string.s stdlib/stdio.s
./assembler -o inception programs/inception.s stdlib/stdio.s
./assembler -o mod48 programs/mod48.s stdlib/string.s stdlib/stdio.s
