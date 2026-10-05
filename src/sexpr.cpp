#include "sexpr.h"

#include <iostream>
#include <string>
#include <cctype>

using namespace std;

SExpr* rho = makeNil();

// Ignore whitespace characters in the input.
void skipWhitespace(){

    while (cin && isspace(cin.peek()))
    {
        cin.get();
    }
}

// Creates & returns a new atom S-expression.
SExpr* makeAtom(string value){

    SExpr* expr = new SExpr;

    expr->type = Type::ATOM;
    expr->atom = value;
    expr->car = nullptr;
    expr->cdr = nullptr;

    return expr;
}

// Creates & returns a new NIL S-expression.
SExpr* makeNil(){

    SExpr* expr = new SExpr;

    expr->type = Type::NIL;
    expr->car = nullptr;
    expr->cdr = nullptr;

    return expr;
}

// Returns true if the S-expression is an atom.
bool isAtom(SExpr* expr){
    return expr->type == Type::ATOM;
}

// Returns true if the S-expression is NIL.
bool isNil(SExpr* expr){
    return expr->type == Type::NIL;
}

// Returns true if the S-expression is an atom representing a valid integer.
bool isNumber(SExpr* expr){

    if (!isAtom(expr)){
        return false;
    }

    string value = expr->atom;

    if (value.empty()){
        return false;
    }

    int start = 0;

    if (value[0] == '-'){
        if (value.length() == 1){
            return false;
        }

        start = 1;
    }

    for (int i = start; i < value.length(); i++){
        if (!isdigit(value[i])){
            return false;
        }
    }

    return true;
}

// Converts an atom representing an integer into a C++ integer.
int symbolToInt(SExpr* expr){

    return stoi(expr->atom);
}

// Converts a C++ integer into an atom S-expression.
SExpr* intToSymbol(int value){

    return makeAtom(to_string(value));
}

// Returns the first element of a cell.
SExpr* car(SExpr* expr){
    return expr->car;
}

// Returns the remainder of a cell.
SExpr* cdr(SExpr* expr){
    return expr->cdr;
}

// Creates & returns a cell using first as the car and second as the cdr.
SExpr* cons(SExpr* first, SExpr* second){

    SExpr* expr = new SExpr;

    expr->type = Type::CELL;
    expr->car = first;
    expr->cdr = second;

    return expr;
}

// Simply return its argument to avoid evaluating an s-expression.
SExpr* quote(SExpr* expr){
    return expr;
}

// Reads a symbol from input & returns it as an atom.
SExpr* readAtom(){

    string symbol;

    // while the next character is not EOF, whitespace, or parentheses, add it to `symbol`.
    while (cin.peek() != EOF && !isspace(cin.peek()) && cin.peek() != '(' && cin.peek() != ')'){
        symbol += cin.get();
    }

    return makeAtom(symbol);
}

SExpr* readExpr();
SExpr* readList();

// Reads and constructs a list from the input.
SExpr* readList(){

    skipWhitespace();

    // List ends, return NIL.
    if (cin.peek() == ')'){
        cin.get();
        return makeNil();
    }

    SExpr* first = readExpr();
    SExpr* rest = readList();

    return cons(first, rest);
}

// Reads and returns the next S-expression from input.
SExpr* readExpr(){

    skipWhitespace();

    if (cin.peek() == '('){
        cin.get();
        return readList();
    }
    else if (cin.peek() == '\'') {
        cin.get();
        
        SExpr* quotedExpr = readExpr();
        SExpr* quoteAtom = makeAtom("quote");
        SExpr* quotedList= cons(quotedExpr, makeNil());

        return cons(quoteAtom, quotedList);
    }
    else {
        return readAtom();
    }
}

void printList(SExpr* expr);

// Prints an S-expression.
void printExpr(SExpr* expr){

    if (expr->type == Type::ATOM){
        cout << expr->atom;
    }
    else if (expr->type == Type::NIL){
        cout << "()";
    }
    else if (expr->type == Type::CELL){
        cout << "(";
        printList(expr);
        cout << ")";
    }
}

// Prints the contents of a list, including dotted notation.
void printList(SExpr* expr){

    if (expr->type == Type::NIL){
        return;
    }

    printExpr(expr->car);

    if (isNil(expr->cdr)){
        return;
    } 
    else if (expr->cdr->type == Type::CELL){
        cout << " ";
        printList(expr->cdr);
    }
    else {
        cout << " . ";
        printExpr(expr->cdr);
    }

}

// Creates & returns a list containing a name and its value.
SExpr* makePair(SExpr* name, SExpr* value) {
    return cons(name, cons(value, makeNil()));
}

// Searches rho for a symbol and returns its assigned value.
SExpr* lookup(SExpr* symbol) {
    SExpr* current = rho;

    while (!isNil(current)){
        SExpr* pair = car(current);
        SExpr* name = car(pair);

        if (name->atom == symbol->atom){
            return car(cdr(pair));
        }

        current = cdr(current);
    }

    return symbol;
}

// Evaluates an S-expression by recognizing and executing supported operations, assignments, and predicates.
SExpr* eval(SExpr* expr){

    if (isAtom(expr)){
        return lookup(expr);
    }

    if (isNil(expr)){
        return expr;
    }

    if (isAtom(car(expr))) {
        string symbol = car(expr)->atom;

        if (symbol == "set") {
            SExpr* name = car(cdr(expr));
            SExpr* value = car(cdr(cdr(expr)));

            SExpr* evaluatedValue = eval(value);

            rho = cons(makePair(name, evaluatedValue), rho);

            return evaluatedValue;
        }
        else if (symbol == "nil?" || symbol == "not?"){
            SExpr* argument = car(cdr(expr));
            SExpr* evaluatedArgument = eval(argument);

            if (isNil(evaluatedArgument)){
                return makeAtom("T");
            }

            return makeNil();
        }
        else if (symbol == "atom?"){
            SExpr* argument = car(cdr(expr));
            SExpr* evaluatedArgument = eval(argument);

            if (isAtom(evaluatedArgument)){
                return makeAtom("T");
            }

            return makeNil();
        }
        else if (symbol == "list?"){
            SExpr* argument = car(cdr(expr));
            SExpr* evaluatedArgument = eval(argument);

            if (evaluatedArgument->type == Type::CELL){
                return makeAtom("T");
            }

            return makeNil();
        }
        else if (symbol == "number?"){
            SExpr* argument = car(cdr(expr));
            SExpr* evaluatedArgument = eval(argument);

            if (isNumber(evaluatedArgument)){
                return makeAtom("T");
            }

            return makeNil();
        }
        else if (symbol == "car"){
            SExpr* argument = car(cdr(expr));
            SExpr* evaluatedArgument = eval(argument);

            return car(evaluatedArgument);
        }
        else if (symbol == "cdr"){
            SExpr* argument = car(cdr(expr));
            SExpr* evaluatedArgument = eval(argument);

            return cdr(evaluatedArgument);
        }
        else if (symbol == "cons"){
            SExpr* argument1 = car(cdr(expr));
            SExpr* argument2 = car(cdr(cdr(expr)));

            SExpr* evaluatedArgument1 = eval(argument1);
            SExpr* evaluatedArgument2 = eval(argument2);

            return cons(evaluatedArgument1, evaluatedArgument2);
        }
        else if (symbol == "quote"){
            SExpr* argument = car(cdr(expr));
            return quote(argument);
        }
        else if (symbol == "eval"){
            SExpr* argument = car(cdr(expr));
            SExpr* evaluatedArgument = eval(argument);

            return eval(evaluatedArgument);
        }
    }
    
    return expr;
}

// Repeatedly reads and prints S-expressions until EOF (End-of-File).
void mainLoop(){

    while (cin)
        {
            skipWhitespace();

            if (cin.peek() == EOF)
            {
                break;
            }

            SExpr* expr = readExpr();
            SExpr* result = eval(expr);

            printExpr(result);
            cout << endl;

        }

}
