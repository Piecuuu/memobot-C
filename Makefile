CC ?= gcc
LIBS = -lpthread -lcurl -ldiscord -lsqlite3 #-latomic -lcrypto -lssl
CFLAGS = -Wall -g -O0 -fstack-protector-all -Wpedantic

SRC=$(wildcard src/*.c)

all: $(SRC)
	$(CC) -o memobot $^ $(CFLAGS) $(LIBS)


