# Test Data

This directory contains test data for running the examples.

## Files

### test_sequences.fa
Small set of test DNA sequences used to build the test graph.

- 3 sequences
- 64 bp each
- Used for graph construction

### test_query.fa
Query sequences to test against the graph.

- 2 sequences
- Designed to match subsequences in `test_sequences.fa`

### graphs/
Pre-built graphs and annotations (created by running build commands).

## Regenerating Test Data

If you need to rebuild the test graph:

```bash
# From project root
cd metagraph-examples

# Build graph
./metagraph/metagraph/build/metagraph build \
    -k 31 \
    -o data/graphs/test_graph \
    data/test_sequences.fa

# Create annotation
./metagraph/metagraph/build/metagraph annotate \
    -i data/graphs/test_graph.dbg \
    --anno-filename \
    -o data/graphs/test_graph \
    data/test_sequences.fa
```

This creates:
- `graphs/test_graph.dbg` - de Bruijn graph (k=31)
- `graphs/test_graph.column.annodbg` - column-compressed annotation

## Using Your Own Data

To use your own sequences:

1. Place FASTA files in this directory
2. Build graph: `metagraph build -k <k> -o data/graphs/your_graph your_sequences.fa`
3. Create annotation: `metagraph annotate -i data/graphs/your_graph.dbg --anno-filename -o data/graphs/your_graph your_sequences.fa`
4. Run examples with your graph files

## Data Format Requirements

- **FASTA format**: Standard FASTA with `>header` and sequence lines
- **DNA alphabet**: A, C, G, T (N's are typically skipped)
- **Sequence length**: Should be longer than k-mer size (k=31 by default)
