BUILD=./build/
OBJS = $(wildcard src/*.cpp)

APP_NAME = Test

CC = g++

COMPILER_FLAGS = -w

LINKER_FLAGS = -lSDL2

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILD)$(APP_NAME)
