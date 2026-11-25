CC = gcc 
CFLAGS = -Wall -Wextra
process : process.c 
	$(CC) $(CFLAGS) -o process process.c 
run: process
	./process 
clean:
	rm -f process 
.PHONY: run clean 