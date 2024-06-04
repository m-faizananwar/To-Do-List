CC = gcc
CFLAGS = -Iinclude -Wall
SRC = src/main.c src/assets.c
DEPS = include/ui.h include/assets.h include/auth.h include/conio.h
OBJ = keynotes

all: $(OBJ)

$(OBJ): $(SRC) $(DEPS)
	$(CC) $(CFLAGS) $(SRC) -o $(OBJ)

clean:
	rm -f $(OBJ)

run: all
	./$(OBJ)

.PHONY: all clean run
