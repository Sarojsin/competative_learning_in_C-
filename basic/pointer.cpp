// =============================================================================
// POINTERS AND REFERENCES IN C++ - COMPLETE TEACHING GUIDE
// =============================================================================
// This file teaches pointers, references, and modern smart pointers from the
// ground up, exactly as needed for competitive programming and real C++ work.
//
// Pointers are one of the most powerful (and dangerous) features in C++.
// Mastering them is essential for understanding memory, performance, and
// how many data structures and libraries actually work.
//
// Topics covered (all with deep explanations):
//   1. Pointer declaration, initialization, address-of (&) and dereference (*)
//   2. Dereferencing and modifying values through pointers
//   3. Pointer arithmetic (++, +, -, subtraction)
//   4. Pointers to arrays and array-to-pointer decay
//   5. Pointers to functions (including typedef/using for cleaner syntax)
//   6. nullptr, NULL, and the null pointer constant
//   7. Dynamic memory allocation with new / delete and new[] / delete[]
//   8. Common pitfalls (dangling pointers, double delete, memory leaks)
//   9. References vs Pointers - when to use which
//  10. Modern C++11 Smart Pointers:
//        - std::unique_ptr     (exclusive ownership)
//        - std::shared_ptr     (shared ownership + reference counting)
//        - std::weak_ptr       (breaking cycles, observing without owning)
//
// Style: Every concept is explained like a patient teacher in a classroom.
//        Code examples are runnable and produce visible output.
// =============================================================================

#include <bits/stdc++.h>
// bits/stdc++.h brings in everything we need (iostream, vector, memory, etc.)

using namespace std;

// =============================================================================
// HELPER FUNCTIONS FOR DEMONSTRATIONS
// =============================================================================

// Simple functions used to demonstrate "pointers to functions"
int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

int subtract(int a, int b) {
    return a - b;
}

// A small class to demonstrate smart pointers with objects
class Student {
public:
    string name;
    int age;

    Student(string n, int a) : name(n), age(a) {
        cout << "  [Student constructor] " << name << " created\n";
    }

    ~Student() {
        cout << "  [Student destructor]  " << name << " destroyed\n";
    }

    void introduce() const {
        cout << "    Hi, I'm " << name << ", age " << age << "\n";
    }
};

// =============================================================================
// MAIN - ALL TEACHING DEMONSTRATIONS
// =============================================================================

int main() {
    cout << "=================================================================\n";
    cout << "     C++ POINTERS & REFERENCES - COMPLETE TEACHING DEMONSTRATION\n";
    cout << "=================================================================\n\n";

    // ========================================================================
    // SECTION 1: POINTER DECLARATION, INITIALIZATION, & AND *
    // ========================================================================
    cout << "=== SECTION 1: Pointer Declaration & Basics ===\n\n";

    int number = 42;
    cout << "Original variable: number = " << number << "\n";
    cout << "Address of number: " << &number << " (this is a memory address)\n\n";
    // Declaration of a pointer:
    // int* ptr;   means "ptr is a pointer to an int"
    // The * is part of the type: "pointer to int"
    int* ptr = &number;   // ptr now stores the address of 'number'

    cout << "Pointer ptr holds the address: " << ptr << "\n";
    cout << "Size of pointer on this system: " << sizeof(ptr) << " bytes\n\n";

    // The & operator = "address-of"
    // The * operator = "dereference" (follow the pointer to the value)

    // ========================================================================
    // SECTION 2: DEREFERENCING - READING AND WRITING THROUGH POINTERS
    // ========================================================================
    cout << "=== SECTION 2: Dereferencing (* operator) ===\n\n";

    cout << "Value at the address stored in ptr (*ptr) = " << *ptr << "\n\n";

    // We can READ and WRITE through the pointer
    cout << "Changing the value THROUGH the pointer...\n";
    *ptr = 100;   // This actually changes 'number'!

    cout << "After *ptr = 100:\n";
    cout << "  number = " << number << "\n";
    cout << "  *ptr   = " << *ptr << "\n\n";

    // Important: *ptr and number are now two names for the SAME memory location

    // ========================================================================
    // SECTION 3: POINTER ARITHMETIC
    // ========================================================================
    cout << "=== SECTION 3: Pointer Arithmetic ===\n\n";

    int arr[5] = {10, 20, 30, 40, 50};

    int* p = arr;   // points to first element (arr decays to pointer)

    cout << "Array: 10 20 30 40 50\n";
    cout << "p points to first element: *p = " << *p << "\n";

    // Pointer arithmetic: adding/subtracting integers moves by ELEMENT size
    p = p + 1;                    // move forward by 1 int (usually 4 bytes)
    cout << "After p = p + 1: *p = " << *p << "\n";

    p += 2;                       // move forward by 2 more elements
    cout << "After p += 2: *p = " << *p << "\n";

    p--;                          // go back one element
    cout << "After p--: *p = " << *p << "\n\n";

    // Pointer subtraction: gives the NUMBER OF ELEMENTS between them
    int* start = arr;
    int* end = arr + 4;
    ptrdiff_t distance = end - start;   // ptrdiff_t is the signed type for differences
    cout << "Distance between start and end pointers: " << distance << " elements\n\n";

    // WARNING: Pointer arithmetic only makes sense within the same array/object!
    // Going outside array bounds is undefined behavior.

    // ========================================================================
    // SECTION 4: POINTERS TO ARRAYS + ARRAY DECAY
    // ========================================================================
    cout << "=== SECTION 4: Pointers to Arrays & Array Decay ===\n\n";

    int scores[4] = {85, 92, 78, 95};

    // In almost all contexts, an array name "decays" to a pointer to its first element
    int* scorePtr = scores;           // equivalent to &scores[0]

    cout << "Accessing array via pointer:\n";
    for (int i = 0; i < 4; ++i) {
        // Two ways to access the same element:
        cout << "  scores[" << i << "] = " << scores[i]<< "   or   *(scorePtr + " << i << ") = " << *(scorePtr + i) << "\n";
    }

    // You can also use array subscript on a pointer (they are interchangeable here)
    cout << "\nUsing pointer with [] syntax: scorePtr[2] = " << scorePtr[2] << "\n\n";

    // ========================================================================
    // SECTION 5: POINTERS TO FUNCTIONS
    // ========================================================================
    cout << "=== SECTION 5: Pointers to Functions ===\n\n";

    // Syntax: returnType (*pointerName)(parameterTypes);
    int (*operation)(int, int);   // pointer to a function that takes two ints and returns int

    operation = add;              // assign address of add function
    cout << "operation = add → operation(5, 3) = " << operation(5, 3) << "\n";

    operation = multiply;
    cout << "operation = multiply → operation(5, 3) = " << operation(5, 3) << "\n";

    operation = subtract;
    cout << "operation = subtract → operation(5, 3) = " << operation(5, 3) << "\n\n";

    // Cleaner syntax using typedef or using (C++11)
    using MathFunc = int(*)(int, int);
    MathFunc func = multiply;
    cout << "Using 'using' alias: func(4, 7) = " << func(4, 7) << "\n\n";

    cout << "Function pointers are the foundation of callbacks, sorting comparators, etc.\n\n";

    // ========================================================================
    // SECTION 6: nullptr - THE MODERN NULL POINTER
    // ========================================================================
    cout << "=== SECTION 6: nullptr and Null Pointers ===\n\n";

    int* safePtr = nullptr;   // Modern C++ way (C++11)

    cout << "safePtr is set to nullptr (address 0 conceptually)\n";

    if (safePtr == nullptr) {
        cout << "  safePtr is null - safe to check before dereferencing\n";
    }

    // NEVER dereference a null pointer! This would crash (segmentation fault)
    // *safePtr = 99;   // ← DANGEROUS - undefined behavior

    // Historical note:
    //   NULL is usually defined as 0 or (void*)0 in C
    //   In modern C++ you should ALWAYS use nullptr instead of NULL or 0

    cout << "\nRule: Always initialize pointers. Prefer nullptr over uninitialized or NULL.\n\n";

    // ========================================================================
    // SECTION 7: DYNAMIC MEMORY - new AND delete
    // ========================================================================
    cout << "=== SECTION 7: Dynamic Memory Allocation (new / delete) ===\n\n";

    // 'new' allocates memory on the HEAP (free store) at runtime
    // Returns a pointer to the allocated memory
    int* dynamicInt = new int;           // allocates one uninitialized int
    *dynamicInt = 777;

    cout << "Allocated one int on heap: *dynamicInt = " << *dynamicInt << "\n";

    // You MUST manually free it with 'delete'
    delete dynamicInt;
    dynamicInt = nullptr;   // Good practice: nullify after delete

    cout << "Memory freed with delete. Pointer set back to nullptr.\n\n";

    // Allocating an ARRAY on the heap
    int size = 5;
    int* dynamicArray = new int[size];   // allocates 5 ints (uninitialized)

    for (int i = 0; i < size; ++i) {
        dynamicArray[i] = i * 10;
    }

    cout << "Dynamically allocated array: ";
    for (int i = 0; i < size; ++i) {
        cout << dynamicArray[i] << " ";
    }
    cout << "\n";

    // Must use delete[] for arrays allocated with new[]
    delete[] dynamicArray;
    dynamicArray = nullptr;

    cout << "Array memory freed with delete[]\n\n";

    // Allocating an object
    Student* s = new Student("Rahul", 21);
    s->introduce();
    delete s;   // destructor is called automatically
    s = nullptr;

    cout << "\n";

    // ========================================================================
    // SECTION 8: DANGEROUS PITFALLS (EXPLAINED - DO NOT REPRODUCE)
    // ========================================================================
    cout << "=== SECTION 8: Common Pointer Pitfalls (Educational Only) ===\n\n";

    cout << "1. MEMORY LEAK - forgetting to delete\n";
    cout << "   int* leak = new int[100000];\n";
    cout << "   // forgot delete[] leak;  → memory is lost until program ends\n\n";

    cout << "2. DANGLING POINTER - using after delete\n";
    cout << "   int* dang = new int(5);\n";
    cout << "   delete dang;\n";
    cout << "   // *dang = 10;  → undefined behavior! (use-after-free)\n\n";

    cout << "3. DOUBLE DELETE - deleting twice\n";
    cout << "   int* d = new int;\n";
    cout << "   delete d;\n";
    cout << "   // delete d;   → undefined behavior (often crashes)\n\n";

    cout << "4. FORGETTING [] when deleting arrays\n";
    cout << "   int* a = new int[10];\n";
    cout << "   delete a;   → undefined behavior (should be delete[] a)\n\n";

    cout << "Golden Rules:\n";
    cout << "  - Every new must have exactly one matching delete\n";
    cout << "  - Every new[] must have exactly one matching delete[]\n";
    cout << "  - Set pointer to nullptr after delete (defensive)\n";
    cout << "  - Never use a pointer after it has been deleted\n\n";

    // ========================================================================
    // SECTION 9: REFERENCES vs POINTERS
    // ========================================================================
    cout << "=== SECTION 9: References vs Pointers ===\n\n";

    int original = 50;

    // REFERENCE (alias)
    int& ref = original;   // ref is another name for original
    ref = 99;              // changes original directly

    cout << "After ref = 99 → original = " << original << "\n\n";

    // POINTER
    int* pOriginal = &original;
    *pOriginal = 123;

    cout << "After *pOriginal = 123 → original = " << original << "\n\n";

    // Key differences (explained in comments):

    /*
        REFERENCES                          |   POINTERS
        ------------------------------------|----------------------------------
        Must be initialized when declared   |   Can be declared uninitialized
        Cannot be reseated (always alias)   |   Can point to different objects
        Cannot be null                      |   Can be nullptr
        No special syntax after init        |   Need * and & operators
        Usually implemented as pointers     |   Explicit address manipulation
        Safer and cleaner for most uses     |   More flexible and powerful
    */

    cout << "Recommendation:\n";
    cout << "  - Prefer references when you can (cleaner, safer)\n";
    cout << "  - Use pointers when you need: optional values (nullptr), reseating,\n";
    cout << "    dynamic arrays, or when working with C APIs.\n\n";

    // ========================================================================
    // SECTION 10: SMART POINTERS (C++11) - THE MODERN SOLUTION
    // ========================================================================
    cout << "=== SECTION 10: Smart Pointers (C++11) ===\n\n";

    cout << "Smart pointers are wrappers that automatically manage memory using RAII.\n";
    cout << "When the smart pointer goes out of scope, it automatically deletes the object.\n";
    cout << "This eliminates almost all manual delete mistakes.\n\n";

    // ---------- unique_ptr ----------
    cout << "--- std::unique_ptr (Exclusive Ownership) ---\n";
    {
        // C++11 syntax (make_unique was added in C++14)
        unique_ptr<Student> uPtr(new Student("Priya", 20));
        uPtr->introduce();

        // unique_ptr cannot be copied (only moved)
        // unique_ptr<Student> copy = uPtr;  // COMPILE ERROR

        cout << "  unique_ptr will auto-delete when it goes out of scope\n";
    } // <-- destructor of Student is called here automatically!
    cout << "  (unique_ptr went out of scope → object destroyed)\n\n";

    // ---------- shared_ptr ----------
    cout << "--- std::shared_ptr (Shared Ownership) ---\n";
    {
        shared_ptr<Student> s1 = make_shared<Student>("Amit", 22);
        cout << "  use_count after s1 = " << s1.use_count() << "\n";

        shared_ptr<Student> s2 = s1;   // now two owners
        cout << "  use_count after s2 = s1 → " << s1.use_count() << "\n";

        s2->introduce();

        cout << "  Both s1 and s2 point to the same object\n";
    } // object is deleted ONLY when the LAST shared_ptr is destroyed
    cout << "  (Last shared_ptr destroyed → object deleted)\n\n";

    // ---------- weak_ptr ----------
    cout << "--- std::weak_ptr (Non-owning Observer) ---\n";
    {
        shared_ptr<Student> strong = make_shared<Student>("Neha", 19);
        weak_ptr<Student> weak = strong;   // weak_ptr does NOT increase ref count

        cout << "  strong.use_count() = " << strong.use_count() << "\n";

        if (auto temp = weak.lock()) {   // lock() gives a shared_ptr if object still alive
            cout << "  weak_ptr successfully locked the object\n";
            temp->introduce();
        }

        // weak_ptr is commonly used to break circular references
        // (e.g., parent-child relationships where both sides own each other)
    }
    cout << "  (Object destroyed. weak_ptr now points to nothing)\n\n";

    // ========================================================================
    // FINAL SUMMARY
    // ========================================================================
    cout << "=================================================================\n";
    cout << "                        QUICK REFERENCE\n";
    cout << "=================================================================\n";
    cout << "Declare pointer:        int* p = &x;\n";
    cout << "Dereference:            *p = 5;     or   int val = *p;\n";
    cout << "Pointer arithmetic:     p++, p += 3, p2 - p1\n";
    cout << "Array decay:            int* p = arr;   // same as &arr[0]\n";
    cout << "Function pointer:       int (*f)(int,int) = add;\n";
    cout << "Modern null:            nullptr   (never use NULL or 0)\n";
    cout << "Heap allocation:        new int, new int[10]\n";
    cout << "Heap deallocation:      delete ptr, delete[] arr\n";
    cout << "Reference:              int& r = x;   // alias, cannot reseat\n";
    cout << "unique_ptr (C++11):     unique_ptr<T> p(new T(args));\n";
    cout << "shared_ptr:             auto p = make_shared<T>(args);\n";
    cout << "weak_ptr:               weak_ptr<T> w = shared;\n";
    cout << "=================================================================\n";
    cout << "Remember: Smart pointers are almost always the right choice in modern C++.\n";
    cout << "=================================================================\n";

    return 0;
}
