
# Playground 

Testing ground for c projects

## Features

- Modular C source organization
- Automatic compilation of `.c` files
- Separate directories for source files, headers, build artifacts, and executables
- Configurable compiler flags
- Memory limit testing using `ulimit`
- Optional restrictions on dynamic memory allocation
- Simple Makefile workflow

## Project Structure

```text
.
├── bin/                 # Compiled executables
├── build/               # Object files (.o)
├── include/             # Project header files
├── libs/                # Library and configuration headers
│   └── constraints.h    # Resource constraint definitions
├── src/                 # C source files
│   ├── main.c
│   └── ...
├── constraints.sh       # Memory constraint testing script
├── Makefile
├── LICENSE
└── README.md
```

## Requirements

- GCC
- GNU Make
- Bash
- Linux or an environment supporting `ulimit -v`

## Build

Compile the project:

```bash
make
```

The executable will be generated at:

```text
bin/main
```

## Usage

Run the compiled program:

```bash
make run
```

Run with a memory constraint:

```bash
make test
```

By default, `make test` runs the executable with a 64 KB virtual memory limit.

You can also specify a custom memory limit in bytes:

```bash
./constraints.sh ./bin/main 131072
```

This example sets a 128 KB virtual memory limit.

If no memory limit is specified, `constraints.sh` defaults to 1 MB.

## Resource Constraints

Resource definitions are located in `libs/constraints.h`.

```c
#define MAX_MEMORY (64 * 1024)
#define MAX_BUFFER (4096)

#define NO_MALLOC 0
```

| Macro | Default | Description |
|---|---:|---|
| `MAX_MEMORY` | 64 KB | Intended application memory budget |
| `MAX_BUFFER` | 4096 bytes | Maximum buffer size |
| `NO_MALLOC` | 0 | Configuration flag for dynamic allocation restrictions |

Include the header in your source files:

```c
#include "constraints.h"
```

The Makefile includes the `libs/` directory in its header search path.

**Note:** These macros do not automatically enforce memory limits. `MAX_MEMORY` is a configuration constant, and `NO_MALLOC` requires implementation in the application. The shell script independently applies a virtual memory limit to the process.

## Clean

Remove generated object files and the executable:

```bash
make clean
```

## Compiler Configuration

The default compiler flags are:

```makefile
-Wall -Wextra -std=c11 -O2
```

You can modify these in the Makefile.

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

