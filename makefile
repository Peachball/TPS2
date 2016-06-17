BUILD=build/
INCLUDE_PATHS = -Isrc/ -Ilib/include/

APP_NAME = TPS

CC = g++

COMPILER_FLAGS = -std=c++11 $(INCLUDE_PATHS)

GAME_OBJS = $(wildcard src/game/*.cpp)
GUI_OBJS = $(wildcard src/gui/*.cpp)

OBJS = $(wildcard src/*.cpp) $(GAME_OBJS) $(GUI_OBJS)

OBJ_FILES = $(addprefix $(BUILD), $(OBJS:.cpp=.o))

LINKER_FLAGS = -lSDL2 -lSDL2_image

build: $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -w $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILD)$(APP_NAME)

debug: $(OBJS)
	$(CC) $(OBJ_FILES) $(COMPILER_FLAGS) -g -O0 $(LINKER_FLAGS) -o $(APP_NAME)

run: $(BUILD)$(APP_NAME)
	exec $<

$(BUILD)%.o: %.cpp %.h
	mkdir -p $(dir $@)
	$(CC) -c $< $(COMPILER_FLAGS) -o $@

$(BUILD)src/Main.o: src/Main.cpp
	mkdir -p $(dir $@)
	$(CC) -c $< $(COMPILER_FLAGS) -o $@

clean:
	find . -type f -iname \*.swp -delete
	rm -R -- build/*

rebuild:
	make clean
	make build
