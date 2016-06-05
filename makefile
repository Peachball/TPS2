BUILD=./build/
INCLUDE_PATHS = -Isrc/

APP_NAME = Test

CC = g++

COMPILER_FLAGS = -w -std=c++11 $(INLCUDE_PATHS)

GAME_OBJS = $(wildcard src/game/*.cpp)
GUI_OBJS = $(wildcard src/gui/*.cpp)

OBJS = $(wildcard src/*.cpp) $(GAME_OBJS) $(GUI_OBJS)

LINKER_FLAGS = -lSDL2 -lSDL2_image

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) \
		-o $(BUILD)$(APP_NAME)

debug: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) -g -O0 $(LINKER_FLAGS) $(INCLUDE_PATHS) \
		-o $(BUILD)$(APP_NAME)

clean:
	find . -type f -iname \*.swp -delete
