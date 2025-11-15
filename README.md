# MetaGraph API Examples

This repository contains examples demonstrating how to use MetaGraph as a C++ library for working with annotated de Bruijn graphs.

## Overview

MetaGraph is a tool for scalable construction and indexing of genome graphs and sequence-to-graph alignment. This repository provides practical examples of using MetaGraph's C++ API in your own applications.

## Two Integration Approaches

This repository demonstrates **two different ways** to integrate MetaGraph into your CMake project:

### 1. [`add_subdirectory`](subdirectory/) - Simple & Direct
Best for active development and simpler projects. MetaGraph is included as a git submodule.

**[📖 See subdirectory/README.md](subdirectory/README.md)** for full instructions.

### 2. [`ExternalProject_Add`](ExternalProject/) - Automatic & Isolated  
Best for downloading MetaGraph from Git automatically with stricter build isolation.

**[📖 See ExternalProject/README.md](ExternalProject/README.md)** for full instructions.

## Repository Structure

```
metagraph-examples/
├── subdirectory/              # Example using add_subdirectory
│   ├── CMakeLists.txt
│   ├── README.md
│   └── metagraph/             # MetaGraph submodule
├── ExternalProject/           # Example using ExternalProject_Add
│   ├── CMakeLists.txt
│   └── README.md
├── examples/                  # Example programs (shared)
│   └── basic_query.cpp
├── data/                      # Test data
│   ├── graphs/                # Pre-built test graphs
│   ├── test_DNA_query.fa
│   └── test_DNA_sequences.fa
└── README.md                  # This file
```

## Quick Start

Choose your preferred integration method and follow its README:

- **[subdirectory/README.md](subdirectory/README.md)** - For `add_subdirectory` approach
- **[ExternalProject/README.md](ExternalProject/README.md)** - For `ExternalProject_Add` approach

Both examples include:
- Pre-built test data (DNA and Protein graphs)
- A convenience target: run `make run_example` to execute with test data automatically
- Detailed build and usage instructions in their respective READMEs

## What's Included

### Example Code (`examples/basic_query.cpp`)

Demonstrates:
- Loading a de Bruijn graph from disk
- Loading annotations
- Querying sequences against the annotated graph
- Retrieving and displaying matching labels

### Test Data (`data/`)

Pre-built graphs and query sequences for both DNA and Protein alphabets:
- `graphs/test_DNA_graph.dbg` - Sample DNA de Bruijn graph
- `graphs/test_DNA_graph.column.annodbg` - Annotations for DNA graph
- `test_DNA_query.fa` - Query sequences
- Similar files for Protein alphabet

## License

This project follows the same license as MetaGraph. See [LICENSE](metagraph/LICENSE) for details.
