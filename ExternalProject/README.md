# MetaGraph External Project Example

This example demonstrates how to use MetaGraph via CMake's `ExternalProject_Add` instead of `add_subdirectory`. This approach automatically downloads and builds MetaGraph from its Git repository without requiring a submodule.

This is useful when you want to:

- Download MetaGraph from a Git repository automatically (no submodule needed)
- Have more control over MetaGraph's build configuration
- Isolate MetaGraph's build in a separate build tree
- Pin to specific MetaGraph versions via Git tags

## Quick Start

### Building with the External Project Approach

```bash
# Navigate to the external project example directory
cd ExternalProject

# Create a build directory
mkdir build && cd build

# Configure
cmake -DCMAKE_BUILD_TYPE=Release ..

# Build (this will first build MetaGraph, then the example)
make -j$(nproc)
```

### Running the Example

```bash
# Easiest: use the convenience target (runs with correct arguments)
make run_example

# Or run directly with custom arguments (from ExternalProject/build/)
./external_query \
    ../../data/graphs/test_DNA_graph.dbg \
    ../../data/graphs/test_DNA_graph.column.annodbg \
    ../../data/test_DNA_query.fa
```

## How It Works

The `ExternalProject/CMakeLists.txt` file:

1. **Downloads from Git**: Uses `ExternalProject_Add` to clone MetaGraph from GitHub
2. **Builds Separately**: MetaGraph is built in its own build tree with all dependencies bundled
3. **Installs**: MetaGraph installs headers and bundled libraries to a local prefix
4. **Links**: Your executable links against the bundled `libmetagraph-core.a` and system dependencies
5. **Includes**: Include directories point to the installed headers

**Key simplification**: MetaGraph now bundles all internal dependencies (zlib, sdsl, htslib, spdlog, folly, etc.) into `libmetagraph-core.a` during installation. You only need to link against system dependencies like Boost::iostreams and OpenMP.

No submodule setup required - CMake handles the download automatically on first build!

## Using in Your Own Project

To use this approach in your own project:

### 1. Set up CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)
project(YourProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(ExternalProject)

# Download and build MetaGraph
ExternalProject_Add(metagraph-external
    GIT_REPOSITORY https://github.com/ratschlab/metagraph.git
    GIT_TAG master  # or a specific version tag
    SOURCE_SUBDIR metagraph
    CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
        -DMETAGRAPH_BUILD_EXECUTABLE=OFF
)

ExternalProject_Get_Property(metagraph-external INSTALL_DIR)

# Find system dependencies
find_package(OpenMP REQUIRED)
find_package(Boost REQUIRED COMPONENTS iostreams)

# Your executable
add_executable(your_app main.cpp)
target_include_directories(your_app PRIVATE ${INSTALL_DIR}/include)
target_link_directories(your_app PRIVATE ${INSTALL_DIR}/lib)
target_link_libraries(your_app PRIVATE 
    metagraph-cli
    metagraph-core
    Boost::iostreams
    OpenMP::OpenMP_CXX
    -ldeflate
    -latomic
)
add_dependencies(your_app metagraph-external)
```

### 2. System Dependencies

You need to have these installed on your system:

- **Boost** (with iostreams component)
- **OpenMP** (usually comes with your compiler)
- **libdeflate** (compression library)

On Ubuntu/Debian:
```bash
sudo apt-get install libboost-iostreams-dev libdeflate-dev
```

On macOS:
```bash
brew install boost libdeflate
```

### 3. Specify MetaGraph Version

To pin to a specific version:

```cmake
ExternalProject_Add(metagraph-external
    GIT_REPOSITORY https://github.com/ratschlab/metagraph.git
    GIT_TAG v1.0.0  # Pin to a specific version tag
    # or use a commit hash for exact reproducibility
    # GIT_TAG abc123def456...
    SOURCE_SUBDIR metagraph
    CMAKE_ARGS ...
)
```
