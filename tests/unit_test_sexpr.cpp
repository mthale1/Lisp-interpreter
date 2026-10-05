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

void testMakeAtom(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "--------------------- TEST MAKE ATOM ----------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: makeAtom should create an atom with the correct type and value." << endl;

    SExpr* result;

    result = makeAtom("string");
    check(result->type == Type::ATOM, "Creates an S-expression with the correct type.", result->type == Type::ATOM ? "ATOM" : "NOT ATOM", "ATOM");
    check(result->atom == "string", "Stores the correct atom value.", result->atom, "string");

    result = makeAtom("123");
    check(result->atom == "123", "Stores numbers as atom text.", result->atom, "123");

    result = makeAtom("+-=");
    check(result->atom == "+-=", "Stores symbols as atom text.", result->atom, "+-=");
}

void testMakeNil(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "--------------------- TEST MAKE NIL -----------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: makeNil should create an S-expression with type NIL." << endl;

    SExpr* result = makeNil();

    check(result->type == Type::NIL, "Creates an S-expression with the correct type.", result->type == Type::NIL ? "NIL" : "NOT NIL", "NIL");
}

void testIsAtom(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "---------------------- TEST IS ATOM -----------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: isAtom should return true only when the S-expression is an atom." << endl;

    SExpr* atom = makeAtom("atom");
    SExpr* nil = makeNil();
    SExpr* cell = cons(atom, nil);

    check(isAtom(atom), "Identifies an atom as an atom.", isAtom(atom) ? "true" : "false", "true");
    check(!isAtom(nil), "Does not identify NIL as an atom.", isAtom(nil) ? "true" : "false", "false");
    check(!isAtom(cell), "Does not identify a cell as an atom.", isAtom(cell) ? "true" : "false", "false");
}

void testIsNil(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "---------------------- TEST IS NIL ------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: isNil should return true only when the S-expression is NIL." << endl;

    SExpr* atom = makeAtom("atom");
    SExpr* nil = makeNil();
    SExpr* cell = cons(atom, nil);

    check(isNil(nil), "Identifies NIL as NIL.", isNil(nil) ? "true" : "false", "true");
    check(!isNil(atom), "Does not identify an atom as NIL.", isNil(atom) ? "true" : "false", "false");
    check(!isNil(cell), "Does not identify a cell as NIL.", isNil(cell) ? "true" : "false", "false");
}

void testIsNumber(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "--------------------- TEST IS NUMBER ----------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: isNumber should return true only when the S-expression is an atom representing a valid integer." << endl;

    SExpr* input;

    input = makeAtom("123");
    check(isNumber(input), "Identifies a positive integer.", isNumber(input) ? "true" : "false", "true");

    input = makeAtom("-42");
    check(isNumber(input), "Identifies a negative integer.", isNumber(input) ? "true" : "false", "true");

    input = makeAtom("abc");
    check(!isNumber(input), "Does not identify letters as an integer.", isNumber(input) ? "true" : "false", "false");

    input = makeAtom("12a");
    check(!isNumber(input), "Does not identify mixed characters as an integer.", isNumber(input) ? "true" : "false", "false");

    input = makeAtom("-");
    check(!isNumber(input), "Does not identify a minus sign alone as an integer.", isNumber(input) ? "true" : "false", "false");

    input = makeNil();
    check(!isNumber(input), "Does not identify NIL as an integer.", isNumber(input) ? "true" : "false", "false");
}

void testSymbolToInt(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------ TEST SYMBOL TO INT ---------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: symbolToInt should convert an atom representing an integer into a C++ integer." << endl;

    check(symbolToInt(makeAtom("123")) == 123, "Converts a positive numeric atom to an integer.",to_string(symbolToInt(makeAtom("123"))), "123");
    check(symbolToInt(makeAtom("-42")) == -42, "Converts a negative numeric atom to an integer.", to_string(symbolToInt(makeAtom("-42"))), "-42");
    check(symbolToInt(makeAtom("0")) == 0, "Converts zero to an integer.", to_string(symbolToInt(makeAtom("0"))), "0");
}

void testIntToSymbol(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------ TEST INT TO SYMBOL ---------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: intToSymbol should convert a C++ integer into an atom S-expression." << endl;

    SExpr* result;

    result = intToSymbol(123);
    check(result->type == Type::ATOM && result->atom == "123", "Converts a positive integer to a numeric atom.", exprToString(result), "123");

    result = intToSymbol(-42);
    check(result->type == Type::ATOM && result->atom == "-42", "Converts a negative integer to a numeric atom.", exprToString(result), "-42");

    result = intToSymbol(0);
    check(result->type == Type::ATOM && result->atom == "0", "Converts zero to a numeric atom.", exprToString(result), "0");
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
    check(car(listABC) == a, "Returns an atom stored in the car of a list.", exprToString(car(listABC)), "a");

    SExpr* listAB = cons(a, b); 
    SExpr* listABCD = cons(listAB, cons(c, cons(d, nil)));
    check(car(listABCD) == listAB, "Returns a nested list stored in the car of a list.", exprToString(car(listABCD)), "(a . b)");

    SExpr* dottedPair = cons(a, b);
    check(car(dottedPair) == a, "Returns the car of a dotted pair.", exprToString(car(dottedPair)), "a");
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
    check(cdr(listABC) == listBC, "Returns the rest of a list.", exprToString(cdr(listABC)), "(b c)");

    SExpr* listAB = cons(a, cons(b, nil));
    SExpr* listCD = cons(c, cons(d, nil));
    SExpr* listABCD = cons(listAB, listCD);
    check(cdr(listABCD) == listCD, "Returns the rest of a nested list.", exprToString(cdr(listABCD)), "(c d)");

    SExpr* dottedPair = cons(a, b);
    check(cdr(dottedPair) == b, "Returns the cdr of a dotted pair.", exprToString(cdr(dottedPair)), "b");
}

void checkCons(SExpr* first, SExpr* second, string description, string test, string carExpected, string cdrExpected){
    SExpr* result = cons(first, second);

    cout << "\n\n🔎 Test: " << description << " | " << test;

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

    checkCons(a, b, "Both arguments are atoms.", "cons(a, b)", "a", "b");
    checkCons(a, listBC, "The first argument is an atom and the second is a list.", "cons(a, (b c))", "a", "(b c)");
    checkCons(listAB, c, "The first argument is a list and the second is an atom.", "cons((a b), c)", "(a b)", "c");
    checkCons(listAB, listBC, "Both arguments are lists.", "cons((a b), (b c))", "(a b)", "(b c)");
    checkCons(a, nil, "The second argument is NIL.", "cons(a, ())", "a", "()");
    
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

    check(quote(a) == a, "Returns an atom without evaluating it.", exprToString(quote(a)), "a");
    check(quote(listAB) == listAB, "Returns a list without evaluating it.", exprToString(quote(listAB)), "(a b)");
    check(quote(nil) == nil, "Returns NIL without evaluating it.", exprToString(quote(nil)), "()");
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
    cout << "📌 Note: Next character should never be whitespace." << endl;

    checkSkipWhitespace("   a", 'a', "Skips multiple spaces.");
    checkSkipWhitespace("nospaces", 'n', "No spaces.");
    checkSkipWhitespace("\nnewline", 'n', "Skips newline.");
    checkSkipWhitespace("\ttab", 't', "Skips tab.");
    checkSkipWhitespace(" \n\trandom", 'r', "Skips mixed whitespace.");
}

void testReadAtom(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "---------------------- TEST READ ATOM ---------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: readAtom should read characters and create an atom with the correct value." << endl;

    checkReadOutput(readAtom, "abc ", makeAtom("abc"), "Reads letters as an atom.");
    checkReadOutput(readAtom, "car ", makeAtom("car"), "Reads a function name as an atom.");
    checkReadOutput(readAtom, "123 ", makeAtom("123"), "Reads numbers as an atom.");
    checkReadOutput(readAtom, "+-= ", makeAtom("+-="), "Reads symbols as an atom.");
    checkReadOutput(readAtom, "par)", makeAtom("par"), "Reads an atom followed by a closing parenthesis.");
    checkReadOutput(readAtom, "space ", makeAtom("space"), "Reads an atom followed by whitespace.");
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
    checkReadOutput(readList, ")", expr, "Reads an empty list.");

    expr = cons(a, nil);
    checkReadOutput(readList, "a)", expr, "Reads a list containing one atom.");

    expr = cons(a, cons(b, cons(c, nil)));
    checkReadOutput(readList, "a b c)", expr, "Reads a list containing multiple atoms.");

    SExpr* listAB = cons(a, cons(b, nil));
    expr = cons(listAB, cons(c, nil));
    checkReadOutput(readList, "(a b) c)", expr, "Reads a list containing a nested list.");
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

    checkReadOutput(readExpr, "abc", abc, "Reads an atom.");

    checkReadOutput(readExpr, "(a b c)", listABC, "Reads a list.");

    expr = cons(quoteAtom, cons(a, nil));
    checkReadOutput(readExpr, "'a", expr, "Reads quote shorthand for an atom.");

    expr = cons(quoteAtom, cons(listABC, nil));
    checkReadOutput(readExpr, "'(a b c)", expr, "Reads quote shorthand for a list.");

    checkReadOutput(readExpr, "()", nil, "Reads an empty list.");

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

    checkPrintOutput(printExpr, a, "a", "Prints an atom.");
    checkPrintOutput(printExpr, nil, "()", "Prints NIL.");

    expr = cons(a, nil);
    checkPrintOutput(printExpr, expr, "(a)", "Prints a single-element list.");

    expr = cons(a, cons(b, cons(c, nil)));
    checkPrintOutput(printExpr, expr, "(a b c)", "Prints a list.");

    SExpr* listAB = cons(a, cons(b, nil));
    expr = cons(listAB, cons(c, nil));
    checkPrintOutput(printExpr, expr, "((a b) c)", "Prints a nested list.");

    expr = cons(a, b);
    checkPrintOutput(printExpr, expr, "(a . b)", "Prints a dotted pair.");

}

void testPrintList(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "---------------------- TEST PRINT LIST --------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: printList should print the contents of a list in the correct format without the parentheses." << endl;

    SExpr* a = makeAtom("a");
    SExpr* b = makeAtom("b");
    SExpr* c = makeAtom("c");
    SExpr* nil = makeNil();

    SExpr* expr;

    expr = cons(a, cons(b, cons(c, nil)));
    checkPrintOutput(printList, expr, "a b c", "Prints the contents of a proper list.");

    expr = cons(a, b);
    checkPrintOutput(printList, expr, "a . b", "Prints the contents of a dotted pair.");

    SExpr* listAB = cons(a, cons(b, nil));
    expr = cons(listAB, cons(c, nil));
    checkPrintOutput(printList, expr, "(a b) c", "Prints the contents of a nested list.");

}

void testMakePair(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "--------------------- TEST MAKE PAIR ----------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: makePair should create a list containing a name and its value." << endl;

    SExpr* name = makeAtom("a");
    SExpr* value = makeAtom("2");

    SExpr* pair = makePair(name, value);

    check(pair->type == Type::CELL, "Creates a pair with type CELL.", pair->type == Type::CELL ? "CELL" : "NOT CELL", "CELL");
    check(car(pair) == name, "Stores the name as the first element.", exprToString(car(pair)), "a");
    check(car(cdr(pair)) == value, "Stores the value as the second element.", exprToString(car(cdr(pair))), "2");
    check(isNil(cdr(cdr(pair))), "Ends the pair with NIL.", exprToString(cdr(cdr(pair))), "()");
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
    check(result == two, "Finds the value associated with a symbol.", exprToString(result), "2");

    result = lookup(b);
    check(result == four, "Finds another value in the environment.", exprToString(result), "4");

    result = lookup(x);
    check(result == x, "Returns an undefined symbol unchanged.", exprToString(result), "x");

    rho = cons(makePair(a, ten), rho);

    result = lookup(a);
    check(result == ten, "Returns the most recent value when a symbol is defined again.", exprToString(result), "10");
}

void testSet(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------- TEST SET ------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: Set should store a value in the environment and return the assigned value." << endl;

    SExpr* a = makeAtom("a");
    SExpr* two = makeAtom("2");
    SExpr* ten = makeAtom("10");
    SExpr* nil = makeNil();
    SExpr* setAtom = makeAtom("set");

    SExpr* expr;
    SExpr* result;

    rho = makeNil();

    expr = cons(setAtom, cons(a, cons(two, nil)));
    result = eval(expr);
    check(result == two, "Returns the value assigned to a symbol.", exprToString(result), "2");

    result = eval(a);
    check(result == two, "Evaluates a defined symbol to its value.", exprToString(result), "2");

    expr = cons(setAtom, cons(a, cons(ten, nil)));
    result = eval(expr);
    check(result == ten, "Returns the new value when a symbol is redefined.", exprToString(result), "10");

    check(car(cdr(car(cdr(rho)))) == two, "Keeps the previous definition when a symbol is redefined.", exprToString(car(cdr(car(cdr(rho))))), "2");

    result = eval(a);
    check(result == ten, "Evaluates a redefined symbol to its most recent value.", exprToString(result), "10");
}

void testPredicates(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "-------------------- TEST PREDICATES ----------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: Predicates should return T when true and NIL when false." << endl;

    rho = makeNil();

    SExpr* nil = makeNil();
    SExpr* a = makeAtom("a");
    SExpr* number = makeAtom("42");
    SExpr* list = cons(a, cons(makeAtom("b"), nil));

    SExpr* expr;
    SExpr* result;

    expr = cons(makeAtom("nil?"), cons(nil, nil));
    result = eval(expr);
    check(exprToString(result) == "T", "nil? returns true for NIL.", exprToString(result), "T");

    expr = cons(makeAtom("nil?"), cons(a, nil));
    result = eval(expr);
    check(isNil(result), "nil? returns false for an atom.", exprToString(result), "()");

    expr = cons(makeAtom("atom?"), cons(a, nil));
    result = eval(expr);
    check(exprToString(result) == "T", "atom? returns true for an atom.", exprToString(result), "T");

    expr = cons(makeAtom("atom?"), cons(list, nil));
    result = eval(expr);
    check(isNil(result), "atom? returns false for a list.", exprToString(result), "()");

    expr = cons(makeAtom("list?"), cons(list, nil));
    result = eval(expr);
    check(exprToString(result) == "T", "list? returns true for a list.", exprToString(result), "T");

    expr = cons(makeAtom("list?"), cons(a, nil));
    result = eval(expr);
    check(isNil(result), "list? returns false for an atom.", exprToString(result), "()");

    expr = cons(makeAtom("not?"), cons(nil, nil));
    result = eval(expr);
    check(exprToString(result) == "T", "not? returns true for NIL.", exprToString(result), "T");

    expr = cons(makeAtom("not?"), cons(a, nil));
    result = eval(expr);
    check(isNil(result), "not? returns false for a non-NIL value.", exprToString(result), "()");

    expr = cons(makeAtom("number?"), cons(number, nil));
    result = eval(expr);
    check(exprToString(result) == "T", "number? returns true for a numeric atom.", exprToString(result), "T");

    expr = cons(makeAtom("number?"), cons(a, nil));
    result = eval(expr);
    check(isNil(result), "number? returns false for a nonnumeric atom.", exprToString(result), "()");
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
    check(result == a, "Returns an undefined atom unchanged.", exprToString(result), "a");

    expr = nil;
    result = eval(expr);
    check(result == nil, "Returns NIL unchanged.", exprToString(result), "()");

    expr = cons(carAtom, cons(listABC, nil));
    result = eval(expr);
    check(result == a, "Evaluates a simple car expression.", exprToString(result), "a");

    expr = cons(cdrAtom, cons(listABC, nil));
    result = eval(expr);
    check(result == listBC, "Evaluates a simple cdr expression.", exprToString(result), "(b c)");

    expr = cons(consAtom, cons(a, cons(b, nil)));
    result = eval(expr);
    check(result->type == Type::CELL && result->car == a && result->cdr == b,
          "Evaluates a simple cons expression.", exprToString(result), "(a . b)");

    expr = cons(quoteAtom, cons(a, nil));
    result = eval(expr);
    check(result == a, "Evaluates a simple quote expression.", exprToString(result), "a");

    expr = cons(evalAtom, cons(a, nil));
    result = eval(expr);
    check(result == a, "Evaluates a simple eval expression.", exprToString(result), "a");

    SExpr* quoteListABC = cons(quoteAtom, cons(listABC, nil));

    expr = cons(carAtom, cons(quoteListABC, nil));
    result = eval(expr);
    check(result == a, "Evaluates car with a quoted list.", exprToString(result), "a");

    expr = cons(cdrAtom, cons(quoteListABC, nil));
    result = eval(expr);
    check(result == listBC, "Evaluates cdr with a quoted list.", exprToString(result), "(b c)");

    SExpr* quoteA = cons(quoteAtom, cons(a, nil));
    SExpr* quoteListBC = cons(quoteAtom, cons(listBC, nil));

    expr = cons(consAtom, cons(quoteA, cons(quoteListBC, nil)));
    result = eval(expr);
    check(result->type == Type::CELL && result->car == a && result->cdr == listBC,
          "Evaluates cons with quoted arguments.", exprToString(result), "(a b c)");

    expr = cons(evalAtom, cons(cons(carAtom, cons(listABC, nil)), nil));
    result = eval(expr);
    check(result == a, "Evaluates the result of another expression.", exprToString(result), "a");
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
    testPredicates();

    testEval();

    return 0;
}