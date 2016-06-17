#Detect os
ifeq ($(OS),Windows_NT)
	os += WIN32
	ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
		os += AMD64
	endif
	ifeq ($(PROCESSOR_ARCHITECTURE),x86)
		os += IA32
	endif
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		os += LINUX
	endif
	ifeq ($(UNAME_S),Darwin)
		os += OSX
	endif
	UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_P),x86_64)
		os += AMD64
	endif
	ifneq ($(filter %86,$(UNAME_P)),)
		os += IA32
	endif
	ifneq ($(filter arm%,$(UNAME_P)),)
		os += ARM
	endif
endif

BUILD_LINUX =build/
INCLUDE_PATHS = -Isrc/

APP_NAME = TPS

CC = g++

COMPILER_FLAGS = -std=c++11 $(INCLUDE_PATHS)

GAME_OBJS = $(wildcard src/game/*.cpp)
GUI_OBJS = $(wildcard src/gui/*.cpp)

OBJS_LINUX = $(wildcard src/*.cpp) $(GAME_OBJS) $(GUI_OBJS)

OBJ_FILES_LINUX = $(addprefix $(BUILD), $(OBJS:.cpp=.o))

LINKER_FLAGS = -lSDL2 -lSDL2_image

ifneq ($(findstring WIN32,$(os)),)
	MAKE_DIR = mkdir
	REM_DIR = RM /S /Q
	BUILD=$(subst /,\,$(BUILD_LINUX))
#	OBJ_FILES=$(subst /,\,$(OBJ_FILES_LINUX))
	OBJS=$(subst /,\,$(OBJS_LINUX))
endif

ifneq ($(findstring LINUX,$(os)),)
	MAKE_DIR = mkdir -p
	REM_DIR = rm -rf
	PATH_SEP = /
	BUILD =$(BUILD_LINUX)
	OBJ_FILES =$(OBJ_FILES_LINUX)
	OBJS =$(OBJS_LINUX)
endif

test:
ifneq ($(findstring WIN32,$(os)),)
	echo windows
endif
	echo $(OBJ_FILES_LINUX)

build: $(OBJ_FILES_LINUX)
	echo $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -w $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(BUILD)$(APP_NAME)

debug: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) -g -O0 $(LINKER_FLAGS) -o $(APP_NAME)

run: $(BUILD)$(APP_NAME)
	exec $<

$(BUILD_LINUX)%.o: %.cpp
	$(MAKE_DIR) $(dir $@)
	$(CC) -c $< $(COMPILER_FLAGS) -o $@

%:
	echo hi


clean:
	$(REM_DIR) $(BUILD)
