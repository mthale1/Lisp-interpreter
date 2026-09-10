# S-Expression Interpreter
### Current Version: 1.2

## Description
This project implements an S-expression interpreter in C++. 

Project 1.2 adds support for `car`, `cdr`, `cons`, `quote`, and `eval`. The reader also supports the single-quote shorthand (') for quoted expressions.


## Build Instructions:

Compile the interpreter:
`g++ src/main.cpp src/sexpr.cpp -o sexpr`

Compile the unit tests:
`g++ tests/unit_test_sexpr.cpp src/sexpr.cpp -I src -o unit_tests`

## Run Instructions

Run the interpreter:
`./sexpr`

Run the interpreter using the provided test input:
`./sexpr < tests/test_input.txt`

Run the interpreter using the provided test input and save the output:
`./sexpr < tests/test_input.txt > tests/test_output.txt`

Run the unit tests:
`./unit_tests`

Run the unit tests and save the output:
`./unit_tests > tests/unit_test_output.txt`

## Testing

All testing files can be found in the `tests/` folder.

Unit-style tests are included in `unit_test_sexpr.cpp`. The tests check the individual interpreter functions and print PASS or FAIL for each test case.

`unit_test_output.txt` contains the output from the unit tests.

`test_input.txt` contains expressions used to test the complete interpreter.

`test_output.txt` contains the output produced from the provided test input.

## Known Deficiencies

Memory cleanup in `mainLoop()` is currently disabled because evaluated expressions may share nodes with the original expression. `freeExpr()` is implemented, but memory ownership will need to be handled more carefully before automatic cleanup can safely be enabled.

Invalid expressions and incorrect numbers of arguments are not currently checked.