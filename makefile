OBJS = $(wildcard src/*.cpp)

OBJ_NAME = Test

CC = g++

COMPILER_FLAGS = -w

LINKER_FLAGS = -lSDL2

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
