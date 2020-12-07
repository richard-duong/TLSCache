CC=g++
CFLAGS = -Wall -g -I/extern/libressl/include -L/extern/libressl/lib
HEADERS = bloom.h 


all: test_bloom client server
	bin/./test_bloom >> output.txt

test_bloom:
	$(CC) $(CFLAGS) tests/test_bloom.cpp -o bin/test_bloom

client:
	$(CC) $(CFLAGS) src/client.cpp -o bin/client

server:
	$(CC) $(CFLAGS) src/server.cpp -o bin/server

