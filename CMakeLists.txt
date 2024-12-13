cmake_minimum_required(VERSION 3.10)

# Set the project name
project(GolfAtHome)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(GolfAtHome src/main.cpp src/lib/RP/RPUtlRandom.cpp src/lib/RP/RPGolConfig.cpp src/lib/RP/RPGolWindSet.cpp)

# Include debug symbols
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

target_link_libraries(GolfAtHome ${CMAKE_SOURCE_DIR}/)

# Include directories (if any)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/lib)

