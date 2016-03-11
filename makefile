BUILD=./build/
OBJS = $(wildcard src/*.cpp)

APP_NAME = Test

CC = g++
WINDOWSCC = i686-w64-mingw32-g++

COMPILER_FLAGS = -w -std=c++11

LINKER_FLAGS = -lSDL2 -lSDL2_image

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILD)$(APP_NAME)

clean:
	find . -type f -iname \*.swp -delete
