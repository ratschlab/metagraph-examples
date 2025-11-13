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

This will build both MetaGraph and the example programs.

### 3. Prepare Test Data

Build a simple test graph using the provided test sequences:

```bash
cd ..  # Back to project root

# Build graph with k=31
./metagraph/metagraph/build/metagraph build \
    -k 31 -o data/graphs/test_graph data/test_sequences.fa

# Create annotations
./metagraph/metagraph/build/metagraph annotate \
    -i data/graphs/test_graph.dbg \
    --anno-filename \
    -o data/graphs/test_graph \
    data/test_sequences.fa
```

This creates:
- `data/graphs/test_graph.dbg` - The de Bruijn graph
- `data/graphs/test_graph.column.annodbg` - Column-compressed annotation

### 4. Run Examples

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

## Key API Components

### Graph Operations
- `load_critical_dbg(path)` - Load a de Bruijn graph
- `graph.get_k()` - Get k-mer size
- `graph.num_nodes()` - Get number of nodes
- `graph.map_to_nodes_sequentially(seq, callback)` - Map sequence to nodes

### Annotation Operations
- `parse_annotation_type(path)` - Detect annotation type from file
- `initialize_annotation(type)` - Create annotation object
- `annotation->load(path)` - Load annotation from disk
- `annotator.get_labels(node)` - Get labels for a node
- `annotator.num_labels()` - Get total number of labels

### Sequence I/O
- `read_fasta_file_critical(path, callback)` - Read FASTA/FASTQ files
- `kseq_t` - Structure representing a sequence record

## Documentation

- [MetaGraph Main Documentation](https://metagraph.ethz.ch)
- [API Status](../metagraph/metagraph/api/Status.md)
- [MetaGraph Paper](https://doi.org/10.1186/s13059-020-02237-7)

## Building Graphs

MetaGraph supports various graph construction modes and annotation types:

### Basic Graph Construction

```bash
# Simple construction
metagraph build -k 31 -o output input.fa

# With memory limit
metagraph build -k 31 --mem-cap-gb 10 -o output input.fa

# Canonical mode (recommended for DNA)
metagraph build -k 31 --mode canonical -o output input.fa
```

### Annotation Types

```bash
# Column annotation (default, good for most cases)
metagraph annotate -i graph.dbg --anno-filename -o output input.fa

# Row annotation (better for very sparse annotations)
metagraph annotate -i graph.dbg --anno-type row -o output input.fa

# With counts
metagraph annotate -i graph.dbg --count-kmers -o output input.fa
```

## Troubleshooting

### Build Issues

**Problem:** CMake can't find dependencies
```bash
# Make sure submodules are initialized
git submodule update --init --recursive
```

**Problem:** Compiler errors about C++ standard
```bash
# Ensure you're using C++17 or later
cmake .. -DCMAKE_CXX_STANDARD=17
```

### Runtime Issues

**Problem:** "Cannot load graph"
- Check file paths are correct
- Ensure graph files exist and are readable
- Verify graph was built with compatible MetaGraph version

**Problem:** Segmentation fault
- Check graph and annotation compatibility
- Ensure annotation was built for the same graph
- Verify k-mer size matches between graph and queries

**Problem:** Out of memory
- Use memory-mapped mode: `--mmap` flag when building graphs
- Reduce batch size for large-scale queries
- Use appropriate annotation type for your use case

## Performance Tips

1. **Use canonical mode** for DNA sequences to reduce graph size
2. **Build with release mode**: `cmake -DCMAKE_BUILD_TYPE=Release ..`
3. **Use parallel processing**: Most MetaGraph operations support `-p` flag
4. **Memory mapping**: Use `--mmap` for large graphs that don't fit in RAM
5. **Batch queries**: Process multiple sequences together for better throughput

## Contributing

Found a bug or want to add an example? Please open an issue or submit a pull request!

## License

This project follows the same license as MetaGraph. See [LICENSE](metagraph/LICENSE) for details.

## Citation

If you use MetaGraph in your research, please cite:

```bibtex
@article{karasikov2020metagraph,
  title={MetaGraph: Indexing and analysing nucleotide archives at petabase-scale},
  author={Karasikov, Mikhail and Mustafa, Harun and Danciu, Ioana and Zimmermann, Marc and Barber, Christopher and R{\"a}tsch, Gunnar and others},
  journal={bioRxiv},
  year={2020},
  publisher={Cold Spring Harbor Laboratory}
}
```

## Support

- GitHub Issues: https://github.com/ratschlab/metagraph/issues
- Documentation: https://metagraph.ethz.ch
- Paper: https://doi.org/10.1186/s13059-020-02237-7
