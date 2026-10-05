#include "sexpr.h"

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

// Unit test helper functions
void check(bool condition, string test, string actual, string expected){

    if (condition){
        cout << "\n✅ PASS \t";
    }
    else {
        cout << "\n❌ FAIL \t";
    }

    cout << test << " | Actual: " << actual << " | Expected: " << expected << endl;
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
    SExpr* result = readFunction();

    cin.rdbuf(originalCin);
    cin.clear();

    cout << "\n\n🔎 Test: " << test << endl;

    string actualType = result->type == Type::NIL ? "NIL" : result->type == Type::CELL ? "CELL" : "ATOM";
    string expectedType = expected->type == Type::NIL ? "NIL" : expected->type == Type::CELL ? "CELL" : "ATOM";

    check(result->type == expected->type, "Creates the correct type.", actualType, expectedType);
    check(sameContents(result, expected), "Stores the correct contents.", exprToString(result), exprToString(expected));
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
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "--------------------- TEST MAKE ATOM ----------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: makeAtom should create an atom with the correct type and value." << endl;

    SExpr* result;

    result = makeAtom("string");
    check(result->type == Type::ATOM, "Creates an S-expression with the correct type: makeAtom(\"string\")", result->type == Type::ATOM ? "ATOM" : "NOT ATOM", "ATOM");
    check(result->atom == "string", "Stores the correct atom value: makeAtom(\"string\")", result->atom, "string");

    result = makeAtom("123");
    check(result->atom == "123", "Stores numbers as atom text: makeAtom(\"123\")", result->atom, "123");

    result = makeAtom("+-=");
    check(result->atom == "+-=", "Stores symbols as atom text: makeAtom(\"+-=\")", result->atom, "+-=");
}

void testMakeNil(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "--------------------- TEST MAKE NIL -----------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: makeNil should create an S-expression with type NIL." << endl;

    SExpr* result = makeNil();

    check(result->type == Type::NIL, "Creates an S-expression with the correct type: makeNil()", result->type == Type::NIL ? "NIL" : "NOT NIL", "NIL");
}

void testIsAtom(){
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
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------ TEST SYMBOL TO INT ---------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: symbolToInt should convert an atom representing an integer into a C++ integer." << endl;

    check(symbolToInt(makeAtom("123")) == 123, "Converts a positive numeric atom to an integer: symbolToInt(123)",to_string(symbolToInt(makeAtom("123"))), "123");
    check(symbolToInt(makeAtom("-42")) == -42, "Converts a negative numeric atom to an integer: symbolToInt(-42)", to_string(symbolToInt(makeAtom("-42"))), "-42");
    check(symbolToInt(makeAtom("0")) == 0, "Converts zero to an integer: symbolToInt(0)", to_string(symbolToInt(makeAtom("0"))), "0");
}

void testIntToSymbol(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------ TEST INT TO SYMBOL ---------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: intToSymbol should convert a C++ integer into an atom S-expression." << endl;

    SExpr* result;

    result = intToSymbol(123);
    check(result->type == Type::ATOM && result->atom == "123", "Converts a positive integer to a numeric atom: intToSymbol(123)", exprToString(result), "123");

    result = intToSymbol(-42);
    check(result->type == Type::ATOM && result->atom == "-42", "Converts a negative integer to a numeric atom: intToSymbol(-42)", exprToString(result), "-42");

    result = intToSymbol(0);
    check(result->type == Type::ATOM && result->atom == "0", "Converts zero to a numeric atom: intToSymbol(0)", exprToString(result), "0");
}

void testCar() {
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
    SExpr* result = cons(first, second);

    cout << "\n\n🔎 Test: " << description;

    check(result->type == Type::CELL, "Creates a new cell.", result->type == Type::CELL ? "CELL" : "NOT CELL", "CELL");
    check(result->car == first, "Stores the first argument as the car.", exprToString(result->car), carExpected);
    check(result->cdr == second, "Stores the second argument as the cdr.", exprToString(result->cdr), cdrExpected);
}

void testCons() {
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
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "----------------------- TEST LOOKUP -----------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: Lookup should return the most recent value associated with a symbol or return the symbol itself if it is undefined." << endl;

    SExpr* a = makeAtom("a");
    SExpr* b = makeAtom("b");
    SExpr* x = makeAtom("x");
    SExpr* two = makeAtom("2");
    SExpr* four = makeAtom("4");
    SExpr* ten = makeAtom("10");

    SExpr* result;

    rho = makeNil();

    rho = cons(makePair(a, two), rho);
    rho = cons(makePair(b, four), rho);

    result = lookup(a);
    check(result == two, "Returns the value associated with a symbol: lookup(a)", exprToString(result), "2");

    result = lookup(b);
    check(result == four, "Returns another value stored in the environment: lookup(b)", exprToString(result), "4");

    result = lookup(x);
    check(result == x, "Returns an undefined symbol unchanged: lookup(x)", exprToString(result), "x");

    rho = cons(makePair(a, ten), rho);

    result = lookup(a);
    check(result == ten, "Returns the most recent value when a symbol is defined again: lookup(a)", exprToString(result), "10");
}

void testSet(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------- TEST SET ------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: set should store a value in the environment and return the assigned value." << endl;

    rho = makeNil();

    SExpr* result;

    result = evalString("(set a 2)");
    check(exprToString(result) == "2", "Returns the value assigned to a symbol: (set a 2)", exprToString(result), "2");

    result = evalString("a");
    check(exprToString(result) == "2", "Evaluates a defined symbol to its value: eval(a)", exprToString(result), "2");

    result = evalString("(set a 10)");
    check(exprToString(result) == "10", "Returns the new value when a symbol is redefined: (set a 10)", exprToString(result), "10");

    SExpr* previous = car(cdr(car(cdr(rho))));
    check(exprToString(previous) == "2", "Keeps the previous definition when a symbol is redefined.", exprToString(previous), "2");

    result = evalString("a");
    check(exprToString(result) == "10", "Evaluates a redefined symbol to its most recent value: eval(a)", exprToString(result), "10");
}

void testNil(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "---------------------- TEST NIL? --------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: nil? should return T when its argument is () and () otherwise." << endl;

    rho = makeNil();

    SExpr* result;

    result = evalString("(nil? ())");
    check(exprToString(result) == "T", "Returns T when the argument is (): (nil? ())", exprToString(result), "T");

    result = evalString("(nil? 'a)");
    check(isNil(result), "Returns () when the argument is not (): (nil? 'a)", exprToString(result), "()");
}

void testAtom(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "--------------------- TEST ATOM? --------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: atom? should return T when its argument is an atom and () when its argument is a list." << endl;

    rho = makeNil();

    SExpr* result;

    result = evalString("(atom? x)");
    check(exprToString(result) == "T", "Returns T when the argument is an atom: (atom? x)", exprToString(result), "T");

    result = evalString("(atom? (x))");
    check(isNil(result), "Returns () when the argument is a list: (atom? (x))", exprToString(result), "()");
}

void testList(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "--------------------- TEST LIST? --------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: list? should return T when its argument is a list and () when its argument is an atom." << endl;

    rho = makeNil();

    SExpr* result;

    result = evalString("(list? (x))");
    check(exprToString(result) == "T", "Returns T when the argument is a list: (list? (x))", exprToString(result), "T");

    result = evalString("(list? x)");
    check(isNil(result), "Returns () when the argument is an atom: (list? x)", exprToString(result), "()");
}

void testNot(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "---------------------- TEST NOT? --------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: not? should return T when its argument is () and () otherwise." << endl;

    rho = makeNil();

    SExpr* result;

    result = evalString("(not? ())");
    check(exprToString(result) == "T", "Returns T when the argument is (): (not? ())", exprToString(result), "T");

    result = evalString("(not? 'a)");
    check(isNil(result), "Returns () when the argument is not (): (not? 'a)", exprToString(result), "()");
}

void testNumber(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "-------------------- TEST NUMBER? -------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: number? should return T when its argument is a numeric atom and () otherwise." << endl;

    rho = makeNil();

    SExpr* result;

    result = evalString("(number? 123)");
    check(exprToString(result) == "T", "Returns T for a positive integer: (number? 123)", exprToString(result), "T");

    result = evalString("(number? -42)");
    check(exprToString(result) == "T", "Returns T for a negative integer: (number? -42)", exprToString(result), "T");

    result = evalString("(number? 'abc)");
    check(isNil(result), "Returns () for letters: (number? 'abc)", exprToString(result), "()");

    result = evalString("(number? '12a)");
    check(isNil(result), "Returns () for mixed characters: (number? '12a)", exprToString(result), "()");

    result = evalString("(number? '-)");
    check(isNil(result), "Returns () for a minus sign alone: (number? '-)", exprToString(result), "()");

    result = evalString("(number? ())");
    check(isNil(result), "Returns () for (): (number? ())", exprToString(result), "()");
}

void testAnd(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------ TEST AND? ------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: and? returns () if either argument is (). Anything other than () is treated as true." << endl;

    rho = makeNil();

    SExpr* result;

    result = evalString("(and? 'T 'T)");
    check(exprToString(result) == "T", "Returns T when both arguments are T: (and? 'T 'T)", exprToString(result), "T");

    result = evalString("(and? 'T ())");
    check(isNil(result), "Returns () when the second argument is (): (and? 'T ())", exprToString(result), "()");

    result = evalString("(and? () 'T)");
    check(isNil(result), "Returns () when the first argument is (): (and? () 'T)", exprToString(result), "()");

    result = evalString("(and? 'T 'a)");
    check(exprToString(result) == "a", "Treats anything other than () as true: (and? 'T 'a)", exprToString(result), "a");

    result = evalString("(and? () (set x 1))");
    check(isNil(result), "Returns () when the first argument is (): (and? () (set x 1))", exprToString(result), "()");

    result = evalString("x");
    check(exprToString(result) == "x", "Does not evaluate the second argument when the first argument is (): eval(x)", exprToString(result), "x");
}

void testOr(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------ TEST OR? -------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: or? returns () only if both arguments are (). Anything other than () is treated as true." << endl;

    rho = makeNil();

    SExpr* result;

    result = evalString("(or? 'T ())");
    check(exprToString(result) == "T", "Returns T when the first argument is T: (or? 'T ())", exprToString(result), "T");

    result = evalString("(or? () 'T)");
    check(exprToString(result) == "T", "Returns T when the second argument is T: (or? () 'T)", exprToString(result), "T");

    result = evalString("(or? () ())");
    check(isNil(result), "Returns () when both arguments are (): (or? () ())", exprToString(result), "()");

    result = evalString("(or? () 'a)");
    check(exprToString(result) == "a", "Treats anything other than () as true: (or? () 'a)", exprToString(result), "a");

    // The first argument should be evaluated.
    result = evalString("(or? (set x 1) ())");
    check(exprToString(result) == "1", "Returns the result of the first argument: (or? (set x 1) ())", exprToString(result), "1");

    result = evalString("x");
    check(exprToString(result) == "1", "Evaluates the first argument: eval(x)", exprToString(result), "1");

    // The second argument should not be evaluated when the first argument is T.
    result = evalString("(or? 'T (set y 1))");
    check(exprToString(result) == "T", "Returns T without evaluating the second argument: (or? 'T (set y 1))", exprToString(result), "T");

    result = evalString("y");
    check(exprToString(result) == "y", "Does not evaluate the second argument when the first argument is T: eval(y)", exprToString(result), "y");
}

void testEq(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------ TEST EQ? -------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: eq? returns T when two atoms are the same. If the atoms are different or either argument is not an atom, it returns ()." << endl;

    rho = makeNil();

    SExpr* result;

    result = evalString("(eq? 'a 'a)");
    check(exprToString(result) == "T", "Returns T when both atoms are the same: (eq? 'a 'a)", exprToString(result), "T");

    result = evalString("(eq? 'a 'b)");
    check(isNil(result), "Returns () when the atoms are different: (eq? 'a 'b)", exprToString(result), "()");

    result = evalString("(eq? () ())");
    check(isNil(result), "Returns () when the arguments are (): (eq? () ())", exprToString(result), "()");

    result = evalString("(eq? '(a) '(a))");
    check(isNil(result), "Returns () when the arguments are lists: (eq? '(a) '(a))", exprToString(result), "()");

    result = evalString("(eq? 'a ())");
    check(isNil(result), "Returns () when only one argument is an atom: (eq? 'a ())", exprToString(result), "()");
}

void testIf(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------- TEST IF -------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: if evaluates the true branch when the condition is anything other than (), and the false branch when the condition is (). Only the chosen branch is evaluated." << endl;

    rho = makeNil();

    SExpr* result;

    result = evalString("(if 'T 'yes 'no)");
    check(exprToString(result) == "yes", "Returns the true branch when the condition is T: (if 'T 'yes 'no)", exprToString(result), "yes");

    result = evalString("(if () 'yes 'no)");
    check(exprToString(result) == "no", "Returns the false branch when the condition is (): (if () 'yes 'no)", exprToString(result), "no");

    evalString("(if 'T 'yes (set falseBranch 1))");
    result = evalString("falseBranch");
    check(exprToString(result) == "falseBranch", "Does not evaluate the false branch when the condition is T: eval(falseBranch)", exprToString(result), "falseBranch");

    evalString("(if () (set trueBranch 1) 'no)");
    result = evalString("trueBranch");
    check(exprToString(result) == "trueBranch", "Does not evaluate the true branch when the condition is (): eval(trueBranch)", exprToString(result), "trueBranch");

    result = evalString("(if 'a 'yes 'no)");
    check(exprToString(result) == "yes", "Treats anything other than () as true: (if 'a 'yes 'no)", exprToString(result), "yes");
}

void testCond(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------ TEST COND ------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: cond evaluates conditions in order and returns the result associated with the first condition that is not ()." << endl;

    rho = makeNil();

    SExpr* result;

    result = evalString("(cond ('T 'first () 'second))");
    check(exprToString(result) == "first", "Returns the result of the first true condition: (cond ('T 'first () 'second))", exprToString(result), "first");

    result = evalString("(cond (() 'first 'T 'second))");
    check(exprToString(result) == "second", "Continues to the next condition when a condition is (): (cond (() 'first 'T 'second))", exprToString(result), "second");

    evalString("(cond ('T 'first 'T (set laterBranch 1)))");

    result = evalString("laterBranch");
    check(exprToString(result) == "laterBranch", "Does not evaluate later results after finding a true condition: eval(laterBranch)", exprToString(result), "laterBranch");

    evalString("(cond ((set conditionRan 1) 'first 'T 'second))");

    result = evalString("conditionRan");
    check(exprToString(result) == "1", "Evaluates the condition: eval(conditionRan)", exprToString(result), "1");
}

void testEval(){
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
    SExpr* result;

    expr = a;
    result = eval(expr);
    check(result == a, "Returns an undefined atom unchanged: eval(a)", exprToString(result), "a");

    expr = nil;
    result = eval(expr);
    check(result == nil, "Returns NIL unchanged: eval(())", exprToString(result), "()");

    expr = cons(carAtom, cons(listABC, nil));
    result = eval(expr);
    check(result == a, "Evaluates car with a list argument: eval((car (a b c)))", exprToString(result), "a");

    expr = cons(cdrAtom, cons(listABC, nil));
    result = eval(expr);
    check(result == listBC, "Evaluates cdr with a list argument: eval((cdr (a b c)))", exprToString(result), "(b c)");

    expr = cons(consAtom, cons(a, cons(b, nil)));
    result = eval(expr);
    check(result->type == Type::CELL && result->car == a && result->cdr == b,
          "Evaluates cons with atom arguments: eval((cons a b))", exprToString(result), "(a . b)");

    expr = cons(quoteAtom, cons(a, nil));
    result = eval(expr);
    check(result == a, "Evaluates quote without evaluating its argument: eval((quote a))", exprToString(result), "a");

    expr = cons(evalAtom, cons(a, nil));
    result = eval(expr);
    check(result == a, "Evaluates an eval expression: eval((eval a))", exprToString(result), "a");

    SExpr* quoteListABC = cons(quoteAtom, cons(listABC, nil));

    expr = cons(carAtom, cons(quoteListABC, nil));
    result = eval(expr);
    check(result == a, "Evaluates car with a quoted list: eval((car (quote (a b c))))", exprToString(result), "a");

    expr = cons(cdrAtom, cons(quoteListABC, nil));
    result = eval(expr);
    check(result == listBC, "Evaluates cdr with a quoted list: eval((cdr (quote (a b c))))", exprToString(result), "(b c)");

    SExpr* quoteA = cons(quoteAtom, cons(a, nil));
    SExpr* quoteListBC = cons(quoteAtom, cons(listBC, nil));

    expr = cons(consAtom, cons(quoteA, cons(quoteListBC, nil)));
    result = eval(expr);
    check(result->type == Type::CELL && result->car == a && result->cdr == listBC,
          "Evaluates cons with quoted arguments: eval((cons (quote a) (quote (b c))))", exprToString(result), "(a b c)");

    expr = cons(evalAtom, cons(cons(carAtom, cons(listABC, nil)), nil));
    result = eval(expr);
    check(result == a, "Evaluates the result of another expression: eval((eval (car (a b c))))", exprToString(result), "a");
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

    testEval();

    return 0;
}