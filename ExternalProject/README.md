# MetaGraph External Project Example

This example demonstrates how to use MetaGraph via CMake's `ExternalProject_Add` instead of `add_subdirectory`. This approach automatically downloads and builds MetaGraph from its Git repository without requiring a submodule.

This is useful when you want to:

- Download MetaGraph from a Git repository automatically (no submodule needed)
- Build MetaGraph separately from your main project
- Have more control over MetaGraph's build configuration
- Isolate MetaGraph's build in a separate build tree
- Pin to specific MetaGraph versions via Git tags

## Differences from `add_subdirectory`

| Aspect | `add_subdirectory` | `ExternalProject_Add` |
|--------|-------------------|---------------------|
| **When built** | During main project configuration/build | As a separate external build step |
| **Include paths** | Automatically propagated | Must be specified manually |
| **Build isolation** | Shares build directory | Separate build directory |
| **CMake scope** | Variables/targets shared | Completely isolated |
| **Git integration** | Requires submodule | Fetches automatically from Git |
| **Setup** | Need to run `git submodule update` | Nothing - CMake downloads it |
| **Complexity** | Simpler for local projects | More complex but more flexible |

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
2. **Builds Separately**: MetaGraph is built in its own build tree (`build/metagraph-external/`)
3. **Creates Imported Targets**: Creates CMake imported library targets pointing to the built libraries
4. **Links Manually**: Your executable links against these imported targets
5. **Includes Manually**: Include directories must be explicitly specified

No submodule setup required - CMake handles the download automatically on first build!

## Using in Your Own Project

To use this approach in your own project, copy the `CMakeLists.txt` from this directory and customize:

### Specify the MetaGraph Version

```cmake
ExternalProject_Add(metagraph-external
    GIT_REPOSITORY https://github.com/ratschlab/metagraph.git
    GIT_TAG v1.0.0  # Pin to a specific version tag
    # or use a commit hash for exact reproducibility
    # GIT_TAG abc123def456...
    GIT_SHALLOW TRUE
    
    # ... rest of configuration
)
```

### Use a Local Copy (for development)

If you want to use a local checkout instead:

```cmake
ExternalProject_Add(metagraph-external
    SOURCE_DIR /path/to/local/metagraph
    # ... rest of configuration
)
```

## Advantages of ExternalProject_Add

1. **Automatic Download**: Can fetch MetaGraph from Git without manual submodule management
2. **Build Isolation**: MetaGraph's build doesn't interfere with your project's CMake variables
3. **Version Control**: Easy to pin to specific versions via Git tags
4. **Conditional Building**: More control over when MetaGraph rebuilds
5. **Multiple Configurations**: Can build MetaGraph with different settings than your project

## Disadvantages

1. **Complexity**: More verbose CMake configuration
2. **Manual Includes**: Include paths must be specified explicitly
3. **IDE Integration**: Some IDEs may not navigate to MetaGraph source as easily
4. **Build Time**: Always builds MetaGraph separately, even for incremental builds
5. **Debugging**: Harder to step through MetaGraph code during debugging

## When to Use Which Approach

### Use `add_subdirectory` when:
- You're actively developing both your project and MetaGraph
- You want seamless IDE integration
- Your project is tightly coupled with MetaGraph
- You prefer simpler CMake configuration

### Use `ExternalProject_Add` when:
- You want to download MetaGraph automatically from Git
- You need strict build isolation
- You're building a distributable package
- You want to pin to specific MetaGraph versions
- Your build system needs more control over dependencies

## Troubleshooting

### Libraries not found
If you get linking errors, ensure MetaGraph built successfully:
```bash
ls ExternalProject/build/metagraph-external/src/metagraph-build/lib/
```

You should see `libmetagraph_core.a` and `libmetagraph_cli.a`.

### Include errors
Check that the include paths in `ExternalProject/CMakeLists.txt` match the actual source structure:
```bash
ls ExternalProject/build/metagraph-external/src/metagraph/src/
```

### Rebuild MetaGraph
To force a rebuild of the external project:
```bash
cd ExternalProject/build
rm -rf metagraph-external
make -j$(nproc)
```

## See Also

- Main README: [README.md](README.md) - For the `add_subdirectory` approach
- MetaGraph Documentation: [metagraph/README.md](metagraph/README.md)
