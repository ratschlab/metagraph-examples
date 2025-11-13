# MetaGraph Basic Usage Example

This example demonstrates how to use MetaGraph as a library to load and query annotated de Bruijn graphs.

## What's Included

- `metagraph.hpp` - A convenience header in `metagraph/api/` that includes essential MetaGraph headers
- `example.cpp` - A complete example showing graph loading and querying
- `CMakeLists.txt` - CMake configuration for building the example

## Prerequisites

Before running this example, you need to create a test graph and annotation. You can use the sample data from the MetaGraph repository.

### Step 1: Build MetaGraph

From the MetaGraph root directory:

```bash
cd metagraph
mkdir -p build && cd build
cmake ..
make -j4
cd ../..
```

### Step 2: Create Test Data

Use the sample data to create a small graph and annotation:

```bash
cd metagraph/build

# Build a graph from test data
./metagraph_DNA build -v -k 10 -o test_graph \
    ../tests/data/transcripts_1000.fa

# Annotate the graph
./metagraph_DNA annotate -v \
    -i test_graph.dbg \
    --anno-filename \
    -o test_graph \
    ../tests/data/transcripts_1000.fa

cd ../..
```

This will create:
- `test_graph.dbg` - The de Bruijn graph
- `test_graph.column.annodbg` - Column-compressed annotation

## Building the Example

From the `examples/basic_usage` directory:

```bash
mkdir -p build
cd build
cmake ..
make
```

This will create the `example` executable.

## Running the Example

You'll need:
1. A graph file (`.dbg`)
2. An annotation file (`.annodbg`)
3. A query file (`.fa` or `.fasta`)

### Using the test data created above:

```bash
# From examples/basic_usage/build/
./example \
    ../../../metagraph/build/test_graph.dbg \
    ../../../metagraph/build/test_graph.column.annodbg \
    ../../../metagraph/tests/data/transcripts_1000.fa
```

### Expected Output

The example will:
1. Load the graph and print basic statistics (k-mer size, number of nodes)
2. Load the annotations and display the number of labels
3. Query sequences from the input file
4. Print matching labels for each sequence

Example output:
```
Loading graph from: test_graph.dbg
Graph loaded successfully
  k: 10
  Number of nodes: 12345

Loading annotation from: test_graph.column.annodbg
Annotation loaded successfully
  Number of labels: 100
  Sample labels:
    - sequence_1
    - sequence_2
    - sequence_3

Querying sequences from: query.fa

Sequence: sequence_1
  Length: 500 bp
  Matches: sequence_1	sequence_2

3 sequence(s) processed successfully

Example completed successfully!
```

## Using MetaGraph in Your Own Project

To use MetaGraph in your own C++ project:

### 1. Include the Header

```cpp
#include "metagraph.hpp"
```

### 2. Link Against MetaGraph Libraries

In your CMakeLists.txt:

```cmake
# Add MetaGraph as a subdirectory
add_subdirectory(path/to/metagraph/metagraph ${CMAKE_BINARY_DIR}/metagraph_build)

# Link your executable
target_link_libraries(your_executable PRIVATE
    metagraph-cli
    metagraph-core
)

# Include directories
target_include_directories(your_executable PRIVATE
    path/to/metagraph/metagraph/src
    path/to/metagraph/metagraph/api
)
```

### 3. Key Components

The main components you'll typically use:

- `mtg::cli::load_critical_dbg()` - Load a de Bruijn graph
- `mtg::cli::initialize_annotated_dbg()` - Load annotations
- `mtg::cli::QueryExecutor` - Execute queries on annotated graphs
- `mtg::cli::Config` - Configuration for various operations

## Further Reading

- [MetaGraph Documentation](https://metagraph.ethz.ch/static/docs/index.html)
- [Quick Start Guide](https://metagraph.ethz.ch/static/docs/quick_start.html)
- Main repository: https://github.com/ratschlab/metagraph

## Troubleshooting

### Build Errors

If you encounter build errors, make sure:
- You've built MetaGraph first (including all dependencies)
- CMake can find all required dependencies
- You're using C++17 or later

### Runtime Errors

Common issues:
- **"Cannot load graph"**: Check that the file path is correct and the file exists
- **"Graph version mismatch"**: The graph was built with a different version of MetaGraph
- **Segmentation fault**: Ensure your graph and annotation files are compatible

## License

This example code is provided under the same license as MetaGraph.
