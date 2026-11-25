CC      = gcc
CFLAGS  = -Wall -Wextra
TARGETS = sender receiver

all: $(TARGETS)

sender: sender.c header.h
	$(CC) $(CFLAGS) -o sender sender.c

receiver: recv.c header.h
	$(CC) $(CFLAGS) -o recv recv.c

run: all
	./sender
	./recv

clean:
	rm -f $(TARGETS)

.PHONY: all run clean