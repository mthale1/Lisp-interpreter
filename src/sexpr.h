#ifndef SEXPR_H
#define SEXPR_H

#include <string>

enum class Type {
    ATOM,
    CELL,
    NIL
};

struct SExpr {
    Type type;
    std::string atom;
    SExpr* car = nullptr;
    SExpr* cdr = nullptr;
};

extern SExpr* rho;

void skipWhitespace();

// S-expression creation and checking
SExpr* makeAtom(std::string value);
SExpr* makeNil();
bool isAtom(SExpr* expr);
bool isNil(SExpr* expr);
bool isNumber(SExpr* expr);
int symbolToInt(SExpr* expr);
SExpr* intToSymbol(int value);

// S-expression operations
SExpr* car(SExpr* expr);
SExpr* cdr(SExpr* expr);
SExpr* cons(SExpr* first, SExpr* second);
SExpr* quote(SExpr* expr);

// Environment and evaluation
SExpr* makePair(SExpr* name, SExpr* value);
SExpr* lookup(SExpr* symbol);
SExpr* eval(SExpr* expr);

// Reading
SExpr* readAtom();
SExpr* readExpr();
SExpr* readList();

// Printing
void printList(SExpr* expr);
void printExpr(SExpr* expr);

// Program Loop
void mainLoop();



#endif