all:
	gcc -Wall -O2 -o grabix grabix.cpp bgzf.c -lstdc++ -lz
