cmake_minimum_required(VERSION 3.16)

project(MetagraphExamples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add metagraph as a subdirectory
add_subdirectory(metagraph/metagraph)

# Create example executables
add_executable(basic_query examples/basic_query.cpp)

# Link against metagraph libraries - includes are inherited automatically
target_link_libraries(basic_query PRIVATE
    metagraph-cli
    metagraph-core
)
