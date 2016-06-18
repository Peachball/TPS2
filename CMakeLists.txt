cmake_minimum_required (VERSION 3.4)
project(TPS2)

file(GLOB_RECURSE OBJS_SOURCE src/*.cpp)
file(GLOB_RECURSE HEADER_FILES src/*.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(src)
include_directories(lib/include)

add_executable(${PROJECT_NAME} ${OBJS_SOURCE} ${HEADER_FILES})

if (UNIX)

endif(UNIX)

if(WIN32)
	file(GLOB_RECURSE WIN_NATIVES lib/native/windows/x64/*.lib)
	target_link_libraries(${PROJECT_NAME} ${WIN_NATIVES})
endif(WIN32)
