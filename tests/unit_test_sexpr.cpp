#include "sexpr.h"

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

void testMakeAtom(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "--------------------- TEST MAKE ATOM ----------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;

    string value = "string";
    SExpr* atom = makeAtom(value);

    if (atom->type == Type::ATOM && atom->atom == value){
        cout << "\n✅ PASS ✅\t" << "Creates an atom with the correct type and value. | makeAtom(\"string\") -> ATOM \"string\"" << endl;
    }
    else {
        cout << "\n❌ FAIL ❌\t" << "Should create an atom with the correct type and value. | makeAtom(\"string\") -> ATOM \"string\"" << endl;
    }
}

void testMakeNil(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "--------------------- TEST MAKE NIL -----------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;

    SExpr* nil = makeNil();

    if (nil->type == Type::NIL){
        cout << "\n✅ PASS ✅\t" << "Creates an S-expression with type NIL. | makeNil() -> NIL" << endl;
    }
    else {
        cout << "\n❌ FAIL ❌\t" << "Creates an S-expression with type NIL. | makeNil() -> NIL" << endl;
    }
}

void testIsAtom(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "---------------------- TEST IS ATOM -----------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: isAtom should return true only when the S-expression is an atom." << endl;

    SExpr* atom = makeAtom("atom");
    SExpr* nil = makeNil();
    SExpr* cell = cons(atom, nil);

    if (isAtom(atom)){
        cout << "\n✅ PASS ✅\t Identifies an atom as an atom. | isAtom(a) -> true" << endl;
    } else {
        cout << "\n❌ FAIL ❌\t Identifies an atom as an atom. | isAtom(a) -> true" << endl;
    }

    if (!isAtom(nil)){
        cout << "\n✅ PASS ✅\t Does not identify NIL as an atom. | isAtom(NIL) -> false" << endl;
    } else {
        cout << "\n❌ FAIL ❌\t Does not identify NIL as an atom. | isAtom(NIL) -> false" << endl;

    }

    if (!isAtom(cell)){
        cout << "\n✅ PASS ✅\t Does not identify a cell as an atom. | isAtom(cell) -> false" << endl;
    } else {
        cout << "\n❌ FAIL ❌\t Does not identify a cell as an atom. | isAtom(cell) -> false" << endl;

    }
    
}

void testIsNil(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "---------------------- TEST IS NIL ------------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: isNil should return true only when the S-expression is NIL." << endl;

    SExpr* atom = makeAtom("atom");
    SExpr* nil = makeNil();
    SExpr* cell = cons(atom, nil);

    if (isNil(nil)){
        cout << "\n✅ PASS ✅\t";
    } else {
        cout << "\n❌ FAIL ❌\t";
    }
    cout << "Identifies NIL as NIL. | isNil(nil) -> true" << endl;

    if (!isNil(atom)){
        cout << "\n✅ PASS ✅\t";
    } else {
        cout << "\n❌ FAIL ❌\t";
    }
    cout << "Does not identify an atom as NIL. | isNil(atom) -> false" << endl;

    if (!isNil(cell)){
        cout << "\n✅ PASS ✅\t";
    } else {
        cout << "\n❌ FAIL ❌\t";
    }
    cout << "Does not identify a cell as NIL. | isNil(cell) -> false" << endl;
    
}

void checkSkipWhitespace(string inputText, char expectedCharacter, string test){
    istringstream input(inputText);
    streambuf* originalCin = cin.rdbuf(input.rdbuf());

    skipWhitespace();

    char nextCharacter = cin.peek();

    if (nextCharacter == expectedCharacter){
        cout << "\n✅ PASS ✅\t";
    }
    else {
        cout << "\n❌ FAIL ❌\t";
    }
    cout << test << " | Next character is '" << nextCharacter << "'." << endl;

    cin.rdbuf(originalCin);

}

void testSkipWhitespace(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------ TEST SKIP WHITESPACE -------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: Next character should never be whitespace." << endl;

    checkSkipWhitespace("   a", 'a', "Skips multiple spaces");
    checkSkipWhitespace("nospaces", 'n', "No spaces.");
    checkSkipWhitespace("\nnewline", 'n', "Skips newline.");
    checkSkipWhitespace("\ttab", 't', "Skips tab.");
    checkSkipWhitespace(" \n\trandom", 'r', "Skips mixed whitespace.");
}

void checkCar(SExpr* input, SExpr* expected, string test){

    SExpr* actual = car(input);

    if (actual == expected) {
        cout << "\n✅ PASS ✅\t";
    }
    else {
        cout << "\n❌ FAIL ❌\t";
    }
    cout << test << endl;

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
    checkCar(listABC, a, "Returns an atom stored in the car of a list. | (car (a b c)) -> a");

    SExpr* listAB = cons(a, b); 
    SExpr* listCD = cons(c, cons(d, nil));
    SExpr* listABCD = cons(listAB, cons(c, cons(d, nil)));
    checkCar(listABCD, listAB, "Returns a nested list stored in the car of a list. | (car ((a b) c d)) -> (a b)");

    SExpr* dottedPair = cons(a, b);
    checkCar(dottedPair, a, "Returns the car of a dotted pair. | (car (a . b)) -> a");


}

void checkCdr(SExpr* input, SExpr* expected, string test){

    SExpr* actual = cdr(input);

    if (actual == expected) {
        cout << "\n✅ PASS ✅\t" << test << endl;
    }
    else {
        cout << "\n❌ FAIL ❌\t" << test << endl;
    }

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
    checkCdr(listABC, listBC, "Returns the cdr of a list. | (cdr (a b c)) -> (b c)");


    SExpr* listAB = cons(a, cons(b, nil));
    SExpr* listCD = cons(c, cons(d, nil));
    SExpr* listABCD = cons(listAB, listCD);
    checkCdr(listABCD, listCD, "Returns the cdr when the first element is a nested list. | (cdr ((a b) c d)) -> (c d)");

    SExpr* dottedPair = cons(a, b);
    checkCdr(dottedPair, b, "Returns the cdr of a dotted pair. | (cdr (a . b)) -> b");
}

void checkCons(SExpr* first, SExpr* second, string description, string test, string carExpected, string cdrExpected){
    SExpr* result = cons(first, second);


    cout << "\n\n🔎 Test: " << description << " | " << test;

    if (result->type == Type::CELL){
        cout << "\n✅ PASS ✅\t";
    }
    else {
        cout << "\n❌ FAIL ❌\t";
    }
    cout << "Creates a new cell. | " << test << " should have type -> CELL";

    if (result->car == first){
        cout << "\n✅ PASS ✅\t";
    }
    else {
        cout << "\n❌ FAIL ❌\t";
    }
    cout << "Stores the first argument as the car. | " << test << " should have car -> " << carExpected;

    if (result->cdr == second){
        cout << "\n✅ PASS ✅\t";
    }
    else {
        cout << "\n❌ FAIL ❌\t";
    }
    cout << "Stores the second argument as the cdr. | " << test << " should have cdr -> " << cdrExpected << endl;
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

    if (quote(a) == a) {
        cout << "\n✅ PASS ✅\t";
    }
    else {
        cout << "\n❌ FAIL ❌\t";
    }
    cout << "Returns an atom without evaluating it. | quote(a) -> a" << endl;

    if (quote(listAB) == listAB) {
        cout << "\n✅ PASS ✅\t";
    }
    else {
        cout << "\n❌ FAIL ❌\t";
    }
    cout << "Returns a list without evaluating it. | quote((a b)) -> (a b)" << endl;

    if (quote(nil) == nil) {
        cout << "\n✅ PASS ✅\t";
    }
    else {
        cout << "\n❌ FAIL ❌\t";
    }
    cout << "Returns NIL without evaluating it. | quote(()) -> ()" << endl;
}

void checkEval(SExpr* input, SExpr* expected, string description, string test){
    SExpr* result = eval(input);

    if (result == expected){
        cout << "\n✅ PASS ✅\t";
    }
    else {
        cout << "\n❌ FAIL ❌\t";
    }

    cout << description << " | " << test << endl;

}

void testEval(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "------------------------- TEST EVAL -----------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: Eval should evaluate atoms, NIL, and expressions containing the supported operations car, cdr, cons, quote, and eval." << endl;

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
    checkEval(expr, a, "Returns an atom unchanged.", "eval(a) -> a");

    expr = nil;
    checkEval(expr, nil, "Returns NIL unchanged.", "eval(()) -> ()");
    
    expr = cons(carAtom, cons(listABC, nil));
    checkEval(expr, a, "Evaluates a simple car expression.", "eval((car (a b c))) -> a");

    expr = cons(cdrAtom, cons(listABC, nil));
    checkEval(expr, listBC, "Evaluates a simple cdr expression.", "eval((cdr (a b c))) -> (b c)");

    expr = cons(consAtom, cons(a, cons(b, nil)));
    result = eval(expr);
    if (result->type == Type::CELL && result->car == a && result->cdr == b){
        cout << "\n✅ PASS ✅\t";
    }
    else {
        cout << "\n❌ FAIL ❌\t";
    }
    cout << "Evaluates a simple cons expression. | eval((cons a b)) -> (a . b)" << endl;

    expr = cons(quoteAtom, cons(a, nil));
    checkEval(expr, a, "Evaluates a simple quote expression.", "eval((quote a)) -> a");

    expr = cons(evalAtom, cons(a, nil));
    checkEval(expr, a, "Evaluates a simple eval expression.", "eval((eval a)) -> a");

    SExpr* quoteListABC = cons(quoteAtom, cons(listABC, nil));
    expr = cons(carAtom, cons(quoteListABC, nil));
    checkEval(expr, a, "Evaluates car with a quoted list.", "eval((car (quote (a b c)))) -> a");

    expr = cons(cdrAtom, cons(quoteListABC, nil));
    checkEval(expr, listBC, "Evaluates cdr with a quoted list.", "eval((cdr (quote (a b c)))) -> (b c)");

    SExpr* quoteA = cons(quoteAtom, cons(a, nil));
    SExpr* quoteListBC = cons(quoteAtom, cons(listBC, nil));
    expr = cons(consAtom, cons(quoteA, cons(quoteListBC, nil)));
    result = eval(expr);
    if (result->type == Type::CELL && result->car == a && result->cdr == listBC){
        cout << "\n✅ PASS ✅\t";
    }
    else {
        cout << "\n❌ FAIL ❌\t";
    }
    cout << "Evaluates cons with quoted arguments. | " << "eval((cons (quote a) (quote (b c)))) -> (a b c)" << endl;


    expr = cons(evalAtom, cons(cons(carAtom, cons(listABC, nil)), nil));
    checkEval(expr, a, "Evaluates the result of another expression.", "eval((eval (car (a b c)))) -> a");
}

void checkReadAtom(string input, string expected, string test){

    istringstream testInput(input + " ");
    streambuf* originalCin = cin.rdbuf(testInput.rdbuf());

    cin.clear();

    SExpr* result = readAtom();

    cin.rdbuf(originalCin);
    cin.clear();

    cout << "\n\n🔎 Test: " << test;

    if (result->type == Type::ATOM){
        cout << "\n✅ PASS ✅\t";
    }
    else {
        cout << "\n❌ FAIL ❌\t";
    }
    cout << "Creates the correct type. | readAtom should have type -> ATOM";

    if (result->atom == expected){
        cout << "\n✅ PASS ✅\t";
    }
    else{
        cout << "\n❌ FAIL ❌\t";
    }
    cout << "Stores the correct value. | readAtom should have value -> \"" << expected << "\"" << endl;

}

void testReadAtom(){
    cout << "\n-----------------------------------------------------------" << endl;
    cout << "---------------------- TEST READ ATOM ---------------------" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "📌 Note: readAtom should read characters and create an atom with the correct value." << endl;

    checkReadAtom("abc", "abc", "Reads letters as an atom.");
    checkReadAtom("car", "car", "Reads a function name as an atom.");
    checkReadAtom("123", "123", "Reads numbers as an atom.");
    checkReadAtom("+-=", "+-=", "Reads symbols as an atom.");
    checkReadAtom("par)", "par", "Reads an atom followed by a closing parenthesis.");
    checkReadAtom("space ", "space", "Reads an atom followed by whitespace.");
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

void checkReadList(string input, SExpr* expected, string description, string expectedOutput){

    istringstream testInput(input);
    streambuf* originalCin = cin.rdbuf(testInput.rdbuf());

    SExpr* result = readList();

    cin.rdbuf(originalCin);

    cout << "\n\n🔎 Test: " << description;

    // Check type
    if (result->type == expected->type){
        cout << "\n✅ PASS ✅\t";
    }
    else {
        cout << "\n❌ FAIL ❌\t";
    }
    cout << "Creates the correct type. | readList should have type -> ";

    if (expected->type == Type::NIL) {
        cout << "NIL";
        return; // NIL has no contents to check.
    }
    else {
        cout << "CELL";
    }

    // Check contents
    if (sameContents(result, expected)){
        cout << "\n✅ PASS ✅\t";
    }
    else {
        cout << "\n❌ FAIL ❌\t";
    }
    cout << "Stores the correct contents. | readList should contain -> " << expectedOutput;

    // Find the end of the list
    SExpr* end = result;
    while (end->type == Type::CELL){
        end = end->cdr;
    }

    // Check that it ends with NIL
    if (end->type == Type::NIL) {
        cout << "\n✅ PASS ✅\t";
    }
    else {
        cout << "\n❌ FAIL ❌\t";
    }
    cout << "Ends the list correctly. | readList should end with -> NIL" << endl;


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
    checkReadList(")", expr, "Reads an empty list.", "NIL");

    expr = cons(a, nil);
    checkReadList("a)", expr, "Reads a list containing one atom.", "(a)");

    expr = cons(a, cons(b, cons(c, nil)));
    checkReadList("a b c)", expr, "Reads a list containing multiple atoms.", "(a b c)");

    SExpr* listAB = cons(a, cons(b, nil));
    expr = cons(listAB, cons(c, nil));
    checkReadList("(a b) c)", expr, "Reads a list containing a nested list.", "((a b) c)");

}

void checkReadExpr(string input, SExpr* expected, string description, string expectedOutput){

    istringstream testInput(input + " ");
    streambuf* originalCin = cin.rdbuf(testInput.rdbuf());

    SExpr* result = readExpr();

    cin.rdbuf(originalCin);

    cout << "\n\n🔎 Test: " << description;

    // Check type
    if (result->type == expected->type){
        cout << "\n✅ PASS ✅\t";
    }
    else {
        cout << "\n❌ FAIL ❌\t";
    }

    cout << "Creates the correct type. | readExpr should have type -> ";

    if (expected->type == Type::ATOM){
        cout << "ATOM";
    }
    else if (expected->type == Type::NIL){
        cout << "NIL";
    }
    else {
        cout << "CELL";
    }

    // Check contents
    if (sameContents(result, expected)){
        cout << "\n✅ PASS ✅\t";
    }
    else {
        cout << "\n❌ FAIL ❌\t";
    }

    cout << "Stores the correct contents. | readExpr should contain -> " << expectedOutput << endl;

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
    

    checkReadExpr("abc", abc, "Reads an atom.", "abc");

    checkReadExpr("(a b c)", listABC, "Reads a list.", "(a b c)");

    expr = cons(quoteAtom, cons(a, nil));
    checkReadExpr("'a", expr, "Reads quote shorthand for an atom.", "(quote a)");

    expr = cons(quoteAtom, cons(listABC, nil));
    checkReadExpr("'(a b c)", expr, "Reads quote shorthand for a list.", "(quote (a b c))");

    checkReadExpr("()", nil, "Reads an empty list.", "NIL");

}

void checkPrint(void (*printFunction)(SExpr*), SExpr* input, string expected, string description){

    ostringstream output;
    streambuf* originalCout = cout.rdbuf(output.rdbuf());

    printFunction(input);

    cout.rdbuf(originalCout);


    if (output.str() == expected){
        cout << "\n✅ PASS ✅\t";
    }
    else {
        cout << "\n❌ FAIL ❌\t";
    }
    cout << description << " | printed -> " << output.str() << ", expected " << expected << endl;

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

    checkPrint(printExpr, a, "a", "Prints an atom.");
    checkPrint(printExpr, nil, "()", "Prints NIL.");

    SExpr* listABC = cons(a, cons(b, cons(c, nil)));
    checkPrint(printExpr, listABC, "(a b c)", "Prints a list.");

    SExpr* listAB = cons(a, cons(b, nil));
    SExpr* nestedList = cons(listAB, cons(c, nil));
    checkPrint(printExpr, nestedList, "((a b) c)", "Prints a nested list.");

    SExpr* dottedPair = cons(a, b);
    checkPrint(printExpr, dottedPair, "(a . b)", "Prints a dotted pair.");

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

    SExpr* listABC = cons(a, cons(b, cons(c, nil)));
    checkPrint(printList, listABC, "a b c", "Prints the contents of a proper list.");

    SExpr* dottedPair = cons(a, b);
    checkPrint(printList, dottedPair, "a . b", "Prints the contents of a dotted pair.");

}


int main() {
    testMakeAtom();
    testMakeNil();
    testIsAtom();
    testIsNil();
    testSkipWhitespace();
    testCar();
    testCdr();
    testCons();
    testQuote();
    testEval();
    testReadAtom();
    testReadList();
    testReadExpr();
    testPrintExpr();
    testPrintList();

    return 0;
}