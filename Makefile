CFLAGS=-O2 -Wall -Wextra -lX11 -lpci
PREFIX=$(HOME)/.local

all: classchange

classchange: classchange.c
	$(CC) classchange.c -o classchange $(CFLAGS)
	strip classchange

install: classchange
	mkdir -p $(PREFIX)/bin
	install ./classchange $(PREFIX)/bin/classchange
