CC = gcc
CFLAGS = -O2 -Wall -D_GNU_SOURCE
LDFLAGS = -lX11 -lXi
BIN = keylogger_Xi

.PHONY: all clean
all: $(BIN)

clean:
	rm -f $(BIN)

keylogger_Xi: keylogger_Xi.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<
