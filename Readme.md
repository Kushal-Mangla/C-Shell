# C-Shell

A custom UNIX shell implementation written in C. This shell provides common functionalities similar to bash with additional custom commands.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [File Structure](#file-structure)
- [Installation](#installation)
- [Usage](#usage)
- [Custom Commands](#custom-commands)
- [Assumptions](#assumptions)

## Overview

C-Shell is a custom shell implementation that mimics the behavior of standard UNIX shells while providing additional functionality. The shell supports command execution, directory navigation, file and directory search, process management, I/O redirection, and more.

## Features

- **Custom prompt** with username, system name, and current directory
- **Command execution** for both built-in commands and system commands
- **Background process** execution with the `&` operator
- **Multiple command** execution using `;` separator
- **I/O redirection** with `>`, `>>`, and `<` operators
- **Custom commands**:
  - `hop` - Directory navigation similar to `cd`
  - `reveal` - List files and directories with various options
  - `seek` - Search for files and directories
  - `proclore` - Display process information
  - `log` - Command history management

## File Structure

```
C-Shell/
├── Makefile               # Compilation instructions
├── Readme.md              # Project documentation
├── src_code/              # Source code directory
    ├── globals.h          # Global variables and common functions
    ├── globals.c          # Implementation of global variables
    ├── hop.h              # Directory navigation functionality
    ├── hop.c              # Implementation of hop command
    ├── input.h            # Input handling functionality
    ├── input.c            # Input handling implementation
    ├── log.h              # Command history functionality
    ├── log.c              # Implementation of log commands
    ├── main.c             # Main shell loop and command execution
    ├── proclore.h         # Process information functionality
    ├── proclore.c         # Implementation of proclore command
    ├── print.h            # Display utilities
    ├── print.c            # Implementation of display utilities
    ├── pwd.h              # Working directory functionality
    ├── reveal.h           # Directory listing functionality
    ├── reveal.c           # Implementation of reveal command
    ├── seek.h             # File search functionality
    ├── seek.c             # Implementation of seek command
    ├── command_log.txt    # File to store command history
    └── process.txt        # File to track background processes
```

## Installation

### Prerequisites
- GCC Compiler
- Make

### Building the Shell
1. Clone the repository:
```
git clone <repository-url>
cd C-Shell
```

2. Compile and run the shell:
```
make
```

This will compile all source files and launch the shell.

## Usage

The shell provides a prompt in the format `<username@system_name:current_directory>` where:
- `username` is your system username
- `system_name` is your computer's hostname
- `current_directory` shows your current location (with `~` representing home directory)

### Basic Command Syntax
```
<command> [arguments]
```

### Multiple Commands
Use semicolons to execute multiple commands:
```
<command1>; <command2>; <command3>
```

### Background Execution
Use `&` to run a command in the background:
```
<command> &
```

## Custom Commands

### hop
Change directories similar to the `cd` command.

```
hop [directory]
```

Options:
- `hop` without arguments: Change to the home directory
- `hop ~`: Change to the home directory
- `hop -`: Change to the previous directory
- `hop ..`: Change to the parent directory
- `hop <path>`: Change to the specified directory

### reveal
List files and directories (similar to `ls`).

```
reveal [options] [directory]
```

Options:
- `-a`: Show all files including hidden files
- `-l`: Show detailed information
- `-la` or `-al`: Combine both options

### seek
Search for files and directories.

```
seek [options] <search_term> [directory]
```

Options:
- `-d`: Only search for directories
- `-f`: Only search for files
- `-e`: If only one file/directory is found, execute/change to it

### proclore
Display process information.

```
proclore [pid]
```

If no PID is provided, shows information about the shell process.

### log
Command history management.

```
log
log purge
log execute <index>
```

Options:
- `log`: Display command history
- `log purge`: Clear command history
- `log execute <index>`: Execute a command from history by index

## Assumptions

1. Background process success messages are printed after another user input is entered
2. All background processes are assumed to be correct
3. "+" signifies a foreground process in status output
4. All commands are stored in log regardless of validity
5. Commands identical to the previous command will not be stored in history
6. Commands like `log`, `log purge`, `log execute <index>` are not stored in history

## License

This project is licensed under the MIT License - see the LICENSE file for details.