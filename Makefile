CC=g++
CFLAGS = -Wall -g
HEADERS = bloom.h 


all: test_bloom
	bin/./test_bloom >> output.txt

test_bloom:
	$(CC) $(CFLAGS) tests/test_bloom.cpp -o bin/test_bloom

