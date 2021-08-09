# Set a minimum CMake version
cmake_minimum_required(VERSION 3.20)

# Define our project name
project(SudokuSolver  
    VERSION 0.5
    LANGUAGES CXX)

# Set C++ 17 compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SFML_DIR "D:/_Code Libraries/SFML-2.5.1/lib/cmake/SFML")
set(SFML_STATIC_LIBRARIES TRUE)

# Look for the SFML components we're interested in
find_package(SFML 2.5.1 COMPONENTS audio graphics window system REQUIRED)

# Define a list of our source files
set(SOURCES
    Tilemap.cpp
    Tilemap.h
    main.cpp
)

# Define our target executable based on the source files above
add_executable(${PROJECT_NAME} WIN32 ${SOURCES})

# Link our executable to SFML and its dependencies
target_link_libraries(${PROJECT_NAME} sfml-audio sfml-graphics sfml-window sfml-system)