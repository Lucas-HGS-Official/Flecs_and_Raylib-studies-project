TARGET = bin/game
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

run: clean default

default: $(TARGET)

$(TARGET): $(OBJ)
	gcc -o $@ $? -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

obj/%.o : src/%.c
	gcc -c $< -o $@ -Iinclude

clean:
	rm -f obj/*.o
	rm -f bin/*