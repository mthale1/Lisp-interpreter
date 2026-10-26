#include "sexpr.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

struct TestFailure {
    string testFunction;
    string test;
    int line;
};

int testsPassed = 0;
int testsFailed = 0;
vector<TestFailure> failedTests;
string currentTestFunction;

// Unit test helper functions
void check(bool condition, string test, string actual, string expected, int line){

    if (condition){
        cout << "\n✅ PASS \t";
        testsPassed++;
    }
    else {
        cout << "\n❌ FAIL \t";
        testsFailed++;
        failedTests.push_back({currentTestFunction, test, line});
    }

    cout << test << " | Actual: " << actual << " | Expected: " << expected << endl;
}

#define check(condition, test, actual, expected) check(condition, test, actual, expected, __LINE__)

SExpr* readString(string input){
    istringstream testInput(input);
    streambuf* originalCin = cin.rdbuf(testInput.rdbuf());

    cin.clear();
    SExpr* expr = readExpr();

    cin.rdbuf(originalCin);
    cin.clear();

    return expr;
}

string exprToString(SExpr* expr) {
    ostringstream output;
    streambuf* originalCout = cout.rdbuf(output.rdbuf());

    printExpr(expr);

    cout.rdbuf(originalCout);

    return output.str();
}

bool sameContents(SExpr* first, SExpr* second){

    if (first->type == Type::ATOM && second->type == Type::ATOM){
        return first->atom == second->atom;
    }

    if (first->type == Type::NIL && second->type == Type::NIL){
        return true;
    }

    if (first->type == Type::CELL && second->type == Type::CELL){
        return sameContents(first->car, second->car) && sameContents(first->cdr, second->cdr);
    }

    return false;
}

void checkReadOutput(SExpr* (*readFunction)(), string input, SExpr* expected, string test){
    istringstream testInput(input);
    streambuf* originalCin = cin.rdbuf(testInput.rdbuf());

    cin.clear();
    SExpr* res = readFunction();

    cin.rdbuf(originalCin);
    cin.clear();

    cout << "\n\n🔎 Test: " << test << endl;

    string actualType = res->type == Type::NIL ? "NIL" : res->type == Type::CELL ? "CELL" : "ATOM";
    string expectedType = expected->type == Type::NIL ? "NIL" : expected->type == Type::CELL ? "CELL" : "ATOM";

    check(res->type == expected->type, "Creates the correct type.", actualType, expectedType);
    check(sameContents(res, expected), "Stores the correct contents.", exprToString(res), exprToString(expected));
}

void checkPrintOutput(void (*printFunction)(SExpr*), SExpr* expr, string expected, string test){
    ostringstream output;
    streambuf* originalCout = cout.rdbuf(output.rdbuf());

    printFunction(expr);

    cout.rdbuf(originalCout);

    check(output.str() == expected, test, output.str(), expected);
}

// readExpr() is tested separately. This helper uses readExpr() to make other tests easier to read by allowing 
// complete Lisp expressions to be evaluated directly from strings instead of manually building them with cons().
SExpr* evalString(string input){
    istringstream testInput(input);
    streambuf* originalCin = cin.rdbuf(testInput.rdbuf());

    cin.clear();
    SExpr* expr = readExpr();

    cin.rdbuf(originalCin);
    cin.clear();

    return eval(expr);
}

void testMakeAtom(){
    currentTestFunction = "TEST MAKE ATOM";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "--------------------- TEST MAKE ATOM ----------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: makeAtom should create an atom with the correct type and value." << endl;

    SExpr* res;

    res = makeAtom("string");
    check(res->type == Type::ATOM, "Creates an S-expression with the correct type: makeAtom(\"string\")", res->type == Type::ATOM ? "ATOM" : "NOT ATOM", "ATOM");
    check(res->atom == "string", "Stores the correct atom value: makeAtom(\"string\")", res->atom, "string");

    res = makeAtom("123");
    check(res->atom == "123", "Stores numbers as atom text: makeAtom(\"123\")", res->atom, "123");

    res = makeAtom("+-=");
    check(res->atom == "+-=", "Stores symbols as atom text: makeAtom(\"+-=\")", res->atom, "+-=");
}

void testMakeNil(){
    currentTestFunction = "TEST MAKE NIL";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "--------------------- TEST MAKE NIL -----------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: makeNil should create an S-expression with type NIL." << endl;

    SExpr* res = makeNil();

    check(res->type == Type::NIL, "Creates an S-expression with the correct type: makeNil()", res->type == Type::NIL ? "NIL" : "NOT NIL", "NIL");
}

void testIsAtom(){
    currentTestFunction = "TEST IS ATOM";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "---------------------- TEST IS ATOM -----------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: isAtom should return true only when the S-expression is an atom." << endl;

    SExpr* atom = makeAtom("atom");
    SExpr* nil = makeNil();
    SExpr* cell = cons(atom, nil);

    check(isAtom(atom), "Returns true for an atom: isAtom(atom)", isAtom(atom) ? "true" : "false", "true");
    check(!isAtom(nil), "Returns false for (): isAtom(nil)", isAtom(nil) ? "true" : "false", "false");
    check(!isAtom(cell), "Returns false for a cell: isAtom(cell)", isAtom(cell) ? "true" : "false", "false");
}

void testIsNil(){
    currentTestFunction = "TEST IS NIL";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "---------------------- TEST IS NIL ------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: isNil should return true only when the S-expression is NIL." << endl;

    SExpr* atom = makeAtom("atom");
    SExpr* nil = makeNil();
    SExpr* cell = cons(atom, nil);

    check(isNil(nil), "Returns true for (): isNil(nil)", isNil(nil) ? "true" : "false", "true");
    check(!isNil(atom), "Returns false for an atom: isNil(atom)", isNil(atom) ? "true" : "false", "false");
    check(!isNil(cell), "Returns false for a cell: isNil(cell)", isNil(cell) ? "true" : "false", "false");
}

void testIsNumber(){
    currentTestFunction = "TEST IS NUMBER";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "--------------------- TEST IS NUMBER ----------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: isNumber should return true only when the S-expression is an atom representing a valid integer." << endl;

    SExpr* input;

    input = makeAtom("123");
    check(isNumber(input), "Returns true for a positive integer: isNumber(123)", isNumber(input) ? "true" : "false", "true");

    input = makeAtom("-42");
    check(isNumber(input), "Returns true for a negative integer: isNumber(-42)", isNumber(input) ? "true" : "false", "true");

    input = makeAtom("abc");
    check(!isNumber(input), "Returns false for letters: isNumber(abc)", isNumber(input) ? "true" : "false", "false");

    input = makeAtom("12a");
    check(!isNumber(input), "Returns false for mixed characters: isNumber(12a)", isNumber(input) ? "true" : "false", "false");

    input = makeAtom("-");
    check(!isNumber(input), "Returns false for a minus sign alone: isNumber(-)", isNumber(input) ? "true" : "false", "false");

    input = makeNil();
    check(!isNumber(input), "Returns false for (): isNumber(nil)", isNumber(input) ? "true" : "false", "false");
}

void testSymbolToInt(){
    currentTestFunction = "TEST SYMBOL TO INT";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------ TEST SYMBOL TO INT ---------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: symbolToInt should convert an atom representing an integer into a C++ integer." << endl;

    check(symbolToInt(makeAtom("123")) == 123, "Converts a positive numeric atom to an integer: symbolToInt(123)",to_string(symbolToInt(makeAtom("123"))), "123");
    check(symbolToInt(makeAtom("-42")) == -42, "Converts a negative numeric atom to an integer: symbolToInt(-42)", to_string(symbolToInt(makeAtom("-42"))), "-42");
    check(symbolToInt(makeAtom("0")) == 0, "Converts zero to an integer: symbolToInt(0)", to_string(symbolToInt(makeAtom("0"))), "0");
}

void testIntToSymbol(){
    currentTestFunction = "TEST INT TO SYMBOL";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------ TEST INT TO SYMBOL ---------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: intToSymbol should convert a C++ integer into an atom S-expression." << endl;

    SExpr* res;

    res = intToSymbol(123);
    check(res->type == Type::ATOM && res->atom == "123", "Converts a positive integer to a numeric atom: intToSymbol(123)", exprToString(res), "123");

    res = intToSymbol(-42);
    check(res->type == Type::ATOM && res->atom == "-42", "Converts a negative integer to a numeric atom: intToSymbol(-42)", exprToString(res), "-42");

    res = intToSymbol(0);
    check(res->type == Type::ATOM && res->atom == "0", "Converts zero to a numeric atom: intToSymbol(0)", exprToString(res), "0");
}

void testCar() {
    currentTestFunction = "TEST CAR";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------ TEST CAR -------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: Car should return the first element of an S-expression." << endl;
    
    SExpr* a = makeAtom("a");
    SExpr* b = makeAtom("b");
    SExpr* c = makeAtom("c");
    SExpr* d = makeAtom("d");
    SExpr* nil = makeNil();

    SExpr* listBC = cons(b, cons(c, nil));
    SExpr* listABC = cons(a, listBC);
    check(car(listABC) == a, "Returns the first atom of a list: car((a b c))", exprToString(car(listABC)), "a");

    SExpr* listAB = cons(a, b); 
    SExpr* listABCD = cons(listAB, cons(c, cons(d, nil)));
    check(car(listABCD) == listAB, "Returns a nested expression from the car: car(((a . b) c d))", exprToString(car(listABCD)), "(a . b)");

    SExpr* dottedPair = cons(a, b);
    check(car(dottedPair) == a, "Returns the first atom of a dotted pair: car((a . b))", exprToString(car(dottedPair)), "a");
}

void testCdr() {
    currentTestFunction = "TEST CDR";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------- TEST CDR ------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: Cdr should return everything except the first element of an S-expression." << endl;

    SExpr* a = makeAtom("a");
    SExpr* b = makeAtom("b");
    SExpr* c = makeAtom("c");
    SExpr* d = makeAtom("d");

    SExpr* nil = makeNil();

    SExpr* listBC = cons(b, cons(c, nil));
    SExpr* listABC = cons(a, listBC);
    check(cdr(listABC) == listBC, "Returns all elements after the first element: cdr((a b c))", exprToString(cdr(listABC)), "(b c)");

    SExpr* listAB = cons(a, cons(b, nil));
    SExpr* listCD = cons(c, cons(d, nil));
    SExpr* listABCD = cons(listAB, listCD);
    check(cdr(listABCD) == listCD, "Returns all elements after the first element of a nested list: cdr(((a b) c d))", exprToString(cdr(listABCD)), "(c d)");

    SExpr* dottedPair = cons(a, b);
    check(cdr(dottedPair) == b, "Returns the second part of a dotted pair: cdr((a . b))", exprToString(cdr(dottedPair)), "b");
}

void checkCons(SExpr* first, SExpr* second, string description, string carExpected, string cdrExpected){
    SExpr* res = cons(first, second);

    cout << "\n\n🔎 Test: " << description;

    check(res->type == Type::CELL, "Creates a new cell.", res->type == Type::CELL ? "CELL" : "NOT CELL", "CELL");
    check(res->car == first, "Stores the first argument as the car.", exprToString(res->car), carExpected);
    check(res->cdr == second, "Stores the second argument as the cdr.", exprToString(res->cdr), cdrExpected);
}

void testCons() {
    currentTestFunction = "TEST CONS";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------ TEST CONS ------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: Cons should create a new cell using the first argument as the car and the second argument as the cdr." << endl;

    SExpr* a = makeAtom("a");
    SExpr* b = makeAtom("b");
    SExpr* c = makeAtom("c");
    SExpr* nil = makeNil();

    SExpr* listAB = cons(a, cons(b, nil));
    SExpr* listBC = cons(b, cons(c, nil));

    checkCons(a, b, "Cons with two atoms: cons(a, b)", "a", "b");
    checkCons(a, listBC, "Cons with an atom and a list: cons(a, (b c))", "a", "(b c)");
    checkCons(listAB, c, "Cons with a list and an atom: cons((a b), c)", "(a b)", "c");
    checkCons(listAB, listBC, "Cons with two lists: cons((a b), (b c))", "(a b)", "(b c)");
    checkCons(a, nil, "Cons with an atom and (): cons(a, ())", "a", "()");
    
}

void testQuote(){
    currentTestFunction = "TEST QUOTE";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------ TEST QUOTE -----------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: Quote should return its argument without evaluating it." << endl;

    SExpr* a = makeAtom("a");
    SExpr* b = makeAtom("b");
    SExpr* nil = makeNil();

    SExpr* listAB = cons(a, cons(b, nil));

    check(quote(a) == a, "Returns an atom without evaluating it: quote(a)", exprToString(quote(a)), "a");
    check(quote(listAB) == listAB, "Returns a list without evaluating it: quote((a b))", exprToString(quote(listAB)), "(a b)");
    check(quote(nil) == nil, "Returns () without evaluating it: quote(())", exprToString(quote(nil)), "()");
}

void checkSkipWhitespace(string inputText, char expectedCharacter, string test){
    istringstream input(inputText);
    streambuf* originalCin = cin.rdbuf(input.rdbuf());

    skipWhitespace();

    char nextCharacter = cin.peek();

    cin.rdbuf(originalCin);

    check(nextCharacter == expectedCharacter, test, string(1, nextCharacter), string(1, expectedCharacter));
}

void testSkipWhitespace(){
    currentTestFunction = "TEST SKIP WHITESPACE";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------ TEST SKIP WHITESPACE -------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: skipWhitespace should skip leading whitespace, so the next character should not be whitespace." << endl;

    checkSkipWhitespace("   a", 'a', "Skips multiple spaces: \"   a\"");
    checkSkipWhitespace("nospaces", 'n', "Leaves input unchanged when there is no whitespace: \"nospaces\"");
    checkSkipWhitespace("\nnewline", 'n', "Skips a newline: \"\\nnewline\"");
    checkSkipWhitespace("\ttab", 't', "Skips a tab: \"\\ttab\"");
    checkSkipWhitespace(" \n\trandom", 'r', "Skips mixed whitespace: \" \\n\\trandom\"");
}

void testReadAtom(){
    currentTestFunction = "TEST READ ATOM";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "---------------------- TEST READ ATOM ---------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: readAtom should read characters and create an atom with the correct value." << endl;

    checkReadOutput(readAtom, "abc ", makeAtom("abc"), "Reads letters as an atom: \"abc \"");
    checkReadOutput(readAtom, "car ", makeAtom("car"), "Reads a function name as an atom: \"car \"");
    checkReadOutput(readAtom, "123 ", makeAtom("123"), "Reads numbers as an atom: \"123 \"");
    checkReadOutput(readAtom, "+-= ", makeAtom("+-="), "Reads symbols as an atom: \"+-= \"");
    checkReadOutput(readAtom, "par)", makeAtom("par"), "Stops reading an atom at a closing parenthesis: \"par)\"");
    checkReadOutput(readAtom, "space ", makeAtom("space"), "Stops reading an atom at whitespace: \"space \"");
}

void testReadList(){
    currentTestFunction = "TEST READ LIST";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "---------------------- TEST READ LIST ---------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: readList should read the contents of a list and build the correct S-expression." << endl;

    SExpr* a = makeAtom("a");
    SExpr* b = makeAtom("b");
    SExpr* c = makeAtom("c");
    SExpr* nil = makeNil();

    SExpr* expr;

    expr = nil;
    checkReadOutput(readList, ")", expr, "Reads an empty list: \")\"");

    expr = cons(a, nil);
    checkReadOutput(readList, "a)", expr, "Reads a list containing one atom: \"a)\"");

    expr = cons(a, cons(b, cons(c, nil)));
    checkReadOutput(readList, "a b c)", expr, "Reads a list containing multiple atoms: \"a b c)\"");

    SExpr* listAB = cons(a, cons(b, nil));
    expr = cons(listAB, cons(c, nil));
    checkReadOutput(readList, "(a b) c)", expr, "Reads a list containing a nested list: \"(a b) c)\"");
}

void testReadExpr(){
    currentTestFunction = "TEST READ EXPR";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "---------------------- TEST READ EXPR ---------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: readExpr should read an S-expression and determine how it should be interpreted." << endl;

    SExpr* a = makeAtom("a");
    SExpr* b = makeAtom("b");
    SExpr* c = makeAtom("c");
    SExpr* nil = makeNil();

    SExpr* abc = makeAtom("abc");
    SExpr* quoteAtom = makeAtom("quote");

    SExpr* listABC = cons(a, cons(b, cons(c, nil)));

    SExpr* expr;

    checkReadOutput(readExpr, "abc", abc, "Reads an atom: \"abc\"");

    checkReadOutput(readExpr, "(a b c)", listABC, "Reads a list: \"(a b c)\"");

    expr = cons(quoteAtom, cons(a, nil));
    checkReadOutput(readExpr, "'a", expr, "Reads quote shorthand for an atom: \"'a\"");

    expr = cons(quoteAtom, cons(listABC, nil));
    checkReadOutput(readExpr, "'(a b c)", expr, "Reads quote shorthand for a list: \"'(a b c)\"");

    checkReadOutput(readExpr, "()", nil, "Reads an empty list: \"()\"");

}

void testPrintExpr(){
    currentTestFunction = "TEST PRINT EXPR";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "---------------------- TEST PRINT EXPR --------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: printExpr should print an S-expression in the correct format." << endl;

    SExpr* a = makeAtom("a");
    SExpr* b = makeAtom("b");
    SExpr* c = makeAtom("c");
    SExpr* nil = makeNil();

    SExpr* expr;

    checkPrintOutput(printExpr, a, "a", "Prints an atom: printExpr(a)");
    checkPrintOutput(printExpr, nil, "()", "Prints (): printExpr(())");

    expr = cons(a, nil);
    checkPrintOutput(printExpr, expr, "(a)", "Prints a single-element list: printExpr((a))");

    expr = cons(a, cons(b, cons(c, nil)));
    checkPrintOutput(printExpr, expr, "(a b c)", "Prints a list: printExpr((a b c))");

    SExpr* listAB = cons(a, cons(b, nil));
    expr = cons(listAB, cons(c, nil));
    checkPrintOutput(printExpr, expr, "((a b) c)", "Prints a nested list: printExpr(((a b) c))");

    expr = cons(a, b);
    checkPrintOutput(printExpr, expr, "(a . b)", "Prints a dotted pair: printExpr((a . b))");

}

void testPrintList(){
    currentTestFunction = "TEST PRINT LIST";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "---------------------- TEST PRINT LIST --------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: printList should print the contents of a list in the correct format without the outer parentheses." << endl;

    SExpr* a = makeAtom("a");
    SExpr* b = makeAtom("b");
    SExpr* c = makeAtom("c");
    SExpr* nil = makeNil();

    SExpr* expr;

    expr = cons(a, cons(b, cons(c, nil)));
    checkPrintOutput(printList, expr, "a b c", "Prints the contents of a proper list: printList((a b c))");

    expr = cons(a, b);
    checkPrintOutput(printList, expr, "a . b", "Prints the contents of a dotted pair: printList((a . b))");

    SExpr* listAB = cons(a, cons(b, nil));
    expr = cons(listAB, cons(c, nil));
    checkPrintOutput(printList, expr, "(a b) c", "Prints the contents of a nested list: printList(((a b) c))");
}

void testMakePair(){
    currentTestFunction = "TEST MAKE PAIR";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "--------------------- TEST MAKE PAIR ----------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: makePair should create a list containing a name and its value." << endl;

    SExpr* name = makeAtom("a");
    SExpr* value = makeAtom("2");

    cout << "\n🔎 Test: Creates a name-value pair: makePair(a, 2)";

    SExpr* pair = makePair(name, value);

    check(pair->type == Type::CELL, "Creates a pair with type CELL.", pair->type == Type::CELL ? "CELL" : "NOT CELL", "CELL");
    check(car(pair) == name, "Stores the name as the first element.", exprToString(car(pair)), "a");
    check(car(cdr(pair)) == value, "Stores the value as the second element.", exprToString(car(cdr(pair))), "2");
    check(isNil(cdr(cdr(pair))), "Ends the pair with ().", exprToString(cdr(cdr(pair))), "()");
}

void testLookup(){
    currentTestFunction = "TEST LOOKUP";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "----------------------- TEST LOOKUP -----------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: Lookup should search local variables before global variables and return the symbol itself if it is undefined." << endl;

    SExpr* a = makeAtom("a");
    SExpr* b = makeAtom("b");
    SExpr* c = makeAtom("c");
    SExpr* x = makeAtom("x");
    SExpr* two = makeAtom("2");
    SExpr* three = makeAtom("3");
    SExpr* four = makeAtom("4");
    SExpr* ten = makeAtom("10");

    SExpr* res;

    rho = makeNil();
    local = makeNil();

    // Global environment: ((c 4) (b 4) (a 2))
    rho = cons(makePair(a, two), rho);
    rho = cons(makePair(b, four), rho);
    rho = cons(makePair(c, four), rho);

    res = lookup(a);
    check(res == two, "Returns the value associated with a global symbol: lookup(a)", exprToString(res), "2");

    res = lookup(x);
    check(res == x, "Returns an undefined symbol unchanged: lookup(x)", exprToString(res), "x");

    rho = cons(makePair(a, ten), rho);

    res = lookup(a);
    check(res == ten, "Returns the most recent global value: lookup(a)", exprToString(res), "10");

    // Local environment: ((a b) (2 3))
    SExpr* names = cons(a, cons(b, makeNil()));
    SExpr* values = cons(two, cons(three, makeNil()));
    local = cons(cons(names, cons(values, makeNil())), local);

    res = lookup(b);
    check(res == three, "Returns a local value: lookup(b)", exprToString(res), "3");

    res = lookup(a);
    check(res == two, "Returns the local value before the global value: lookup(a)", exprToString(res), "2");

    res = lookup(c);
    check(res == four, "Returns a global value when the symbol is not local: lookup(c)", exprToString(res), "4");

    local = cdr(local);

    res = lookup(a);
    check(res == ten, "Returns the global value after the local environment is removed: lookup(a)", exprToString(res), "10");

    rho = makeNil();
    local = makeNil();
}

void testSet(){
    currentTestFunction = "TEST SET";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------- TEST SET ------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: set should store a value in the environment and return the assigned value." << endl;

    rho = makeNil();

    SExpr* res;

    res = evalString("(set a 2)");
    check(exprToString(res) == "2", "Returns the value assigned to a symbol: (set a 2)", exprToString(res), "2");

    res = evalString("a");
    check(exprToString(res) == "2", "Evaluates a defined symbol to its value: eval(a)", exprToString(res), "2");

    res = evalString("(set a 10)");
    check(exprToString(res) == "10", "Returns the new value when a symbol is redefined: (set a 10)", exprToString(res), "10");

    SExpr* previous = car(cdr(car(cdr(rho))));
    check(exprToString(previous) == "2", "Keeps the previous definition when a symbol is redefined.", exprToString(previous), "2");

    res = evalString("a");
    check(exprToString(res) == "10", "Evaluates a redefined symbol to its most recent value: eval(a)", exprToString(res), "10");
}

void testNil(){
    currentTestFunction = "TEST NIL?";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "---------------------- TEST NIL? --------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: nil? should return T when its argument is () and () otherwise." << endl;

    rho = makeNil();

    SExpr* res;

    res = evalString("(nil? ())");
    check(exprToString(res) == "T", "Returns T when the argument is (): (nil? ())", exprToString(res), "T");

    res = evalString("(nil? 'a)");
    check(isNil(res), "Returns () when the argument is not (): (nil? 'a)", exprToString(res), "()");
}

void testAtom(){
    currentTestFunction = "TEST ATOM?";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "--------------------- TEST ATOM? --------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: atom? should return T when its argument is an atom and () when its argument is a list." << endl;

    rho = makeNil();

    SExpr* res;

    res = evalString("(atom? x)");
    check(exprToString(res) == "T", "Returns T when the argument is an atom: (atom? x)", exprToString(res), "T");

    res = evalString("(atom? (x))");
    check(isNil(res), "Returns () when the argument is a list: (atom? (x))", exprToString(res), "()");
}

void testList(){
    currentTestFunction = "TEST LIST?";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "--------------------- TEST LIST? --------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: list? should return T when its argument is a list and () when its argument is an atom." << endl;

    rho = makeNil();

    SExpr* res;

    res = evalString("(list? (x))");
    check(exprToString(res) == "T", "Returns T when the argument is a list: (list? (x))", exprToString(res), "T");

    res = evalString("(list? x)");
    check(isNil(res), "Returns () when the argument is an atom: (list? x)", exprToString(res), "()");
}

void testNot(){
    currentTestFunction = "TEST NOT?";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "---------------------- TEST NOT? --------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: not? should return T when its argument is () and () otherwise." << endl;

    rho = makeNil();

    SExpr* res;

    res = evalString("(not? ())");
    check(exprToString(res) == "T", "Returns T when the argument is (): (not? ())", exprToString(res), "T");

    res = evalString("(not? 'a)");
    check(isNil(res), "Returns () when the argument is not (): (not? 'a)", exprToString(res), "()");
}

void testNumber(){
    currentTestFunction = "TEST NUMBER?";
    
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "-------------------- TEST NUMBER? -------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: number? should return T when its argument is a numeric atom and () otherwise." << endl;

    rho = makeNil();

    SExpr* res;

    res = evalString("(number? 123)");
    check(exprToString(res) == "T", "Returns T for a positive integer: (number? 123)", exprToString(res), "T");

    res = evalString("(number? -42)");
    check(exprToString(res) == "T", "Returns T for a negative integer: (number? -42)", exprToString(res), "T");

    res = evalString("(number? 'abc)");
    check(isNil(res), "Returns () for letters: (number? 'abc)", exprToString(res), "()");

    res = evalString("(number? '12a)");
    check(isNil(res), "Returns () for mixed characters: (number? '12a)", exprToString(res), "()");

    res = evalString("(number? '-)");
    check(isNil(res), "Returns () for a minus sign alone: (number? '-)", exprToString(res), "()");

    res = evalString("(number? ())");
    check(isNil(res), "Returns () for (): (number? ())", exprToString(res), "()");
}

void testAnd(){
    currentTestFunction = "TEST AND?";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------ TEST AND? ------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: and? returns () if either argument is (). Anything other than () is treated as true." << endl;

    rho = makeNil();

    SExpr* res;

    res = evalString("(and? 'T 'T)");
    check(exprToString(res) == "T", "Returns T when both arguments are T: (and? 'T 'T)", exprToString(res), "T");

    res = evalString("(and? 'T ())");
    check(isNil(res), "Returns () when the second argument is (): (and? 'T ())", exprToString(res), "()");

    res = evalString("(and? () 'T)");
    check(isNil(res), "Returns () when the first argument is (): (and? () 'T)", exprToString(res), "()");

    res = evalString("(and? 'T 'a)");
    check(exprToString(res) == "a", "Treats anything other than () as true: (and? 'T 'a)", exprToString(res), "a");

    res = evalString("(and? () (set x 1))");
    check(isNil(res), "Returns () when the first argument is (): (and? () (set x 1))", exprToString(res), "()");

    res = evalString("x");
    check(exprToString(res) == "x", "Does not evaluate the second argument when the first argument is (): eval(x)", exprToString(res), "x");
}

void testOr(){
    currentTestFunction = "TEST OR?";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------ TEST OR? -------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: or? returns () only if both arguments are (). Anything other than () is treated as true." << endl;

    rho = makeNil();

    SExpr* res;

    res = evalString("(or? 'T ())");
    check(exprToString(res) == "T", "Returns T when the first argument is T: (or? 'T ())", exprToString(res), "T");

    res = evalString("(or? () 'T)");
    check(exprToString(res) == "T", "Returns T when the second argument is T: (or? () 'T)", exprToString(res), "T");

    res = evalString("(or? () ())");
    check(isNil(res), "Returns () when both arguments are (): (or? () ())", exprToString(res), "()");

    res = evalString("(or? () 'a)");
    check(exprToString(res) == "a", "Treats anything other than () as true: (or? () 'a)", exprToString(res), "a");

    // The first argument should be evaluated.
    res = evalString("(or? (set x 1) ())");
    check(exprToString(res) == "1", "Returns the result of the first argument: (or? (set x 1) ())", exprToString(res), "1");

    res = evalString("x");
    check(exprToString(res) == "1", "Evaluates the first argument: eval(x)", exprToString(res), "1");

    // The second argument should not be evaluated when the first argument is T.
    res = evalString("(or? 'T (set y 1))");
    check(exprToString(res) == "T", "Returns T without evaluating the second argument: (or? 'T (set y 1))", exprToString(res), "T");

    res = evalString("y");
    check(exprToString(res) == "y", "Does not evaluate the second argument when the first argument is T: eval(y)", exprToString(res), "y");
}

void testEq(){
    currentTestFunction = "TEST EQ?";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------ TEST EQ? -------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: eq? returns T when two atoms are the same. If the atoms are different or either argument is not an atom, it returns ()." << endl;

    rho = makeNil();

    SExpr* res;

    res = evalString("(eq? 'a 'a)");
    check(exprToString(res) == "T", "Returns T when both atoms are the same: (eq? 'a 'a)", exprToString(res), "T");

    res = evalString("(eq? 'a 'b)");
    check(isNil(res), "Returns () when the atoms are different: (eq? 'a 'b)", exprToString(res), "()");

    res = evalString("(eq? () ())");
    check(isNil(res), "Returns () when the arguments are (): (eq? () ())", exprToString(res), "()");

    res = evalString("(eq? '(a) '(a))");
    check(isNil(res), "Returns () when the arguments are lists: (eq? '(a) '(a))", exprToString(res), "()");

    res = evalString("(eq? 'a ())");
    check(isNil(res), "Returns () when only one argument is an atom: (eq? 'a ())", exprToString(res), "()");
}

void testIf(){
    currentTestFunction = "TEST IF";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------- TEST IF -------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: if evaluates the true branch when the condition is anything other than (), and the false branch when the condition is (). Only the chosen branch is evaluated." << endl;

    rho = makeNil();

    SExpr* res;

    res = evalString("(if 'T 'yes 'no)");
    check(exprToString(res) == "yes", "Returns the true branch when the condition is T: (if 'T 'yes 'no)", exprToString(res), "yes");

    res = evalString("(if () 'yes 'no)");
    check(exprToString(res) == "no", "Returns the false branch when the condition is (): (if () 'yes 'no)", exprToString(res), "no");

    evalString("(if 'T 'yes (set falseBranch 1))");
    res = evalString("falseBranch");
    check(exprToString(res) == "falseBranch", "Does not evaluate the false branch when the condition is T: eval(falseBranch)", exprToString(res), "falseBranch");

    evalString("(if () (set trueBranch 1) 'no)");
    res = evalString("trueBranch");
    check(exprToString(res) == "trueBranch", "Does not evaluate the true branch when the condition is (): eval(trueBranch)", exprToString(res), "trueBranch");

    res = evalString("(if 'a 'yes 'no)");
    check(exprToString(res) == "yes", "Treats anything other than () as true: (if 'a 'yes 'no)", exprToString(res), "yes");
}

void testCond(){
    currentTestFunction = "TEST COND";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------ TEST COND ------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: cond evaluates conditions in order and returns the result associated with the first condition that is not ()." << endl;

    rho = makeNil();

    SExpr* res;

    res = evalString("(cond ('T 'first () 'second))");
    check(exprToString(res) == "first", "Returns the result of the first true condition: (cond ('T 'first () 'second))", exprToString(res), "first");

    res = evalString("(cond (() 'first 'T 'second))");
    check(exprToString(res) == "second", "Continues to the next condition when a condition is (): (cond (() 'first 'T 'second))", exprToString(res), "second");

    evalString("(cond ('T 'first 'T (set laterBranch 1)))");

    res = evalString("laterBranch");
    check(exprToString(res) == "laterBranch", "Does not evaluate later results after finding a true condition: eval(laterBranch)", exprToString(res), "laterBranch");

    evalString("(cond ((set conditionRan 1) 'first 'T 'second))");

    res = evalString("conditionRan");
    check(exprToString(res) == "1", "Evaluates the condition: eval(conditionRan)", exprToString(res), "1");
}

void testAdd(){
    currentTestFunction = "TEST ADD";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------ TEST ADD -------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: add should return the sum of two numeric arguments." << endl;

    rho = makeNil();

    SExpr* res;

    res = evalString("(add 2 3)");
    check(symbolToInt(res) == 2 + 3, "Adds two positive integers: (add 2 3)", exprToString(res), "5");

    res = evalString("(add -5 3)");
    check(symbolToInt(res) == -5 + 3, "Adds a negative and positive integer: (add -5 3)", exprToString(res), "-2");

    res = evalString("(add 0 5)");
    check(symbolToInt(res) == 0 + 5, "Adds zero: (add 0 5)", exprToString(res), "5");

    res = evalString("(add (add 2 3) 4)");
    check(symbolToInt(res) == (2 + 3) + 4, "Evaluates an argument before adding: (add (add 2 3) 4)", exprToString(res), "9");
}

void testSub(){
    currentTestFunction = "TEST SUB";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------ TEST SUB -------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: sub should return the difference of two numeric arguments." << endl;

    rho = makeNil();

    SExpr* res;

    res = evalString("(sub 10 3)");
    check(symbolToInt(res) == 10 - 3, "Subtracts two positive integers: (sub 10 3)", exprToString(res), "7");

    res = evalString("(sub 3 10)");
    check(symbolToInt(res) == 3 - 10, "Returns a negative result when the second argument is larger: (sub 3 10)", exprToString(res), "-7");

    res = evalString("(sub 5 0)");
    check(symbolToInt(res) == 5 - 0, "Subtracts zero: (sub 5 0)", exprToString(res), "5");

    res = evalString("(sub (add 2 3) 1)");
    check(symbolToInt(res) == (2 + 3) - 1, "Evaluates an argument before subtracting: (sub (add 2 3) 1)", exprToString(res), "4");
}

void testMul(){
    currentTestFunction = "TEST MUL";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------ TEST MUL -------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: mul should return the product of two numeric arguments." << endl;

    rho = makeNil();

    SExpr* res;

    res = evalString("(mul 4 5)");
    check(symbolToInt(res) == 4 * 5, "Multiplies two positive integers: (mul 4 5)", exprToString(res), "20");

    res = evalString("(mul -3 4)");
    check(symbolToInt(res) == -3 * 4, "Multiplies a negative and positive integer: (mul -3 4)", exprToString(res), "-12");

    res = evalString("(mul 5 0)");
    check(symbolToInt(res) == 5 * 0, "Multiplies by zero: (mul 5 0)", exprToString(res), "0");

    res = evalString("(mul (add 2 3) (sub 6 2))");
    check(symbolToInt(res) == (2 + 3) * (6 - 2), "Evaluates both arguments before multiplying: (mul (add 2 3) (sub 6 2))", exprToString(res), "20");
}

void testDiv(){
    currentTestFunction = "TEST DIV";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------ TEST DIV -------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: div should return the integer quotient of two numeric arguments." << endl;

    rho = makeNil();

    SExpr* res;

    res = evalString("(div 10 2)");
    check(symbolToInt(res) == 10 / 2, "Divides two positive integers: (div 10 2)", exprToString(res), "5");

    res = evalString("(div 7 2)");
    check(symbolToInt(res) == 7 / 2, "Uses integer division: (div 7 2)", exprToString(res), "3");

    res = evalString("(div -12 3)");
    check(symbolToInt(res) == -12 / 3, "Divides a negative integer by a positive integer: (div -12 3)", exprToString(res), "-4");

    res = evalString("(div (mul 3 4) (add 1 2))");
    check(symbolToInt(res) == (3 * 4) / (1 + 2), "Evaluates both arguments before dividing: (div (mul 3 4) (add 1 2))", exprToString(res), "4");
}

void testRem(){
    currentTestFunction = "TEST REM";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------ TEST REM -------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: rem should return the remainder after dividing two numeric arguments." << endl;

    rho = makeNil();

    SExpr* res;

    res = evalString("(rem 10 3)");
    check(symbolToInt(res) == 10 % 3, "Returns the remainder: (rem 10 3)", exprToString(res), "1");

    res = evalString("(rem 8 4)");
    check(symbolToInt(res) == 8 % 4, "Returns zero when there is no remainder: (rem 8 4)", exprToString(res), "0");

    res = evalString("(rem 7 2)");
    check(symbolToInt(res) == 7 % 2, "Returns the remainder when dividing by two: (rem 7 2)", exprToString(res), "1");

    res = evalString("(rem (add 5 5) 4)");
    check(symbolToInt(res) == (5 + 5) % 4, "Evaluates an argument before finding the remainder: (rem (add 5 5) 4)", exprToString(res), "2");
}

void testLt(){
    currentTestFunction = "TEST LT";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------- TEST LT -------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: lt should return T when the first numeric argument is less than the second and () otherwise." << endl;

    rho = makeNil();

    SExpr* res;

    res = evalString("(lt 2 5)");
    check(exprToString(res) == "T", "Returns T when the first argument is less than the second: (lt 2 5)", exprToString(res), "T");

    res = evalString("(lt 5 2)");
    check(isNil(res), "Returns () when the first argument is greater than the second: (lt 5 2)", exprToString(res), "()");

    res = evalString("(lt 5 5)");
    check(isNil(res), "Returns () when the arguments are equal: (lt 5 5)", exprToString(res), "()");

    res = evalString("(lt (sub 5 3) (add 2 3))");
    check(exprToString(res) == "T", "Evaluates both arguments before comparing: (lt (sub 5 3) (add 2 3))", exprToString(res), "T");
}

void testFunction(){
    currentTestFunction = "TEST FUNCTION";
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "---------------------- TEST FUNCTION ----------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: Function expressions should evaluate to themselves." << endl;

    rho = makeNil();
    local = makeNil();

    SExpr* res;

    res = evalString("(function (a b) (add a b))");
    check(exprToString(res) == "(function (a b) (add a b))", "Returns a function expression unchanged: (function (a b) (add a b))", exprToString(res), "(function (a b) (add a b))");

    res = evalString("(function () 5)");
    check(exprToString(res) == "(function () 5)", "Returns a function with no arguments unchanged: (function () 5)", exprToString(res), "(function () 5)");

    res = evalString("(function (a) a)");
    check(exprToString(res) == "(function (a) a)", "Returns a function with one argument unchanged: (function (a) a)", exprToString(res), "(function (a) a)");

    rho = makeNil();
    local = makeNil();
}

void testFunctionCalls(){
    currentTestFunction = "TEST FUNCTION CALLS";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------ TEST FUNCTION CALLS --------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: User-defined functions should evaluate passed values using a local environment." << endl;

    rho = makeNil();
    local = makeNil();

    // foo = (function (a b) (add a b))
    SExpr* foo = makeAtom("foo");
    SExpr* function = readString("(function (a b) (add a b))");
    rho = cons(makePair(foo, function), rho);

    SExpr* res = evalString("(foo 2 3)");
    check(exprToString(res) == "5", "Calls a user-defined function: (foo 2 3)", exprToString(res), "5");

    res = evalString("(foo (add 1 2) (mul 2 3))");
    check(exprToString(res) == "9", "Evaluates arguments before calling a user-defined function: (foo (add 1 2) (mul 2 3))", exprToString(res), "9");

    check(isNil(local), "Removes the local environment after a function returns", exprToString(local), "()");

    // function with no arguments
    SExpr* noArgs = makeAtom("noArgs");
    SExpr* noArgsFunction = readString("(function () 5)");
    rho = cons(makePair(noArgs, noArgsFunction), rho);

    res = evalString("(noArgs)");
    check(exprToString(res) == "5", "Calls a user-defined function with no arguments: (noArgs)", exprToString(res), "5");

    // bar = (function (x) (foo x 5))
    SExpr* bar = makeAtom("bar");
    SExpr* barFunction = readString("(function (x) (foo x 5))");
    rho = cons(makePair(bar, barFunction), rho);

    res = evalString("(bar 3)");
    check(exprToString(res) == "8", "Calls a user-defined function from another function: (bar 3)", exprToString(res), "8");

    check(isNil(local), "Removes local environments after nested function calls", exprToString(local), "()");

    // Reset the environments
    rho = makeNil();
    local = makeNil();
}

void testEval(){
    currentTestFunction = "TEST EVAL";

    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------- TEST EVAL -----------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: Eval should correctly evaluate supported S-expressions and operations." << endl;

    rho = makeNil();

    SExpr* a = makeAtom("a");
    SExpr* b = makeAtom("b");
    SExpr* c = makeAtom("c");
    SExpr* nil = makeNil();

    SExpr* carAtom = makeAtom("car");
    SExpr* cdrAtom = makeAtom("cdr");
    SExpr* consAtom = makeAtom("cons");
    SExpr* quoteAtom = makeAtom("quote");
    SExpr* evalAtom = makeAtom("eval");

    SExpr* listBC = cons(b, cons(c, nil));
    SExpr* listABC = cons(a, listBC);

    SExpr* expr;
    SExpr* res;

    expr = a;
    res = eval(expr);
    check(res == a, "Returns an undefined atom unchanged: eval(a)", exprToString(res), "a");

    expr = nil;
    res = eval(expr);
    check(res == nil, "Returns NIL unchanged: eval(())", exprToString(res), "()");

    expr = cons(carAtom, cons(listABC, nil));
    res = eval(expr);
    check(res == a, "Evaluates car with a list argument: eval((car (a b c)))", exprToString(res), "a");

    expr = cons(cdrAtom, cons(listABC, nil));
    res = eval(expr);
    check(res == listBC, "Evaluates cdr with a list argument: eval((cdr (a b c)))", exprToString(res), "(b c)");

    expr = cons(consAtom, cons(a, cons(b, nil)));
    res = eval(expr);
    check(res->type == Type::CELL && res->car == a && res->cdr == b,
          "Evaluates cons with atom arguments: eval((cons a b))", exprToString(res), "(a . b)");

    expr = cons(quoteAtom, cons(a, nil));
    res = eval(expr);
    check(res == a, "Evaluates quote without evaluating its argument: eval((quote a))", exprToString(res), "a");

    expr = cons(evalAtom, cons(a, nil));
    res = eval(expr);
    check(res == a, "Evaluates an eval expression: eval((eval a))", exprToString(res), "a");

    SExpr* quoteListABC = cons(quoteAtom, cons(listABC, nil));

    expr = cons(carAtom, cons(quoteListABC, nil));
    res = eval(expr);
    check(res == a, "Evaluates car with a quoted list: eval((car (quote (a b c))))", exprToString(res), "a");

    expr = cons(cdrAtom, cons(quoteListABC, nil));
    res = eval(expr);
    check(res == listBC, "Evaluates cdr with a quoted list: eval((cdr (quote (a b c))))", exprToString(res), "(b c)");

    SExpr* quoteA = cons(quoteAtom, cons(a, nil));
    SExpr* quoteListBC = cons(quoteAtom, cons(listBC, nil));

    expr = cons(consAtom, cons(quoteA, cons(quoteListBC, nil)));
    res = eval(expr);
    check(res->type == Type::CELL && res->car == a && res->cdr == listBC,
          "Evaluates cons with quoted arguments: eval((cons (quote a) (quote (b c))))", exprToString(res), "(a b c)");

    expr = cons(evalAtom, cons(cons(carAtom, cons(listABC, nil)), nil));
    res = eval(expr);
    check(res == a, "Evaluates the result of another expression: eval((eval (car (a b c))))", exprToString(res), "a");
}

void testReport(){
    cout << "\n\n===========================================================" << endl;
    cout << "                        TEST REPORT                        " << endl;
    cout << "===========================================================" << endl;
    cout << "✅ Passed: " << testsPassed << endl;
    cout << "❌ Failed: " << testsFailed << endl;
    cout << "Total:  " << testsPassed + testsFailed << endl;

    if (testsFailed > 0){
        cout << "\nFailed Tests:" << endl;

        string lastTestFunction = "";

        for (TestFailure failure : failedTests){
            if (failure.testFunction != lastTestFunction){
                cout << "\n❌ " << failure.testFunction << endl;
                lastTestFunction = failure.testFunction;
            }

            cout << "   - Line " << failure.line << ": " << failure.test << endl;
        }
    }
}

int main() {
    testMakeAtom();
    testMakeNil();
    testIsAtom();
    testIsNil();
    testIsNumber();
    testSymbolToInt();
    testIntToSymbol();
    
    testCar();
    testCdr();
    testCons();
    testQuote();

    testSkipWhitespace();
    testReadAtom();
    testReadList();
    testReadExpr();
    
    testPrintExpr();
    testPrintList();

    testMakePair();
    testLookup();
    testSet();
    
    testNil();
    testAtom();
    testList();
    testNot();
    testNumber();
    
    testAnd();
    testOr();
    testEq();
    testIf();
    testCond();

    testAdd();
    testSub();
    testMul();
    testDiv();
    testRem();
    testLt();

    testFunction();
    testFunctionCalls();

    testEval();

    testReport();

    return 0;
}