CC ?= gcc
LIBS = -lpthread -lcurl -ldiscord -lsqlite3 #-latomic -lcrypto -lssl
CFLAGS = -Wall -fstack-protector-all -Wpedantic

SRC=$(wildcard src/*.c)

prod: $(SRC)
	$(CC) -o memobot $^ $(CFLAGS) -O2 $(LIBS)

debug: $(SRC)
	$(CC) -o memobot $^ $(CFLAGS) -g -O0 $(LIBS)




