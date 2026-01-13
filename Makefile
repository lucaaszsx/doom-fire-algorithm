# Project name
PROJ_NAME=doom_fire

# Project folders
SRC=./src
BUILD=./build

# .c files
C_SRC=$(wildcard $(SRC)/*.c)

# Object files
OBJ=$(subst .c,.o,$(subst $(SRC),$(BUILD),$(C_SRC)))

# Compiler
CC=gcc

# Compiler flags
CC_FLAGS=-c          \
         -W          \
         -Wall       \
         -std=c11    \
         -pedantic

# SDL3 and SDL3_ttf flags
SDL_CFLAGS=$(shell pkg-config --cflags sdl3 || echo -I/usr/include/SDL3)
SDL_LIBS=$(shell pkg-config --libs sdl3 || echo -lSDL3) -lSDL3_ttf -lm

#
# Compilation and linking
#
all: buildFolder $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
	@ echo 'Building binary using GCC Linker: $^'
	$(CC) $^ $(SDL_LIBS) -o $@
	@ echo 'Finished building binary: $@'
	@ echo ' '

$(BUILD)/main.o: $(SRC)/main.c
	@ echo 'Building main target using GCC Compiler: $<' 
	$(CC) $< $(CC_FLAGS) $(SDL_CFLAGS) -o $@
	@ echo ' '

buildFolder:
	@ mkdir $(BUILD) -p

clean:
	@ rm -rf $(BUILD)/*.o $(PROJ_NAME) *~
	@ rmdir $(BUILD)

run: all
	./$(PROJ_NAME)

.PHONY: all buildFolder clean run