CC ?= gcc
LIBS = -lpthread -latomic -lcrypto -lssl -lcurl -ldiscord -lsqlite3
CFLAGS = -Wall -g -O0 -fstack-protector-all -Wpedantic

SRC=$(wildcard src/*.c)

all: $(SRC)
	$(CC) -o memobot $^ $(CFLAGS) $(LIBS)


