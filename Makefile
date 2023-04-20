# set the compiler
CC := g++

# name of executable
EXEC := export/game.exe

# includes
# needs mingw32 and SDL2
INC_DIR = ./src/include
LIB_DIR = ./src/lib

# set the compiler flags
CFLAGS := -w -g -I$(INC_DIR) -L$(LIB_DIR) -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2 -mwindows -lstdc++ -std=c++17 -static-libstdc++ -static-libgcc -xc++ -lopengl32 -Lpath -lglew32 -lglu32 -lSDL2_gpu -llua

# generate names of object files
OBJS := $(wildcard */*.cpp)

all: $(EXEC)

showfont: showfont.c Makefile
	$(CC) -o $@ $@.c $(CFLAGS) $(LIBS)
glfont: glfont.c Makefile
	$(CC) -o $@ $@.c $(CFLAGS) $(LIBS)
$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) $(OBJS) $(CFLAGS) -o $@
clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean