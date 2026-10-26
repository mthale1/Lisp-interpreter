# S-Expression Interpreter

**Name:** Makenzie Hale  
**Programming Language:** C++  
**Current Version:** 1.7

## Project Summary
This project implements a basic S-expression interpreter in C++. It can read, evaluate, and print S-expressions and supports list operations, predicates, Boolean operations,
conditionals, math operations, and storing values using `set`.

The interpreter maintains global and local environments for storing and looking up symbols and their values. It also supports function expressions, which can be stored using `set`.

Version 1.7 adds support for calling the user-defines functions by evaluating their arguments in a local environment. Local environments are pushed onto a stack during function calls 
and removed when the function returns, allowing functions to call other user-defined functions.


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

`unit_test_sexpr.cpp` - contains unit tests for the individual interpreter functions, reports PASS or FAIL for each test case, and provides a test report with the total number of passed and failed tests.

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