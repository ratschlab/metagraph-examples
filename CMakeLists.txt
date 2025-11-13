cmake_minimum_required(VERSION 3.10)
project(MetagraphExample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the metagraph project as a subdirectory (from submodule)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/metagraph ${CMAKE_CURRENT_BINARY_DIR}/metagraph_build)

# Create the example executable
add_executable(example example.cpp)

# Link against metagraph libraries
target_link_libraries(example PRIVATE
    metagraph-cli
    metagraph-core
)

# Include the metagraph source directory to access headers
target_include_directories(example PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/metagraph/metagraph/src
    ${CMAKE_CURRENT_SOURCE_DIR}/metagraph/metagraph/api
)
