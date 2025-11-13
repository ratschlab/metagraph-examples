# MetaGraph API Examples

This directory contains example programs demonstrating various MetaGraph API features.

## Available Examples

### basic_query.cpp

A comprehensive example showing the fundamentals of using MetaGraph:

- **Loading graphs**: How to load a de Bruijn graph from disk
- **Loading annotations**: How to load and work with graph annotations
- **Querying sequences**: Mapping query sequences to graph nodes
- **Processing results**: Extracting and displaying matching labels

**Compile and run:**
```bash
cd ../build
make basic_query
./basic_query ../data/graphs/test_graph.dbg \
             ../data/graphs/test_graph.column.annodbg \
             ../data/test_query.fa
```

**Key API features demonstrated:**
- `mtg::cli::load_critical_dbg()` - Load graph
- `mtg::cli::parse_annotation_type()` - Detect annotation format
- `mtg::cli::initialize_annotation()` - Create annotation instance
- `annotation->load()` - Load annotation data
- `graph.map_to_nodes_sequentially()` - Map sequences to nodes
- `annotator.get_labels()` - Retrieve labels for nodes

## Adding Your Own Examples

To add a new example:

1. Create your `.cpp` file in this directory
2. Update `../CMakeLists.txt` to add your executable:
   ```cmake
   add_executable(your_example examples/your_example.cpp)
   target_link_libraries(your_example PRIVATE
       metagraph-cli
       metagraph-core
   )
   ```
3. Rebuild: `cd ../build && make`

## Common Patterns

### Loading an Annotated Graph

```cpp
#include "metagraph.hpp"
#include "cli/load/load_annotation.hpp"

using namespace mtg;
using namespace mtg::cli;
using namespace mtg::graph;

// Load graph
auto graph = load_critical_dbg("graph.dbg");

// Load annotation
auto anno_type = parse_annotation_type("graph.column.annodbg");
auto annotation = initialize_annotation(anno_type);
annotation->load("graph.column.annodbg");

// Combine into annotated graph
auto anno_graph = std::make_unique<AnnotatedDBG>(
    graph, 
    std::move(annotation)
);
```

### Reading FASTA Files

```cpp
#include "seq_io/sequence_io.hpp"

seq_io::read_fasta_file_critical("sequences.fa",
    [&](seq_io::kseq_t* read) {
        std::string name = read->name.s;
        std::string sequence = read->seq.s;
        // Process sequence...
    }
);
```

### Mapping Sequences to Nodes

```cpp
std::vector<DeBruijnGraph::node_index> nodes;
graph.map_to_nodes_sequentially(sequence,
    [&](DeBruijnGraph::node_index node) {
        nodes.push_back(node);
    }
);

// Check for matches
for (auto node : nodes) {
    if (node != DeBruijnGraph::npos) {
        // Node matched in graph
        auto labels = annotator.get_labels(node);
        // Process labels...
    }
}
```

### Getting Annotations for Nodes

```cpp
// Get labels for a single node
auto labels = anno_graph->get_annotator().get_labels(node_index);

// Iterate through labels
for (const auto& label : labels) {
    std::cout << "Label: " << label << std::endl;
}

// Get all labels in the annotation
const auto& label_encoder = anno_graph->get_annotator().get_label_encoder();
for (size_t i = 0; i < label_encoder.size(); ++i) {
    std::cout << label_encoder.decode(i) << std::endl;
}
```

## Tips

- Always check if nodes match: `node != DeBruijnGraph::npos`
- Use `map_to_nodes_sequentially()` to maintain order
- Label encoder indices start at 0
- Include necessary headers explicitly (don't rely on transitive includes)
- Use smart pointers (`std::unique_ptr`, `std::shared_ptr`) for graph objects

## Further Reading

- [../README.md](../README.md) - Main project documentation
- [API Status](../metagraph/metagraph/api/Status.md) - Current API status
- [MetaGraph Docs](https://metagraph.ethz.ch) - Full documentation
