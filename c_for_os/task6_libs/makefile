CC      = gcc
CFLAGS  = -Wall -Wextra
LIB     = libmymath.a
OBJ     = mymath.o
TARGET  = app

all: $(TARGET)

# 1. object file (compile only)
$(OBJ): mymath.c mymath.h
	$(CC) $(CFLAGS) -c mymath.c -o $(OBJ)

# 2. static library (.a)
$(LIB): $(OBJ)
	ar rcs $(LIB) $(OBJ)

# 3. executable (main.c + libmymath.a)
$(TARGET): main.c $(LIB) mymath.h
	$(CC) $(CFLAGS) main.c -L. -lmymath -o $(TARGET)

# run
run: $(TARGET)
	./$(TARGET)

# cleanup
clean:
	rm -f $(OBJ) $(LIB) $(TARGET)

.PHONY: all run clean