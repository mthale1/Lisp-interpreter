#include "sexpr.h"

#include <iostream>
#include <string>
#include <cctype>

using namespace std;


void skipWhitespace(){
// Ignore whitespace characters in the input.

    while (cin && isspace(cin.peek()))
    {
        cin.get();
    }
}

SExpr* makeAtom(string value){
// Creates & returns a new atom S-expression.

    SExpr* expr = new SExpr;

    expr->type = Type::ATOM;
    expr->atom = value;
    expr->car = nullptr;
    expr->cdr = nullptr;

    return expr;
}

SExpr* makeNil(){
// Creates & returns a new NIL S-expression.

    SExpr* expr = new SExpr;

    expr->type = Type::NIL;
    expr->car = nullptr;
    expr->cdr = nullptr;

    return expr;
}

bool isAtom(SExpr* expr){
    return expr->type == Type::ATOM;
}

bool isNil(SExpr* expr){
    return expr->type == Type::NIL;
}

SExpr* car(SExpr* expr){
    return expr->car;
}

SExpr* cdr(SExpr* expr){
    return expr->cdr;
}

SExpr* cons(SExpr* first, SExpr* second){
// Creates & returns a new cell containing a car & cdr.

    SExpr* expr = new SExpr;

    expr->type = Type::CELL;
    expr->car = first;
    expr->cdr = second;

    return expr;
}

SExpr* quote(SExpr* expr){
// Simply return its argument to avoid evaluating an s-expression.
    return expr;
}

SExpr* eval(SExpr* expr){
// Evaluates an S-expression and returns the result

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

SExpr* readAtom(){
// Reads a symbol from input & returns it as an atom.

    string symbol;

    // while the next character is not whitespace or parentheses, add the next character to `symbol`.
    while (cin && !isspace(cin.peek()) && cin.peek() != '(' && cin.peek() != ')'){
        symbol += cin.get();
    }

    return makeAtom(symbol);
}

SExpr* readExpr();
SExpr* readList();

SExpr* readList(){
// Reads and constructs a list from the input.

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

SExpr* readExpr(){
// Reads and returns the next S-expression from input.

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

void printExpr(SExpr* expr){
// Prints an S-expression.

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

void printList(SExpr* expr){
// Prints the contents of a list.

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

void freeExpr(SExpr* expr){
// Frees memory used by an S-expression.

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

void mainLoop(){
// Repeatedly reads and prints S-expressions until EOF (End-of-File).

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
