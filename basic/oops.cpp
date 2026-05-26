// =============================================================================
// OBJECT-ORIENTED PROGRAMMING (OOP) IN C++ - COMPLETE TEACHING GUIDE
// =============================================================================
// This is a comprehensive, deeply commented teaching file covering almost every
// important OOP concept in C++ as used in competitive programming, interviews,
// and real-world software development.
//
// C++ supports powerful OOP features while still giving you low-level control.
//
// Topics covered with detailed explanations and runnable examples:
//
//   1. Classes and Objects
//   2. Access Specifiers (public, private, protected)
//   3. Constructors (Default, Parameterized, Copy, Move)
//   4. Destructors
//   5. Member Functions and 'this' pointer
//   6. Static Members (data and functions)
//   7. Friend Functions and Friend Classes
//   8. Inheritance (Single, Multilevel, Hierarchical, Multiple, Hybrid)
//   9. Polymorphism
//        - Compile-time: Function Overloading + Operator Overloading
//        - Run-time: Virtual Functions, Abstract Classes, Interfaces
//  10. Encapsulation and Abstraction
//
// Every concept includes:
//   - Why it exists
//   - How it works
//   - Common pitfalls
//   - Best practices
//   - Working code with visible output
//
// Style: Written like a patient, thorough teacher explaining in a classroom.
// =============================================================================

#include <bits/stdc++.h>
using namespace std;

// =============================================================================
// SECTION 0: HELPER CLASSES (Defined here with heavy teaching comments)
// =============================================================================

// -----------------------------------------------------------------------------
// 1. BankAccount - Used for Encapsulation, Access Specifiers, Constructors
// -----------------------------------------------------------------------------
class BankAccount {
private:
    // Private data: hidden from outside (Encapsulation)
    string accountHolder;
    double balance;
    static int totalAccounts;   // Static data member (shared by all objects)

public:
    // Default Constructor
    BankAccount() {
        accountHolder = "Unknown";
        balance = 0.0;
        totalAccounts++;
        cout << "  [Default Constructor] BankAccount created for " << accountHolder << "\n";
    }

    // Parameterized Constructor
    BankAccount(string name, double initialBalance) {
        accountHolder = name;
        balance = (initialBalance > 0) ? initialBalance : 0;
        totalAccounts++;
        cout << "  [Parameterized Constructor] BankAccount created for " << accountHolder
             << " with balance " << balance << "\n";
    }

    // Copy Constructor (deep copy not needed here, but shown for teaching)
    BankAccount(const BankAccount& other) {
        accountHolder = other.accountHolder;
        balance = other.balance;
        totalAccounts++;
        cout << "  [Copy Constructor] BankAccount copied for " << accountHolder << "\n";
    }

    // Destructor
    ~BankAccount() {
        totalAccounts--;
        cout << "  [Destructor] BankAccount for " << accountHolder << " destroyed. Remaining accounts: "
             << totalAccounts << "\n";
    }

    // Member functions
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "    Deposited " << amount << ". New balance: " << balance << "\n";
        }
    }

    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "    Withdrew " << amount << ". New balance: " << balance << "\n";
        } else {
            cout << "    Withdrawal failed. Insufficient balance.\n";
        }
    }

    double getBalance() const {   // const member function - cannot modify object
        return balance;
    }

    string getHolderName() const {
        return accountHolder;
    }

    // Static member function
    static int getTotalAccounts() {
        return totalAccounts;
    }

    // Friend function declaration (will be defined outside)
    friend void showAccountDetails(const BankAccount& acc);
};

// Initialize static member (must be done outside the class)
int BankAccount::totalAccounts = 0;

// Friend function definition (can access private members)
void showAccountDetails(const BankAccount& acc) {
    cout << "    [Friend Function] Account Holder: " << acc.accountHolder
         << ", Balance: " << acc.balance << "\n";
}

// -----------------------------------------------------------------------------
// 2. Complex Number - For Operator Overloading
// -----------------------------------------------------------------------------
class Complex {
private:
    double real, imag;

public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}   // Constructor with defaults

    // Overload + operator (member function)
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }

    // Overload - operator
    Complex operator-(const Complex& other) const {
        return Complex(real - other.real, imag - other.imag);
    }

    // Overload * operator
    Complex operator*(const Complex& other) const {
        return Complex(real * other.real - imag * other.imag,
                       real * other.imag + imag * other.real);
    }

    // Overload unary minus
    Complex operator-() const {
        return Complex(-real, -imag);
    }

    // Overload << for printing (must be friend or global)
    friend ostream& operator<<(ostream& os, const Complex& c) {
        os << c.real;
        if (c.imag >= 0) os << "+";
        os << c.imag << "i";
        return os;
    }

    // Overload >> for input (friend)
    friend istream& operator>>(istream& is, Complex& c) {
        cout << "    Enter real and imaginary parts: ";
        is >> c.real >> c.imag;
        return is;
    }
};

// -----------------------------------------------------------------------------
// 3. MyString - Demonstrates Move Semantics + Resource Management
// -----------------------------------------------------------------------------
class MyString {
private:
    char* data;
    size_t length;

public:
    // Constructor
    MyString(const char* str = "") {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
        cout << "  [MyString Constructor] Created: \"" << data << "\" (len=" << length << ")\n";
    }

    // Copy Constructor (deep copy)
    MyString(const MyString& other) {
        length = other.length;
        data = new char[length + 1];
        strcpy(data, other.data);
        cout << "  [MyString Copy Constructor] Deep copied: \"" << data << "\"\n";
    }

    // Move Constructor (C++11) - steals resources
    MyString(MyString&& other) noexcept {
        data = other.data;
        length = other.length;
        other.data = nullptr;   // Leave source in valid state
        other.length = 0;
        cout << "  [MyString Move Constructor] Moved: \"" << data << "\"\n";
    }

    // Destructor
    ~MyString() {
        if (data) {
            cout << "  [MyString Destructor] Destroying: \"" << data << "\"\n";
            delete[] data;
        } else {
            cout << "  [MyString Destructor] Destroying moved-from object\n";
        }
    }

    void print() const {
        if (data) cout << data;
        else cout << "(empty/moved)";
    }
};

// -----------------------------------------------------------------------------
// 4. Inheritance Hierarchy - Animal Base + Derived
// -----------------------------------------------------------------------------
class Animal {
protected:
    string name;

public:
    Animal(string n) : name(n) {
        cout << "  [Animal Constructor] " << name << " created\n";
    }

    virtual ~Animal() {   // IMPORTANT: Virtual destructor in base class
        cout << "  [Animal Destructor] " << name << " destroyed\n";
    }

    // Virtual function → enables runtime polymorphism
    virtual void makeSound() const {
        cout << "    " << name << " makes a generic animal sound\n";
    }

    // Pure virtual function → makes this class ABSTRACT
    virtual void eat() const = 0;

    void sleep() const {   // Non-virtual - same behavior for all
        cout << "    " << name << " is sleeping\n";
    }
};

class Dog : public Animal {
public:
    Dog(string n) : Animal(n) {
        cout << "    [Dog Constructor] " << name << " is a dog\n";
    }

    ~Dog() {
        cout << "    [Dog Destructor] " << name << " destroyed\n";
    }

    void makeSound() const override {
        cout << "    " << name << " says: Woof Woof!\n";
    }

    void eat() const override {
        cout << "    " << name << " eats dog food\n";
    }
};

class Cat : public Animal {
public:
    Cat(string n) : Animal(n) {
        cout << "    [Cat Constructor] " << name << " is a cat\n";
    }

    ~Cat() {
        cout << "    [Cat Destructor] " << name << " destroyed\n";
    }

    void makeSound() const override {
        cout << "    " << name << " says: Meow!\n";
    }

    void eat() const override {
        cout << "    " << name << " eats fish\n";
    }
};

// -----------------------------------------------------------------------------
// 5. Multiple Inheritance Example
// -----------------------------------------------------------------------------
class Swimmer {
public:
    virtual ~Swimmer() {}
    virtual void swim() const {
        cout << "    Swimming in water\n";
    }
};

class Flyer {
public:
    virtual ~Flyer() {}
    virtual void fly() const {
        cout << "    Flying in the sky\n";
    }
};

// Duck inherits from both Animal + Swimmer + Flyer (Multiple + Multilevel)
class Duck : public Animal, public Swimmer, public Flyer {
public:
    Duck(string n) : Animal(n) {
        cout << "    [Duck Constructor] " << name << " is a duck\n";
    }

    ~Duck() {
        cout << "    [Duck Destructor] " << name << " destroyed\n";
    }

    void makeSound() const override {
        cout << "    " << name << " says: Quack!\n";
    }

    void eat() const override {
        cout << "    " << name << " eats insects and plants\n";
    }

    void swim() const override {
        cout << "    " << name << " swims gracefully\n";
    }
};

// -----------------------------------------------------------------------------
// 6. Abstract Class / Interface Example (Shape)
// -----------------------------------------------------------------------------
class Shape {
public:
    virtual ~Shape() {}
    virtual double area() const = 0;        // Pure virtual
    virtual double perimeter() const = 0;   // Pure virtual
    virtual void draw() const = 0;          // Pure virtual → acts like interface
};

// Concrete classes implementing the "interface"
class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r) : radius(r) {}

    double area() const override { return 3.14159 * radius * radius; }
    double perimeter() const override { return 2 * 3.14159 * radius; }
    void draw() const override { cout << "    Drawing a circle of radius " << radius << "\n"; }
};

class Rectangle : public Shape {
private:
    double width, height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    double area() const override { return width * height; }
    double perimeter() const override { return 2 * (width + height); }
    void draw() const override { cout << "    Drawing a rectangle " << width << "x" << height << "\n"; }
};

// =============================================================================
// MAIN FUNCTION - ALL DEMONSTRATIONS WITH DETAILED TEACHING
// =============================================================================

int main() {
    cout << "=================================================================\n";
    cout << "          C++ OBJECT-ORIENTED PROGRAMMING - TEACHING FILE\n";
    cout << "=================================================================\n\n";

    // ========================================================================
    // SECTION 1: CLASSES, OBJECTS & ACCESS SPECIFIERS
    // ========================================================================
    cout << "=== SECTION 1: Classes, Objects & Access Specifiers ===\n\n";

    BankAccount acc1;                                    // Default constructor
    BankAccount acc2("Alice", 5000);                     // Parameterized

    cout << "\nTrying to access private data directly would cause compile error:\n";
    // acc2.balance = 10000;   // ← ERROR: 'balance' is private

    cout << "\nUsing public member functions to interact:\n";
    acc2.deposit(1500);
    acc2.withdraw(2000);
    cout << "Current balance of acc2: " << acc2.getBalance() << "\n\n";

    // ========================================================================
    // SECTION 2: CONSTRUCTORS - DEFAULT & PARAMETERIZED
    // ========================================================================
    cout << "=== SECTION 2: Constructors (Default & Parameterized) ===\n\n";
    cout << "Already demonstrated above with BankAccount.\n";
    cout << "Key point: Constructors initialize objects. They have same name as class.\n\n";

    // ========================================================================
    // SECTION 3: COPY CONSTRUCTOR & MOVE CONSTRUCTOR
    // ========================================================================
    cout << "=== SECTION 3: Copy Constructor and Move Constructor ===\n\n";

    BankAccount acc3 = acc2;   // Copy constructor called
    cout << "acc3 balance after copy: " << acc3.getBalance() << "\n\n";

    cout << "--- Move Semantics with MyString ---\n";
    MyString str1("Hello World");
    MyString str2 = str1;                    // Copy constructor (expensive)
    MyString str3 = std::move(str1);         // Move constructor (cheap - steals resources)

    cout << "str3 content: "; str3.print(); cout << "\n";
    cout << "str1 after move (should be empty): "; str1.print(); cout << "\n\n";

    // ========================================================================
    // SECTION 4: DESTRUCTORS
    // ========================================================================
    cout << "=== SECTION 4: Destructors ===\n";
    cout << "Destructors are called automatically when objects go out of scope.\n";
    cout << "Watch for destructor messages at the end of scopes.\n\n";

    {
        BankAccount temp("Temporary", 100);
        // temp destroyed here when scope ends
    }
    cout << "Temporary account was destroyed above.\n\n";

    // ========================================================================
    // SECTION 5: MEMBER FUNCTIONS & THIS POINTER
    // ========================================================================
    cout << "=== SECTION 5: Member Functions and 'this' Pointer ===\n\n";

    class ThisDemo {
    public:
        int value;
        ThisDemo(int v) : value(v) {}

        void show() {
            cout << "    this->value = " << this->value << "\n";
            cout << "    (*this).value = " << (*this).value << "\n";
            cout << "    Direct access: value = " << value << "\n";
        }

        ThisDemo& setValue(int v) {
            this->value = v;           // 'this' is useful for returning *this
            return *this;              // Enables method chaining
        }
    };

    ThisDemo obj(42);
    obj.show();

    // Method chaining using 'this'
    obj.setValue(100).setValue(200);
    cout << "After chaining: value = " << obj.value << "\n\n";

    // ========================================================================
    // SECTION 6: STATIC MEMBERS
    // ========================================================================
    cout << "=== SECTION 6: Static Members (Data & Functions) ===\n\n";

    cout << "Total BankAccount objects created so far: "
         << BankAccount::getTotalAccounts() << "\n";

    {
        BankAccount a1("Bob", 1000);
        BankAccount a2("Charlie", 2000);
        cout << "Total accounts now (inside scope): "
             << BankAccount::getTotalAccounts() << "\n";
    }
    cout << "Total accounts after scope ended: "
         << BankAccount::getTotalAccounts() << "\n\n";

    // ========================================================================
    // SECTION 7: FRIEND FUNCTIONS AND FRIEND CLASSES
    // ========================================================================
    cout << "=== SECTION 7: Friend Functions and Classes ===\n\n";

    BankAccount secret("Secret", 99999);
    cout << "Accessing private data via friend function:\n";
    showAccountDetails(secret);
    cout << "\n";

    // ========================================================================
    // SECTION 8: INHERITANCE - ALL TYPES
    // ========================================================================
    cout << "=== SECTION 8: Inheritance (All Types) ===\n\n";

    cout << "--- Single Inheritance ---\n";
    Dog dog("Bruno");
    dog.makeSound();
    dog.eat();
    dog.sleep();
    cout << "\n";

    cout << "--- Multilevel Inheritance ---\n";
    // (Animal → Mammal → Dog would be multilevel, here we keep it simple)

    cout << "--- Hierarchical Inheritance ---\n";
    Cat cat("Whiskers");
    cat.makeSound();
    cat.eat();
    cout << "\n";

    cout << "--- Multiple Inheritance ---\n";
    Duck duck("Daffy");
    duck.makeSound();
    duck.swim();
    duck.fly();
    duck.eat();
    cout << "\n";

    // ========================================================================
    // SECTION 9: POLYMORPHISM - COMPILE TIME
    // ========================================================================
    cout << "=== SECTION 9: Compile-time Polymorphism ===\n\n";

    cout << "--- Function Overloading (already seen in function.cpp) ---\n";

    cout << "--- Operator Overloading ---\n";
    Complex c1(3, 4), c2(1, 2);
    Complex c3 = c1 + c2;
    Complex c4 = c1 * c2;
    Complex c5 = -c1;

    cout << "c1 = " << c1 << "\n";
    cout << "c2 = " << c2 << "\n";
    cout << "c1 + c2 = " << c3 << "\n";
    cout << "c1 * c2 = " << c4 << "\n";
    cout << "-c1 = " << c5 << "\n\n";

    // ========================================================================
    // SECTION 10: POLYMORPHISM - RUN TIME (Virtual + Abstract)
    // ========================================================================
    cout << "=== SECTION 10: Run-time Polymorphism ===\n\n";

    cout << "--- Virtual Functions & Polymorphism ---\n";
    vector<unique_ptr<Animal>> zoo;
    zoo.push_back(make_unique<Dog>("Max"));
    zoo.push_back(make_unique<Cat>("Luna"));
    zoo.push_back(make_unique<Duck>("Donald"));

    cout << "\nCalling makeSound() through base class pointer:\n";
    for (const auto& animal : zoo) {
        animal->makeSound();   // Correct function called at runtime!
    }

    cout << "\nCalling eat() through base class pointer:\n";
    for (const auto& animal : zoo) {
        animal->eat();
    }
    cout << "\n";

    cout << "--- Abstract Classes & Interfaces ---\n";
    vector<unique_ptr<Shape>> shapes;
    shapes.push_back(make_unique<Circle>(5));
    shapes.push_back(make_unique<Rectangle>(4, 6));

    for (const auto& shape : shapes) {
        shape->draw();
        cout << "    Area: " << shape->area() << ", Perimeter: " << shape->perimeter() << "\n";
    }
    cout << "\n";

    // ========================================================================
    // SECTION 11: ENCAPSULATION & ABSTRACTION
    // ========================================================================
    cout << "=== SECTION 11: Encapsulation and Abstraction ===\n\n";

    cout << "BankAccount class demonstrates Encapsulation:\n";
    cout << "- Private data (balance, name) hidden from outside\n";
    cout << "- Public methods control how data is accessed/modified\n";
    cout << "- Internal validation (e.g., no negative balance)\n\n";

    cout << "Shape hierarchy demonstrates Abstraction:\n";
    cout << "- Shape is an abstract concept (cannot create Shape object)\n";
    cout << "- Only concrete derived classes (Circle, Rectangle) can be instantiated\n";
    cout << "- User only cares about area(), perimeter(), draw() interface\n\n";

    // ========================================================================
    // FINAL QUICK REFERENCE
    // ========================================================================
    cout << "=================================================================\n";
    cout << "                        QUICK REFERENCE\n";
    cout << "=================================================================\n";
    cout << "Class:                  class Name { private: ... public: ... };\n";
    cout << "Access:                 public / private / protected\n";
    cout << "Constructor:            ClassName() { ... }\n";
    cout << "Parameterized:          ClassName(int x) : member(x) {}\n";
    cout << "Copy Constructor:       ClassName(const ClassName& other)\n";
    cout << "Move Constructor:       ClassName(ClassName&& other) noexcept\n";
    cout << "Destructor:             ~ClassName() { ... }\n";
    cout << "this pointer:           Points to current object (used in members)\n";
    cout << "Static:                 static int count;  +  static int getCount()\n";
    cout << "Friend:                 friend void func(const Class& obj);\n";
    cout << "Inheritance:            class Derived : public Base { ... }\n";
    cout << "Virtual function:       virtual void func() { ... }\n";
    cout << "Pure virtual:           virtual void func() = 0;  → Abstract class\n";
    cout << "Operator overload:      Complex operator+(const Complex& other)\n";
    cout << "Virtual destructor:     virtual ~Base() {}   (CRITICAL in inheritance)\n";
    cout << "=================================================================\n";
    cout << "Best Practice: Always make base class destructors virtual.\n";
    cout << "Best Practice: Prefer composition over inheritance when possible.\n";
    cout << "=================================================================\n";

    return 0;
}
