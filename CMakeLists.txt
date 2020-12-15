cmake_minimum_required(VERSION 3.17)

project(tide)

# Add glad includes.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/include/)
set(GLAD_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/src/glad.c)

# Add freetype includes.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/freetype2/include/)
link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/freetype2/libfreetype.a)

# Add glfw submodule
set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
add_subdirectory(dependencies/glfw)

# Add data_desk submodule
add_subdirectory(dependencies/data_desk)

# Set SOURCE to all .h, .cpp and .c files.
file(GLOB SOURCE
    "src/*.h"
    "src/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCE} ${GLAD_SOURCE})
target_link_libraries(${PROJECT_NAME} glfw)


