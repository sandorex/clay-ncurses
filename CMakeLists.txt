cmake_minimum_required(VERSION 3.22)

project(clay-testing VERSION 0.1
                     DESCRIPTION "Test using clay library"
                     LANGUAGES C)

if(EMSCRIPTEN)
    # build raylib for web
    set(PLATFORM Web CACHE STRING "" FORCE)

    # some extras to allow raylib to work properly on web
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# default to debug build
if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
        STRING "Choose the type of build." FORCE)
endif()

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# do not build raylib extras
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG        c1ab645ca298a2801097931d1079b10ff7eb9df8 # 5.5
    GIT_PROGRESS   TRUE
)
# FetchContent_MakeAvailable(raylib)

FetchContent_Declare(
    clay
    GIT_REPOSITORY    "https://github.com/nicbarker/clay.git"
    GIT_TAG           2f67b612560cca6aadb145560beffae7a0460534 # v0.12
    GIT_PROGRESS      TRUE
    SOURCE_SUBDIR     "nonexistant" # non existing directory so project is not built
)

FetchContent_MakeAvailable(clay)
add_library(clay INTERFACE)
target_include_directories(clay INTERFACE ${clay_SOURCE_DIR})

set(CURSES_USE_NCURSES TRUE CACHE BOOL "" FORCE)
set(CURSES_NEED_WIDE TRUE CACHE BOOL "" FORCE)
find_package(Curses REQUIRED)

add_executable(demo src/main.c)
target_link_libraries(demo PUBLIC clay ${CURSES_LIBRARIES})
target_include_directories(demo PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src" ${CURSES_INCLUDE_DIR})
