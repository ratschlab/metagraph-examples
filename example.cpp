/**
 * @file example.cpp
 * @brief Example demonstrating basic MetaGraph usage: loading and querying graphs
 *
 * This example shows how to:
 * 1. Load a de Bruijn graph from disk
 * 2. Load annotations for the graph
 * 3. Query sequences against the annotated graph
 *
 * Prerequisites:
 * Before running this example, you need to build a graph and annotation.
 * Use the Quick Start guide or the following commands:
 *
 *   metagraph build -k 10 -o graph transcripts_1000.fa
 *   metagraph annotate -i graph.dbg --anno-filename -o graph transcripts_1000.fa
 *
 * Then run this example with:
 *   ./example graph.dbg graph.column.annodbg query.fa
 */

#include <iostream>
#include <memory>
#include <string>

#include "metagraph.hpp"

using namespace mtg;
using namespace mtg::cli;
using namespace mtg::graph;

void print_usage(const char* program_name) {
    std::cerr << "Usage: " << program_name 
              << " <graph.dbg> <annotation.annodbg> <query.fa>\n"
              << "\n"
              << "Example:\n"
              << "  " << program_name 
              << " graph.dbg graph.column.annodbg query.fa\n"
              << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        print_usage(argv[0]);
        return 1;
    }

    std::string graph_path = argv[1];
    std::string annotation_path = argv[2];
    std::string query_path = argv[3];

    std::cout << "Loading graph from: " << graph_path << std::endl;
    
    // Load the de Bruijn graph
    auto graph = load_critical_dbg(graph_path);
    if (!graph) {
        std::cerr << "Error: Failed to load graph from " << graph_path << std::endl;
        return 1;
    }
    
    std::cout << "Graph loaded successfully" << std::endl;
    std::cout << "  k: " << graph->get_k() << std::endl;
    std::cout << "  Number of nodes: " << graph->num_nodes() << std::endl;
    
    // Create a minimal configuration for loading annotations
    // Note: In a real application, you would use the Config class properly
    // This is a simplified example for demonstration
    std::cout << "\nLoading annotation from: " << annotation_path << std::endl;
    
    // Create a Config object with dummy command line args
    // We'll override the necessary fields manually
    char* dummy_argv[] = {argv[0], const_cast<char*>("query")};
    Config config(2, dummy_argv);
    
    // Set the required configuration parameters
    config.infbase = graph_path;
    config.infbase_annotators = {annotation_path};
    
    // Initialize the annotated graph
    auto anno_graph = initialize_annotated_dbg(graph, config);
    if (!anno_graph) {
        std::cerr << "Error: Failed to load annotations from " << annotation_path << std::endl;
        return 1;
    }
    
    std::cout << "Annotation loaded successfully" << std::endl;
    std::cout << "  Number of labels: " << anno_graph->get_annotator().num_labels() << std::endl;
    
    // Display some label names (first few)
    const auto& label_encoder = anno_graph->get_annotator().get_label_encoder();
    size_t num_labels_to_show = std::min(size_t(5), label_encoder.size());
    if (num_labels_to_show > 0) {
        std::cout << "  Sample labels:" << std::endl;
        for (size_t i = 1; i <= num_labels_to_show; ++i) {
            std::cout << "    - " << label_encoder.decode(i) << std::endl;
        }
    }
    
    // Now perform a simple query
    std::cout << "\nQuerying sequences from: " << query_path << std::endl;
    
    // Update config for querying
    config.fnames = {query_path};
    config.query_mode = QueryMode::LABELS;  // Just get labels
    config.discovery_fraction = 0.0;  // Require all k-mers to match
    config.num_top_labels = 0;  // Return all matching labels
    
    // Create a thread pool (required by QueryExecutor)
    ThreadPool thread_pool(0, 1000);  // 0 extra threads = single-threaded
    
    // Create query executor
    QueryExecutor executor(config, *anno_graph, nullptr, thread_pool);
    
    // Define a callback to handle query results
    size_t num_sequences = 0;
    auto query_callback = [&num_sequences, &anno_graph](const SeqSearchResult& result) {
        num_sequences++;
        const auto& seq = result.get_sequence();
        
        std::cout << "\nSequence: " << seq.name << std::endl;
        std::cout << "  Length: " << seq.sequence.length() << " bp" << std::endl;
        
        // Print results as string
        std::string result_str = result.to_string(
            "\t",    // delimiter
            false,   // suppress_unlabeled
            false,   // verbose_output
            *anno_graph
        );
        
        if (!result_str.empty()) {
            std::cout << "  Matches: " << result_str << std::endl;
        } else {
            std::cout << "  No matches found" << std::endl;
        }
    };
    
    // Execute the query
    try {
        executor.query_fasta(query_path, query_callback);
        std::cout << "\n" << num_sequences << " sequence(s) processed successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error during query: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\nExample completed successfully!" << std::endl;
    return 0;
}
