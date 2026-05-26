// =============================================================================
// FUNCTIONS IN C++ - COMPLETE TEACHING GUIDE FOR COMPETITIVE PROGRAMMING
// =============================================================================
// This file teaches EVERY important aspect of functions in C++ that you will
// use in coding competitions and real software development.
//
// Functions are the fundamental building block for writing clean, reusable,
// and modular code.
//
// Topics covered in this file:
//   1. Function Declaration (Prototype), Definition, and Calling
//   2. Parameters - Pass by Value vs Pass by Reference
//   3. Default Arguments
//   4. Function Overloading
//   5. Inline Functions
//   6. Recursion (with multiple classic examples)
//
// Style note: Every concept has detailed comments explaining WHY and HOW,
// exactly like a patient teacher would explain in a classroom.
// =============================================================================

#include <bits/stdc++.h>
// <bits/stdc++.h> includes every standard C++ header. Used in competitive
// programming for speed. In real projects you would include only what you need.

using namespace std;
// Allows us to write "cout" instead of "std::cout", "string" instead of "std::string".

// =============================================================================
// SECTION 0: HELPER FUNCTIONS DEFINED HERE (WITH FULL EXPLANATIONS)
// =============================================================================
// In C++, you must either:
//   A) Define a function BEFORE it is used, OR
//   B) Declare (prototype) it before use and define it later.
//
// We will demonstrate both styles below.
//
// Every function below has an extremely detailed comment block explaining it.
// =============================================================================

// -----------------------------------------------------------------------------
// 1. BASIC FUNCTION - Declaration, Definition, and Calling
// -----------------------------------------------------------------------------

// Function DECLARATION (also called Prototype or Signature)
// This tells the compiler: "A function named add exists that takes two ints
// and returns an int. The actual code (definition) will come later."
int add(int a, int b);   // <-- This is ONLY the declaration (no body)

// Function DEFINITION (the actual implementation)
// This is where the logic lives. It must match the declaration exactly.
int add(int a, int b) {
    // 'a' and 'b' are called PARAMETERS (formal parameters).
    // They are local copies of the values passed when the function is called.
    return a + b;   // return statement sends a value back to the caller
}

// Another simple function - this one has NO parameters and returns nothing (void)
void sayHello() {
    cout << "Hello from a function with no parameters!\n";
}

// A function that takes parameters but returns nothing (just does work)
void printSum(int x, int y) {
    cout << "Sum of " << x << " and " << y << " is " << (x + y) << "\n";
}

// -----------------------------------------------------------------------------
// 2. PASS BY VALUE vs PASS BY REFERENCE
// -----------------------------------------------------------------------------

// PASS BY VALUE (default behavior in C++)
// The function receives a COPY of the argument.
// Any changes inside the function do NOT affect the original variable.
void incrementByValue(int num) {
    // 'num' here is a completely separate variable - a copy of whatever was passed
    num = num + 1;   // This only modifies the local copy
    cout << "   Inside incrementByValue: num is now " << num << "\n";
    // When this function ends, this local 'num' is destroyed.
}

// PASS BY REFERENCE (using &)
// The function receives the ACTUAL variable (its memory address).
// Changes inside the function DIRECTLY modify the caller's variable.
void incrementByReference(int &num) {
    // The '&' means "num" is an alias (reference) to the original variable.
    // No copy is made - we are working on the real thing.
    num = num + 1;   // This changes the caller's variable permanently
    cout << "   Inside incrementByReference: num is now " << num << "\n";
}

// PASS BY CONST REFERENCE (very important for efficiency)
// Used when you want to READ a large object without copying it, but promise
// not to modify it. The 'const' prevents accidental modification.
void printVectorSize(const vector<int> &v) {
    // No copy of the vector is made (very fast even for huge vectors).
    // We can only READ from v, cannot change it.
    cout << "   Vector has " << v.size() << " elements (passed by const ref)\n";
}

// -----------------------------------------------------------------------------
// 3. DEFAULT ARGUMENTS
// -----------------------------------------------------------------------------

// Default arguments allow you to omit some parameters when calling.
// The compiler automatically uses the default value if you don't provide one.
// RULE: All default arguments must be at the END (rightmost parameters).
void greet(string name = "World", string greeting = "Hello") {
    // If caller doesn't pass anything, both defaults are used.
    // If caller passes one argument, only 'name' is set, greeting uses default.
    cout << greeting << ", " << name << "!\n";
}

// Another example with default - useful in competitive programming
int power(int base, int exp = 2) {
    // Default exponent is 2 → computes square if exp not provided
    int result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

// -----------------------------------------------------------------------------
// 4. FUNCTION OVERLOADING
// -----------------------------------------------------------------------------

// Function overloading = same function name, different parameter lists.
// The compiler chooses which version to call based on the arguments you pass.
// This makes code more intuitive (you don't need printInt, printDouble, etc.)

void display(int value) {
    cout << "Integer version called: " << value << "\n";
}

void display(double value) {
    cout << "Double version called: " << value << "\n";
}

void display(const string &value) {
    cout << "String version called: " << value << "\n";
}

void display(int a, int b) {
    // Overloaded with TWO parameters
    cout << "Two integers version: " << a << " and " << b << "\n";
}

// -----------------------------------------------------------------------------
// 5. INLINE FUNCTIONS
// -----------------------------------------------------------------------------

// The 'inline' keyword is a REQUEST to the compiler to replace the function
// call with the actual code at compile time (instead of jumping to the function).
//
// Benefits:
//   - Eliminates function call overhead (especially for tiny functions)
//   - Can enable better compiler optimizations
//
// When to use:
//   - Small, simple, frequently called functions
//   - Not for large or complex logic (code bloat)
//
// Note: Modern compilers are smart. They often inline automatically even
// without the keyword. 'inline' is just a hint.

inline int square(int x) {
    return x * x;
}

inline int minOfTwo(int a, int b) {
    return (a < b) ? a : b;
}

// -----------------------------------------------------------------------------
// 6. RECURSION
// -----------------------------------------------------------------------------

// Recursion = a function that calls itself.
// Every recursive function MUST have two parts:
//   1. BASE CASE: condition where recursion stops (very important!)
//   2. RECURSIVE CASE: function calls itself with a smaller/simpler problem

// CLASSIC EXAMPLE 1: Factorial
// n! = n * (n-1) * (n-2) * ... * 1
// 0! = 1 (by definition)
int factorial(int n) {
    // BASE CASE - without this the function would call itself forever (stack overflow)
    if (n <= 1) {
        return 1;
    }
    // RECURSIVE CASE
    // We break the problem into a smaller version of the same problem
    return n * factorial(n - 1);
}

// CLASSIC EXAMPLE 2: Fibonacci (naive recursive version)
// fib(0) = 0, fib(1) = 1
// fib(n) = fib(n-1) + fib(n-2)
int fibonacci(int n) {
    if (n <= 1) {
        return n;   // base cases: fib(0)=0, fib(1)=1
    }
    // This is beautiful but EXTREMELY inefficient for large n
    // Time complexity: O(2^n) - exponential! (many repeated calculations)
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// CLASSIC EXAMPLE 3: Sum of first n natural numbers
int sumUpTo(int n) {
    if (n <= 0) {
        return 0;
    }
    return n + sumUpTo(n - 1);
}

// CLASSIC EXAMPLE 4: Power function using recursion
long long powerRecursive(long long base, int exp) {
    if (exp == 0) {
        return 1;           // base^0 = 1
    }
    if (exp < 0) {
        return 0;           // not handling negative exponents properly here
    }
    return base * powerRecursive(base, exp - 1);
}

// =============================================================================
// MAIN FUNCTION - DEMONSTRATIONS AND EXPLANATIONS
// =============================================================================

int main() {
    cout << "=================================================================\n";
    cout << "         C++ FUNCTIONS - COMPLETE TEACHING DEMONSTRATION\n";
    cout << "=================================================================\n\n";

    // ========================================================================
    // SECTION 1: FUNCTION DECLARATION, DEFINITION, AND CALLING
    // ========================================================================
    cout << "=== SECTION 1: Declaration, Definition, Calling ===\n\n";

    // Calling a function that was declared earlier (prototype) and defined above
    int result = add(5, 7);
    cout << "add(5, 7) = " << result << "\n";

    // You can also call functions with expressions
    cout << "add(10+2, 3*4) = " << add(10 + 2, 3 * 4) << "\n\n";

    sayHello();

    printSum(15, 25);
    cout << "\n";

    // ========================================================================
    // SECTION 2: PASS BY VALUE vs PASS BY REFERENCE
    // ========================================================================
    cout << "=== SECTION 2: Pass by Value vs Pass by Reference ===\n\n";

    int original = 10;
    cout << "Before any calls: original = " << original << "\n\n";

    cout << "Calling incrementByValue(original):\n";
    incrementByValue(original);
    cout << "After incrementByValue: original is STILL " << original
         << " (unchanged because of pass by VALUE)\n\n";

    cout << "Calling incrementByReference(original):\n";
    incrementByReference(original);
    cout << "After incrementByReference: original is now " << original
         << " (changed because of pass by REFERENCE)\n\n";

    // Demonstrating const reference with a vector
    vector<int> numbers = {1, 2, 3, 4, 5};
    printVectorSize(numbers);

    cout << "\nKey takeaway:\n";
    cout << "  - Pass by value = safe but slow for big data (copies everything)\n";
    cout << "  - Pass by reference = fast, can modify original (use &)\n";
    cout << "  - Pass by const reference = fast + safe (use const &)\n\n";

    // ========================================================================
    // SECTION 3: DEFAULT ARGUMENTS
    // ========================================================================
    cout << "=== SECTION 3: Default Arguments ===\n\n";

    greet();                           // uses both defaults
    greet("Alice");                    // uses default greeting
    greet("Bob", "Good morning");      // provides both

    cout << "\nPower examples (default exponent = 2):\n";
    cout << "power(5)     = " << power(5) << "   (5 squared)\n";
    cout << "power(5, 3)  = " << power(5, 3) << "\n";
    cout << "power(2, 10) = " << power(2, 10) << "\n\n";

    // ========================================================================
    // SECTION 4: FUNCTION OVERLOADING
    // ========================================================================
    cout << "=== SECTION 4: Function Overloading ===\n\n";

    display(42);           // calls the int version
    display(3.14159);      // calls the double version
    display("Hello");      // calls the string version
    display(7, 9);         // calls the two-int version

    cout << "\nOverloading lets you use the same name for related operations.\n";
    cout << "Compiler picks the best match based on argument types.\n\n";

    // ========================================================================
    // SECTION 5: INLINE FUNCTIONS
    // ========================================================================
    cout << "=== SECTION 5: Inline Functions ===\n\n";

    int val = 6;
    cout << "square(" << val << ") = " << square(val) << "\n";
    cout << "minOfTwo(15, 9) = " << minOfTwo(15, 9) << "\n\n";

    cout << "Inline functions are great for tiny, hot-path utilities.\n";
    cout << "They remove the cost of a function call for very small operations.\n\n";

    // ========================================================================
    // SECTION 6: RECURSION
    // ========================================================================
    cout << "=== SECTION 6: Recursion ===\n\n";

    // Factorial
    cout << "Factorial examples:\n";
    for (int i = 0; i <= 7; ++i) {
        cout << "factorial(" << i << ") = " << factorial(i) << "\n";
    }

    // Fibonacci (warning about inefficiency)
    cout << "\nFibonacci (naive recursion - only small numbers!):\n";
    for (int i = 0; i <= 10; ++i) {
        cout << "fibonacci(" << i << ") = " << fibonacci(i) << "\n";
    }
    cout << "\nWARNING: fibonacci(40) would take a very long time with this method!\n";
    cout << "         (exponential time complexity due to massive recomputation)\n\n";

    // Sum using recursion
    cout << "sumUpTo(10) = " << sumUpTo(10) << "\n\n";

    // Recursive power
    cout << "powerRecursive(2, 10) = " << powerRecursive(2, 10) << "\n\n";

    // ========================================================================
    // FINAL SUMMARY
    // ========================================================================
    cout << "=================================================================\n";
    cout << "                        QUICK REFERENCE\n";
    cout << "=================================================================\n";
    cout << "Function declaration:   returnType name(parameters);\n";
    cout << "Function definition:    returnType name(parameters) { body }\n";
    cout << "Pass by value:          void f(int x)         // copy\n";
    cout << "Pass by reference:      void f(int &x)        // modify original\n";
    cout << "Const reference:        void f(const int &x)  // read only, no copy\n";
    cout << "Default args:           void f(int x = 10)\n";
    cout << "Overloading:            multiple f() with different parameter lists\n";
    cout << "Inline:                 inline int f() { ... }\n";
    cout << "Recursion:              function calls itself + MUST have base case\n";
    cout << "=================================================================\n";

    return 0;
}
