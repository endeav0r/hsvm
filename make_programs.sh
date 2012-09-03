./assembler -o key programs/key.s stdlib/stdio.s
./assembler -o backdoor programs/backdoor.s stdlib/stdio.s
./assembler -o news programs/news.s stdlib/string.s stdlib/stdio.s
./assembler -o players programs/players.s programs/player.s stdlib/stdio.s stdlib/string.s stdlib/heap.s
./assembler -o hidden_key programs/hidden_key.s stdlib/string.s stdlib/stdio.s
./assembler -o inception programs/inception.s stdlib/stdio.s
