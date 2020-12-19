cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_FLAGS "-g")
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project(tide)

# Add glad includes.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/include/)
set(GLAD_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/src/glad.c)

# Add freetype includes.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/freetype2/include/)
link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/freetype2/libfreetype.a)

# Add stb
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/stb/)

# Add glfw submodule
set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
add_subdirectory(dependencies/glfw)

# Set SOURCE to all .h, .cpp and .c files.
file(GLOB SOURCE
    "src/*.h"
    "src/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCE} ${GLAD_SOURCE})
target_link_libraries(${PROJECT_NAME} glfw)
