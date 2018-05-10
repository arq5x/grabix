CFLAGS ?= -Wall -O2
LDFLAGS += -lstdc++ -lz

# activating Link-time optimization
CFLAGS += -flto

all: grabix

grabix: grabix_main.cpp grabix.cpp bgzf.c grabix.h bgzf.h
	gcc $(CFLAGS) -o grabix grabix_main.cpp grabix.cpp bgzf.c $(LDFLAGS)
