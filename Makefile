all:
	gcc -Wall -O2 -o grabix grabix_main.cpp grabix.cpp bgzf.c -lstdc++ -lz
