BUILD=build/
INCLUDE_PATHS = -Isrc/

APP_NAME = TPS

CC = g++

COMPILER_FLAGS = -w -std=c++11 $(INCLUDE_PATHS)

GAME_OBJS = $(wildcard src/game/*.cpp)
GUI_OBJS = $(wildcard src/gui/*.cpp)

OBJS = $(wildcard src/*.cpp) $(GAME_OBJS) $(GUI_OBJS)

OBJ_FILES = $(addprefix $(BUILD), $(OBJS:.cpp=.o))

LINKER_FLAGS = -lSDL2 -lSDL2_image

all : $(OBJ_FILES)
	$(CC) $(OBJ_FILES) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILD)$(APP_NAME)

debug: $(OBJS)
	$(CC) $(OBJ_FILES) $(COMPILER_FLAGS) -g -O0 $(LINKER_FLAGS) -o $(APP_NAME)

run: $(BUILD)$(APP_NAME)
	exec $<

$(BUILD)%.o: %.cpp
	mkdir -p $(dir $@)
	$(CC) -c $< $(COMPILER_FLAGS) $(INCLUDE_PATHS) -o $@

clean:
	find . -type f -iname \*.swp -delete
	rm -R -- build/*
