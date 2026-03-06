CC = gcc

CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lSDL2 -lSDL2_ttf -lm

SRC = src/main.c src/tictactoe.c
OBJ = $(SRC:.c=.o)

TARGET = tictactoe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)