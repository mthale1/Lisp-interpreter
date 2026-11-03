# S-Expression Interpreter

**Name:** Makenzie Hale  
**Programming Language:** C++  

## Project Summary
This project implements a Lisp-style S-expression interpreter in C++. The interpreter can read, evaluate, and print S-expressions and supports quoting, list operations, predicates, Boolean operations,
conditionals, arithmetic operations, storing values using `set`, and user-defined functions.

The interpreter maintains global and local environments for storing and looking up symbols and their values. Local environments are used during function calls to bind arguments to parameters, while global values and function definitions remain available throughout the program.

User-defined functions can be created using function expressions or defined using `def`. Functions support evaluated arguments, local variables, nested function calls, and access to global values when needed.

### Syntax
The interpreter uses Lisp-style prefix notation, where the operation or function name comes before its arguments inside parentheses.

Examples:
```text
(add 2 3)
(mul 4 5)
(cons 'a '(b c))
(foo 2 3)
```

S-expressions can be quoted using `'` to prevent them from being evaluated.
```text
'(a b c)
'hello
```

Variables can be stored using `set`:
```text
(set x 10)
```

User-defined functions can be defined using `def`, followed by the function name, argument list, and body:
```text
(def foo (a b) (add a b))
(foo 2 3)
```

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

Compare the actual output with the expected output:

```bash
diff tests/expected_output.txt tests/test_output.txt
```
If there is no output from the command, the generated output matches the expected output.

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

`expected_output.txt` - contains the expected output for the expressions in `test_input.txt`.

## Directory Structure
```text
lisp/
├── README.md
├── src/
│   ├── main.cpp
│   ├── sexpr.cpp
│   └── sexpr.h
└── tests/
    ├── expected_output.txt
    ├── test_input.txt
    ├── test_output.txt
    ├── unit_test_output.txt
    └── unit_test_sexpr.cpp
```

## Limitations
No known limitations for the current project requirements.