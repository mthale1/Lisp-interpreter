#include "sexpr.h"

#include <iostream>
#include <string>
#include <cctype>

using namespace std;

SExpr* rho = makeNil();
SExpr* local = makeNil();

SExpr* evalList(SExpr* list){
    if (isNil(list)){
        return makeNil();
    }

    return cons(eval(car(list)), evalList(cdr(list)));
}

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
        
        SExpr* expr = readExpr();
        SExpr* atom = makeAtom("quote");
        SExpr* list = cons(expr, makeNil());

        return cons(atom, list);
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

// Searches local variables first, then rho, for a symbol and returns its assigned value.
SExpr* lookup(SExpr* symbol) {
    if (!isNil(local)) {
        SExpr* env = car(local);
        SExpr* names = car(env);
        SExpr* vals = car(cdr(env));

        while (!isNil(names)){
            SExpr* name = car(names);

            if (name->atom == symbol->atom){
                return car(vals);
            }

            names = cdr(names);
            vals = cdr(vals);
        }
    }

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

        // SET
        if (symbol == "set") {
            SExpr* name = car(cdr(expr));
            SExpr* value = car(cdr(cdr(expr)));

            SExpr* evaluated = eval(value);

            rho = cons(makePair(name, evaluated), rho);

            return evaluated;
        }
        // PREDICATES
        else if (symbol == "nil?" || symbol == "not?"){
            SExpr* arg = car(cdr(expr));
            SExpr* evaluated = eval(arg);

            if (isNil(evaluated)){
                return makeAtom("T");
            }

            return makeNil();
        }
        else if (symbol == "atom?"){
            SExpr* arg = car(cdr(expr));
            SExpr* evaluated = eval(arg);

            if (isAtom(evaluated)){
                return makeAtom("T");
            }

            return makeNil();
        }
        else if (symbol == "list?"){
            SExpr* arg = car(cdr(expr));
            SExpr* evaluated = eval(arg);

            if (evaluated->type == Type::CELL){
                return makeAtom("T");
            }

            return makeNil();
        }
        else if (symbol == "number?"){
            SExpr* arg = car(cdr(expr));
            SExpr* evaluated = eval(arg);

            if (isNumber(evaluated)){
                return makeAtom("T");
            }

            return makeNil();
        }
        // BOOLEANS
        else if (symbol == "and?"){
            SExpr* arg1 = car(cdr(expr));
            SExpr* arg2 = car(cdr(cdr(expr)));

            SExpr* evaluatedArg1 = eval(arg1);

            if (isNil(evaluatedArg1)){
                return makeNil();
            }

            return eval(arg2);
        }
        else if (symbol == "or?"){
            SExpr* arg1 = car(cdr(expr));
            SExpr* arg2 = car(cdr(cdr(expr)));
            SExpr* evaluatedArg1 = eval(arg1);

            if (!isNil(evaluatedArg1)){
                return evaluatedArg1;
            }

            return eval(arg2);
        }
        else if (symbol == "eq?"){
            SExpr* arg1 = eval(car(cdr(expr)));
            SExpr* arg2 = eval(car(cdr(cdr(expr))));

            if (!isAtom(arg1) || !isAtom(arg2)){
                return makeNil();
            }

            if (arg1->atom == arg2->atom){
                return makeAtom("T");
            }

            return makeNil();
        }
        // CONDITIONALS
        else if (symbol == "if"){
            SExpr* condition = car(cdr(expr));
            SExpr* trueBranch = car(cdr(cdr(expr)));
            SExpr* falseBranch = car(cdr(cdr(cdr(expr))));

            SExpr* evaluated = eval(condition);

            if (isNil(evaluated)){
                return eval(falseBranch);
            }

            return eval(trueBranch);
        }
        else if (symbol == "cond"){
            SExpr* conditions = car(cdr(expr));

            while (!isNil(conditions)){
                SExpr* condition = car(conditions);
                SExpr* res = car(cdr(conditions));

                if (!isNil(eval(condition))){
                    return eval(res);
                }

                conditions = cdr(cdr(conditions));
            }

            return makeNil();
        }
        // LIST OPERATIONS
        else if (symbol == "car"){
            SExpr* arg = car(cdr(expr));
            SExpr* evaluated = eval(arg);

            return car(evaluated);
        }
        else if (symbol == "cdr"){
            SExpr* arg = car(cdr(expr));
            SExpr* evaluated = eval(arg);

            return cdr(evaluated);
        }
        else if (symbol == "cons"){
            SExpr* arg1 = car(cdr(expr));
            SExpr* arg2 = car(cdr(cdr(expr)));

            SExpr* evaluatedArg1 = eval(arg1);
            SExpr* evaluatedArg2 = eval(arg2);

            return cons(evaluatedArg1, evaluatedArg2);
        }
        // MATH
        else if (symbol == "add"){
            SExpr* arg1 = eval(car(cdr(expr)));
            SExpr* arg2 = eval(car(cdr(cdr(expr))));

            int val1 = symbolToInt(arg1);
            int val2 = symbolToInt(arg2);

            return intToSymbol(val1 + val2);
        }
        else if (symbol == "sub"){
            SExpr* arg1 = eval(car(cdr(expr)));
            SExpr* arg2 = eval(car(cdr(cdr(expr))));

            int val1 = symbolToInt(arg1);
            int val2 = symbolToInt(arg2);

            return intToSymbol(val1 - val2);
        }
        else if (symbol == "mul"){
            SExpr* arg1 = eval(car(cdr(expr)));
            SExpr* arg2 = eval(car(cdr(cdr(expr))));

            int val1 = symbolToInt(arg1);
            int val2 = symbolToInt(arg2);

            return intToSymbol(val1 * val2);
        }
        else if (symbol == "div"){
            SExpr* arg1 = eval(car(cdr(expr)));
            SExpr* arg2 = eval(car(cdr(cdr(expr))));

            int val1 = symbolToInt(arg1);
            int val2 = symbolToInt(arg2);

            return intToSymbol(val1 / val2);
        }
        else if (symbol == "rem"){
            SExpr* arg1 = eval(car(cdr(expr)));
            SExpr* arg2 = eval(car(cdr(cdr(expr))));

            int val1 = symbolToInt(arg1);
            int val2 = symbolToInt(arg2);

            return intToSymbol(val1 % val2);
        }
        else if (symbol == "lt"){
            SExpr* arg1 = eval(car(cdr(expr)));
            SExpr* arg2 = eval(car(cdr(cdr(expr))));

            int val1 = symbolToInt(arg1);
            int val2 = symbolToInt(arg2);

            if (val1 < val2){
                return makeAtom("T");
            }

            return makeNil();
        }
        // others
        else if (symbol == "function"){
            return expr;
        }
        else if (symbol == "quote"){
            SExpr* arg = car(cdr(expr));
            return quote(arg);
        }
        else if (symbol == "eval"){
            SExpr* arg = car(cdr(expr));
            SExpr* evaluated = eval(arg);

            return eval(evaluated);
        }

        else {
            SExpr* function = lookup(car(expr));

            if (!isAtom(function) && !isNil(function) &&
                isAtom(car(function)) && car(function)->atom == "function") {

                SExpr* args = car(cdr(function));
                SExpr* body = car(cdr(cdr(function)));

                SExpr* vals = evalList(cdr(expr));

                SExpr* env = cons(args, cons(vals, makeNil()));
                local = cons(env, local);

                SExpr* res = eval(body);

                local = cdr(local);

                return res;

            }
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
            SExpr* res = eval(expr);

            printExpr(res);
            cout << endl;

        }

}
