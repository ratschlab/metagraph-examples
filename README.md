# MetaGraph API Examples

This repository contains examples demonstrating how to use MetaGraph as a C++ library for working with annotated de Bruijn graphs.

## Overview

MetaGraph is a tool for scalable construction and indexing of genome graphs and sequence-to-graph alignment. This repository provides practical examples of using MetaGraph's C++ API in your own applications.

## Repository Structure

```
metagraph-examples/
├── examples/           # Example programs
│   └── basic_query.cpp # Basic querying example
├── data/              # Test data
│   ├── graphs/        # Pre-built test graphs
│   ├── test_sequences.fa
│   └── test_query.fa
├── metagraph/         # MetaGraph submodule
├── CMakeLists.txt     # Build configuration
└── README.md          # This file
```

## Prerequisites

- C++17 or later
- CMake 3.16+
- Standard build tools (gcc/clang, make)
- Git (for submodules)

## Quick Start

### 1. Clone with Submodules

```bash
git clone --recursive https://github.com/ratschlab/metagraph-examples.git
cd metagraph-examples
```

If you already cloned without `--recursive`:

```bash
git submodule update --init --recursive
```

### 2. Build

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

This will build the example programs.

### 3. Run Examples

The repository includes pre-built test data, so you can run examples immediately:

```bash
# Basic query example
./build/basic_query \
    data/graphs/test_graph.dbg \
    data/graphs/test_graph.column.annodbg \
    data/test_query.fa
```

## Examples

### basic_query

Demonstrates:
- Loading a de Bruijn graph from disk
- Loading annotations
- Mapping sequences to graph nodes
- Retrieving labels for matched nodes
- Processing and displaying results

**Usage:**
```bash
./basic_query <graph.dbg> <annotation.annodbg> <query.fa>
```

**Expected Output:**
```
Loading graph from: data/graphs/test_graph.dbg
Graph loaded successfully
  k: 31
  Number of nodes: 25

Loading annotation from: data/graphs/test_graph.column.annodbg
Annotation loaded successfully
  Number of labels: 1
  Sample labels:
    - data/test_sequences.fa

Querying sequences from: data/test_query.fa

Sequence: query1
  Length: 37 bp
  K-mers matched: 7/7 (100%)
  Matching labels:
    - data/test_sequences.fa (in 7 k-mers)

Sequence: query2
  Length: 38 bp
  K-mers matched: 8/8 (100%)
  Matching labels:
    - data/test_sequences.fa (in 8 k-mers)

2 sequence(s) processed successfully

Example completed successfully!
```

## Using MetaGraph in Your Project

### Include MetaGraph

Add as a submodule:
```bash
git submodule add https://github.com/ratschlab/metagraph.git
```

### CMake Configuration

```cmake
cmake_minimum_required(VERSION 3.16)
project(YourProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add MetaGraph
add_subdirectory(metagraph/metagraph)

# Your executable
add_executable(your_app main.cpp)

# Link MetaGraph libraries
target_link_libraries(your_app PRIVATE
    metagraph-cli
    metagraph-core
)
```

### Basic Code Example

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
    auto anno_graph = std::make_unique<AnnotatedDBG>(graph, std::move(annotation));
    
    // Query sequences
    seq_io::read_fasta_file_critical("query.fa",
        [&](seq_io::kseq_t* read) {
            std::vector<DeBruijnGraph::node_index> nodes;
            anno_graph->get_graph().map_to_nodes_sequentially(
                read->seq.s,
                [&](auto node) { nodes.push_back(node); }
            );
            
            // Process nodes and get labels...
        }
    );
    
    return 0;
}
```

## License

This project follows the same license as MetaGraph. See [LICENSE](metagraph/LICENSE) for details.
