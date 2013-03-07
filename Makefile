all:
	gcc -Wall -o grabix grabix.cpp bgzf.c -lstdc++ -lz
