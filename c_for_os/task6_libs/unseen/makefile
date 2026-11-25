CC      = gcc
CFLAGS  = -Wall -Wextra
LIB     = libmath.a
OBJ     = math.o
TARGET  = app

all: $(TARGET)

# 1. object file
$(OBJ): math.c math.h
	$(CC) $(CFLAGS) -c math.c -o $(OBJ)

# 2. static library
$(LIB): $(OBJ)
	ar rcs $(LIB) $(OBJ)

# 3. executable (main.c + libcalc.a)
$(TARGET): mainn.c $(LIB) math.h
	$(CC) $(CFLAGS) mainn.c -L. -lmath -o $(TARGET)

# run
run: $(TARGET)
	./$(TARGET)

# cleanup
clean:
	rm -f $(OBJ) $(LIB) $(TARGET)

.PHONY: all run clean