# S-Expression Interpreter

**Name:** Makenzie Hale  
**Programming Language:** C++  
**Current Version:** 1.4

## Project Summary
This project implements a basic S-expression interpreter in C++.

The interpreter can read, evaluate, and print S-expressions. It supports `car`, `cdr`, `cons`, `quote`, and `eval`, along with the single-quote (`'`) shorthand for quoted expressions.

The interpreter includes a global environment for storing and looking up symbols. Values can be assigned using `set`, and redefining a symbol adds a new definition to the environment while lookup returns the most recent value.

The interpreter also supports the predicates `nil?`, `atom?`, `list?`, `not?`, and `number?`.

Version 1.4 adds `and?`, `or?`, and `eq?`, as well as `if` and `cond` for handling conditional expressions.


## Build Instructions

### Interpreter

Compile the interpreter:
```bash
g++ src/main.cpp src/sexpr.cpp -o sexpr
```

### Unit Tests

Compile the unit tests:
```bash
g++ tests/unit_test_sexpr.cpp src/sexpr.cpp -I src -o unit_tests
```

## Run Instructions

### Interpreter
Run the interpreter:
```bash
./sexpr
```

Run the interpreter using the provided test input:
```bash
./sexpr < tests/test_input.txt
```

Run the interpreter using the provided test input and save the output:
```bash
./sexpr < tests/test_input.txt > tests/test_output.txt
```

### Unit Tests

Run the unit tests:
```bash
./unit_tests
```

Run the unit tests and save the output:
```bash
./unit_tests > tests/unit_test_output.txt
```

## Testing

All testing files can be found in the `tests/` directory.

`unit_test_sexpr.cpp` - contains unit tests for the individual interpreter functions and reports PASS or FAIL for each test case.

`unit_test_output.txt` - contains the saved output from the unit tests.

`test_input.txt` - contains expressions used to test the complete interpreter.

`test_output.txt` - contains the output produced from the provided test input.

## Directory Structure
```text
lisp/
├── README.md
├── src/
│   ├── main.cpp
│   ├── sexpr.cpp
│   └── sexpr.h
└── tests/
    ├── unit_test_sexpr.cpp
    ├── unit_test_output.txt
    ├── test_input.txt
    └── test_output.txt
```

## Limitations
No known limitations for the current project requirements.