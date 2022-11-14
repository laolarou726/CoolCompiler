# CoolCompiler [![CodeQL](https://github.com/laolarou726/CoolCompiler/actions/workflows/codeql.yml/badge.svg)](https://github.com/laolarou726/CoolCompiler/actions/workflows/codeql.yml)

Stanford CS1/CS143 Compilers Course Project - Cool compiler with LLVM backend

## Development Status

|Component|Status|
|:---------------:|:-------:|
|  Lexer          |  ✅     |
|  Parser         |  ✅     |
|Semantic Analysis|  ✅     |
|   Optimization  | Skipped |
|Code Generation  | WIP |

## Setup

### Tested Environment

- Ubuntu: 22.04
- LLVM: 15.0.2
- Zlib: 1.2.11

### Install Zlib

```bash

sudo apt-get install zlib1g-dev

```

### Install LLVM

```bash

sudo bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"

```
