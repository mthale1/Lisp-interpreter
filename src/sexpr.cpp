#include "sexpr.h"

#include <iostream>
#include <string>
#include <cctype>

using namespace std;

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

// Evaluates an S-expression by recognizing and executing
// supported operations such as car, cdr, cons, quote, and eval.
SExpr* eval(SExpr* expr){

    if (isAtom(expr)){
        return expr;
    }

    if (isNil(expr)){
        return expr;
    }

    if (isAtom(car(expr))) {
        string symbol = car(expr)->atom;

        if (symbol == "car"){
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

// Reads a symbol from input & returns it as an atom.
SExpr* readAtom(){

    string symbol;

    // while the next character is not whitespace or parentheses, add the next character to `symbol`.
    while (cin && !isspace(cin.peek()) && cin.peek() != '(' && cin.peek() != ')'){
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
        
        SExpr* quotedExpr = readExpr(); // read expression after the '
        SExpr* quoteAtom = makeAtom("quote"); // create quote
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

// Frees memory used by an S-expression.
void freeExpr(SExpr* expr){

    if (expr == nullptr)
    {
        return;
    }

    if (expr->type == Type::CELL)
    {
        freeExpr(expr->car);
        freeExpr(expr->cdr);
    }

    delete expr;
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

            // freeExpr(expr);
        }

}
