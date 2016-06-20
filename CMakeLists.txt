cmake_minimum_required (VERSION 2.8)
project(TPS2)

file(GLOB_RECURSE OBJS_SOURCE src/*.cpp)
file(GLOB_RECURSE HEADER_FILES src/*.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(src)
include_directories(lib/include)

add_executable(${PROJECT_NAME} ${OBJS_SOURCE} ${HEADER_FILES})

if (UNIX)
	set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")
	INCLUDE(FindPkgConfig)

	PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
	PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

	INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
	TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
	target_link_libraries(${PROJECT_NAME} pthread)
endif(UNIX)

if(WIN32)
	file(GLOB_RECURSE WIN_NATIVES lib/native/windows/x64/*.lib)
	target_link_libraries(${PROJECT_NAME} ${WIN_NATIVES})
	set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)
endif(WIN32)
