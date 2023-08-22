CFLAGS=-O3 -Wall -Wextra
PREFIX=$(HOME)/.local

all: classchange install

classchange: classchange.c
	$(CC) classchange.c -o classchange $(CFLAGS)
	strip classchange

install: classchange
	mkdir -p $(PREFIX)/bin
	install ./classchange $(PREFIX)/bin/classchange
