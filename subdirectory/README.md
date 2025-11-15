# MetaGraph `add_subdirectory` Example

This example demonstrates how to integrate MetaGraph into your CMake project using `add_subdirectory`. This is the **simplest approach** for projects that want to include MetaGraph's source code directly.

## Key Features

- ✅ Simple CMake configuration
- ✅ Automatic include path propagation
- ✅ Direct access to MetaGraph targets
- ✅ Easy debugging into MetaGraph code
- ✅ Best for active development

## Prerequisites

- C++17 or later
- CMake 3.16+
- Git (for submodule)
- Standard build tools (gcc/clang, make)

## Quick Start

### 1. Initialize the Submodule

```bash
cd subdirectory
git submodule update --init --recursive
```

This will clone MetaGraph and all its dependencies into the `metagraph/` directory.

### 2. Build

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### 3. Run the Example

```bash
# Easiest: use the convenience target (runs with correct arguments)
make run_example

# Or run directly with custom arguments
./basic_query \
    ../../data/graphs/test_DNA_graph.dbg \
    ../../data/graphs/test_DNA_graph.column.annodbg \
    ../../data/test_DNA_query.fa
```

## How It Works

The `CMakeLists.txt` in this directory:

1. **Adds MetaGraph**: `add_subdirectory(metagraph/metagraph)`
2. **Creates executable**: `add_executable(basic_query ../examples/basic_query.cpp)`
3. **Links libraries**: Links against `metagraph-cli` and `metagraph-core`
4. **Includes automatically**: Include paths are inherited from MetaGraph targets

That's it! CMake handles the rest.

## Example Code

The example code in `../examples/basic_query.cpp` demonstrates:

- Loading a de Bruijn graph from disk
- Loading annotations
- Querying sequences against the graph
- Retrieving labels for matched k-mers

## Using in Your Project

To use this approach in your own project:

### 1. Add MetaGraph as a Submodule

```bash
git submodule add https://github.com/ratschlab/metagraph.git
git submodule update --init --recursive
```

### 2. Create Your CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)
project(YourProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add MetaGraph
add_subdirectory(metagraph/metagraph)

# Your executable
add_executable(your_app main.cpp)

# Link MetaGraph libraries - includes are inherited automatically
target_link_libraries(your_app PRIVATE
    metagraph-cli
    metagraph-core
)
```

### 3. Write Your Code

```cpp
#include "metagraph.hpp"
#include "cli/load/load_annotation.hpp"
#include "seq_io/sequence_io.hpp"

using namespace mtg;
using namespace mtg::cli;
using namespace mtg::graph;

int main() {
    // Load graph
    auto graph = load_critical_dbg("graph.dbg");
    
    // Load annotation
    auto anno_type = parse_annotation_type("graph.column.annodbg");
    auto annotation = initialize_annotation(anno_type);
    annotation->load("graph.column.annodbg");
    
    // Create annotated graph
    auto anno_graph = std::make_unique<AnnotatedDBG>(
        graph, std::move(annotation)
    );
    
    // Query sequences...
    
    return 0;
}
```

## Advantages

- **Simple setup**: Just add the subdirectory
- **Automatic includes**: No need to specify include paths
- **Shared targets**: Direct access to MetaGraph's CMake targets
- **IDE friendly**: Easy navigation and debugging
- **Fast incremental builds**: Shared build directory

## Disadvantages

- **Requires submodule**: Need to manage git submodule
- **Shared CMake scope**: MetaGraph's variables affect your project
- **Source must be local**: Can't download from Git automatically

## When to Use

Choose `add_subdirectory` when:

- You're actively developing with MetaGraph
- You want simple, straightforward CMake configuration
- You need to debug into MetaGraph's code
- Your project is tightly integrated with MetaGraph
- You prefer local source control

For automatic Git downloads and stricter isolation, see the [ExternalProject example](../ExternalProject/README.md).
