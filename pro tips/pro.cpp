// ============================================================
//  PRO C++ TIPS — 31 Topics
//  Each topic shows ❌ Bad / Old Way  →  ✅ Good / Modern Way
// ============================================================
#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;   // ⚠️ Only for demonstration; normally avoid!

// TOPIC 1 — using namespace std in a header or source file
// ════════════════════════════════════════════════════════════
void topic1() {
    cout << "\n=== TOPIC 1: using namespace std ===\n";
    // ❌ Old way: pollutes the global namespace; name clashes in large projects
    using namespace std;   // brings EVERY std name into global scope

    // ⚠️ Hazards: names like count, reserve, distance, etc. may clash with
    //   project identifiers causing silent bugs.
    string s = "pulls";

    // ✅ Modern way: qualify names explicitly or use selective `using`
    std::string t = "pulls";              // fully qualified — safe
    using std::cout;                       // bring ONLY what you need
    cout << "Topic 1: selectively using std::cout is fine.\n";
}

// TOPIC 2 — using std::endl in a loop (forces flush every time)
// ════════════════════════════════════════════════════════════
void topic2() {
    cout << "\n=== TOPIC 2: std::endl in a loop ===\n";
    cout << "(Flush text intentionally omitted for brevity.)\n";
    // ❌ Old way: std::endl writes a newline AND flushes the stream — very slow in tight loops
    // for (int i = 0; i < 1'000'000; ++i)
    //     cout << i << std::endl;

    // ✅ Modern way: use '\n' for newline; flush explicitly only when needed
    // for (int i = 0; i < 1'000'000; ++i)
    //     cout << i << '\n';
    //
    // Rule of thumb:  std::endl = '\n' + flush()
    // std::flush  — explicit flush
    cout << "Use '\\n' inside loops; use std::endl only when you NEED a flush.\n";
}

// TOPIC 3 — index-based for when range-for fits better
// ════════════════════════════════════════════════════════════
void topic3() {
    cout << "\n=== TOPIC 3: index-based vs range-based for ===\n";
    vector<int> nums{10, 20, 30, 40};

    // ❌ Old way: manual index — verbose and error-prone
    int sum_old = 0;
    for (size_t i = 0; i < nums.size(); ++i)
        sum_old += nums[i];

    // ✅ Modern way: range-for — no off-by-one, no index arithmetic
    int sum_new = 0;
    for (int n : nums)
        sum_new += n;
    // Even better with std::accumulate (Topic 4)
    int sum_algo = accumulate(nums.begin(), nums.end(), 0);

    cout << "sums: old=" << sum_old << " new=" << sum_new << " algo=" << sum_algo << '\n';
}

// TOPIC 4 — rewriting std algorithms by hand
// ════════════════════════════════════════════════════════════
void topic4() {
    cout << "\n=== TOPIC 4: use <algorithm> instead of hand-rolled loops ===\n";
    vector<string> words{"the", "quick", "brown", "fox"};

    // ❌ Old way: hand-rolled loops to find / sort / count
    bool has_fox_old = false;
    for (auto &w : words)
        if (w == "fox") has_fox_old = true;
    (void)has_fox_old;  // intentionally unused — comparing approaches

    // ✅ Modern way: expressive, tested, optimised std algorithms
    bool has_fox_new  = any_of(words.begin(), words.end(),
                               [](const string &w){ return w == "fox"; });
    sort(words.begin(), words.end());
    auto fox_idx = find(words.begin(), words.end(), "fox");

    cout << "fox found? " << boolalpha << has_fox_new << "  pos="
         << distance(words.begin(), fox_idx) << '\n';
}

// TOPIC 5 — C-style array vs std::array
// ════════════════════════════════════════════════════════════
void topic5() {
    cout << "\n=== TOPIC 5: C array vs std::array ===\n";
    // ❌ Old way: C-style array — no size(), no begin()/end(), decays to ptr
    int c_arr[5] = {1, 2, 3, 4, 5};
    (void)c_arr;  // intentionally unused — illustration only
    // sizeof(c_arr) / sizeof(c_arr[0])   ← easy to get wrong
    // c_arr + 3                          ← pointer arithmetic ugliness

    // ✅ Modern way: std::array — STL-compatible, carries size, no decay
    array<int, 5> cpp_arr = {1, 2, 3, 4, 5};
    for (int x : cpp_arr) cout << x << ' ';
    cout << "| size=" << cpp_arr.size() << '\n';
}

// TOPIC 6 — reinterpret_cast safety
// ════════════════════════════════════════════════════════════
void topic6() {
    cout << "\n=== TOPIC 6: reinterpret_cast ===\n";
    int x = 0xDEADBEEF;

    // ❌ OLD / DANGEROUS: reinterpret_cast bypasses type safety.
    //     Platform-dependent, violates strict aliasing, UB.
    // uint32_t raw = *reinterpret_cast<uint32_t*>(&x);

    // ✅ SAFER ALTERNATIVE: memcpy for bit reinterpretation (defined behaviour)
    uint32_t safe;
    memcpy(&safe, &x, sizeof safe);   // portable bitwise copy
    cout << hex << "reinterpreted via memcpy: 0x" << safe << '\n';

    // ✅ When you MUST reinterpret: restrict scope and document
    //     e.g. hardware register mapping — use uintptr_t via std::bit_cast (C++20)
    // uintptr_t addr = std::bit_cast<uintptr_t>(&x);
}

// TOPIC 7 — casting away const
// ════════════════════════════════════════════════════════════
void topic7() {
    cout << "\n=== TOPIC 7: casting away const ===\n";
    const int ci = 42;

    // ❌ Old way: const_cast to drop const — undefined behaviour if original is truly const
    // int &mutable_ref = const_cast<int&>(ci);
    // mutable_ref = 99;          // ← RUNTIME CRASH on read-only memory; logical bug at minimum

    // ✅ Correct: accept const; only drop const when you KNOW the original is non-const
    int non_const = ci;           // normal copy — no cast needed
    (void) non_const;

    // ✅ If you really need low-level write access, use a const pointer cast to non-const
    //    **only** when aliased from a non-const object — and document WHY.
    cout << "const value: " << ci << ".  Don't cast away const.\n";
}

// TOPIC 8 — map[ ] inserts element if key not found
// ════════════════════════════════════════════════════════════
void topic8() {
    cout << "\n=== TOPIC 8: operator[] on const map inserts element ===\n";
    map<string, int> scores{{"alice", 10}};

    // ❌ Old way: operator[] inserts a default value if key is missing (zero-init here)
    // "bob" appears with score 0 even if not present — silent insertion!
    cout << "bob score before: " << scores["bob"] << '\n';   // prints 0, inserts "bob"

    // ✅ Modern way: use find / contains / at for read; emplace for insert
    auto it = scores.find("carol");
    if (it != scores.end())
        cout << "carol score: " << it->second << '\n';
    else
        cout << "carol not found — no unwanted insertion.\n";

    // C++20:
    if (scores.contains("alice"))
        cout << "alice found (C++20 contains)\n";
}

// TOPIC 9 — ignoring const-correctness
// ════════════════════════════════════════════════════════════
void print_sum_correct(const vector<int> &v) {
    (void)v;  // intentionally empty — compare signatures only
}

void topic9() {
    cout << "\n=== TOPIC 9: const-correctness ===\n";
    vector<int> data{1, 2, 3};

    // ❌ Old way: copying large containers unnecessarily
    // void print_sum(vector<int> v) { ... }
    // print_sum(data);   // copies entire vector every call

    // ✅ Modern way: pass by const reference
    // const vector<int>& v — or even const auto& for generic code
    (void) data;
    cout << "Pass large read-only objects as const& (or const auto&)\n";
}

// TOPIC 10 — string literal lifetime
// ════════════════════════════════════════════════════════════
void topic10() {
    cout << "\n=== TOPIC 10: string literal lifetime ===\n";
    // ❌ Old way: dangling pointer to a temporary
    // const char* bad = string("hello").c_str();   // c_str() dangles after full-expr

    // ✅ Safe: string literals have static storage duration — pointers are valid for program lifetime
    const char *lit = "hello, static";    // ✅ OK — static storage
    (void)lit;  // intentionally unused in this run

    // ✅ For C++ std::string, keep the string object alive
    string s = "hello, wrapped";          // s owns the buffer
    cout << "safe: " << s.c_str() << " (while s is alive)\n";
}

// TOPIC 11 — structured bindings (C++17)
// ════════════════════════════════════════════════════════════
void topic11() {
    cout << "\n=== TOPIC 11: structured bindings ===\n";
    map<string, int> inventory{{"apples", 5}, {"bananas", 12}};

    // ❌ Old way: explicit iterator / tie
    for (map<string,int>::const_iterator it = inventory.begin(); it != inventory.end(); ++it)
        cout << it->first << " => " << it->second << '\n';

    // ✅ Modern way: structured bindings decompose pairs, tuples, structs
    for (auto [fruit, count] : inventory)
        cout << fruit << " => " << count << '\n';

    // Also works on std::tuple
    tuple<int, string, double> t{1, "hello", 3.14};
    auto [num, txt, pi] = t;
    cout << "tuple: " << num << ' ' << txt << ' ' << pi << '\n';
}

// TOPIC 12 — out-params vs returning a struct
// ════════════════════════════════════════════════════════════
struct PersonInfo {
    string name;
    string email;
    int    age;
};

// ❌ Old way: out-parameter "tuple hack" — caller must pre-allocate, easy to forget
// bool get_person_info_old(int id, string &name_out, string &email_out, int &age_out);

// ✅ Modern way: return a small struct — trivially optimised (NRVO); call-site is clean
PersonInfo get_person_info(int id) {
    (void) id;
    return {"Alice", "alice@example.com", 30};
}

void topic12() {
    cout << "\n=== TOPIC 12: out-params vs returning a struct ===\n";
    PersonInfo p = get_person_info(1);
    cout << p.name << " <" << p.email << "> age=" << p.age << '\n';

    // For C++17+ structured bindings at call site:
    // auto [name, email, age] = get_person_info(1);
}

// TOPIC 13 — constexpr
// ════════════════════════════════════════════════════════════
// ❌ Old way: hard-coded magic values in runtime code
constexpr int MAX_USERS = 100;   // ✅ compile-time constant — inlined, zero runtime cost

// ❌ const int MAX_USERS = 100;  // still OK but not guaranteed compile-time eval everywhere

// ✅ constexpr functions — folded / evaluated at compile time
constexpr int square(int n) { return n * n; }

void topic13() {
    cout << "\n=== TOPIC 13: constexpr ===\n";
    // C++11: constexpr variables / functions
    // C++14: relaxed constexpr (loops, local vars allowed)
    // C++20: consteval (must be compile-time), constinit (must be initialised at compile-time)

    int arr[square(8)] = {};   // array size known at compile time
    cout << "square(8) = " << square(8) << "  (arr has " << square(8) << " elements)\n";
    (void)arr; // suppress unused-variable warning — arr is a compile-time size illustration

    // constinit guarantees static vars are initialised at compile time
    // (avoids static init order fiasco)
}

// TOPIC 14 — polymorphic base: virtual destructor
// ════════════════════════════════════════════════════════════
struct Base14 {
    // ❌ Missing virtual destructor → UB when deleting derived through Base*
    // ~Base14() { cout << "Base dtor\n"; }

    // ✅ Always declare destructor virtual in polymorphic base classes
    virtual ~Base14() { cout << "Base14 dtor\n"; }
    virtual void greet() const { cout << "Hello from Base14\n"; }
};

struct Derived14 : Base14 {
    string msg{"extra resource ctor"};
    ~Derived14() override { cout << "Derived14 dtor — releases " << msg << '\n'; }
    void greet() const override { cout << "Hello from Derived14\n"; }
};

void topic14() {
    cout << "\n=== TOPIC 14: virtual destructor ===\n";
    Base14 *p = new Derived14;
    p->greet();
    delete p;   // ✅ undefined behaviour without virtual dtor; works correctly here
}

// TOPIC 15 — member init ORDER vs init-list ORDER
// ════════════════════════════════════════════════════════════
struct Confusing15 {
    // ❌ Author expects: x init first, then y.  C++ says: members init in DECLARATION order,
    //    ignoring the order in the init list.  Using uninitialised value in initialiser is UB.
    int y{};               // declared FIRST
    int x{};               // declared SECOND

    Confusing15() : y(42), x(y + 1) {}   // ← y(42) applied to declared-first member, x(y+1) to declared-second
    // Net result: y=42, x=43; but swapping init-list order (x(y+1), y(42)) would NOT swap them.
    // Worse: if y relied on x in its constructor, x would be default-constructed first → silent bug.
};

// ✅ Always write initialisers in the same order as declarations
struct Clean15 {
    int a{};
    int b{};
    int c{};
    Clean15() : a(1), b(2), c(3) {}   // order matches declaration → no confusion
};

void topic15() {
    cout << "\n=== TOPIC 15: member init order vs init-list order ===\n";
    Confusing15 cf;
    cout << "Confusing15: y=" << cf.y << " x=" << cf.x << '\n';
    Clean15 cl;
    cout << "Clean15: a=" << cl.a << " b=" << cl.b << " c=" << cl.c << '\n';
    // Pro tip: enable -Wreorder (GCC/Clang) to catch mismatches.
}

// TOPIC 16 — default vs value initialization
// ════════════════════════════════════════════════════════════
void topic16() {
    cout << "\n=== TOPIC 16: default vs value initialization ===\n";
    // ❌ TRAP: default-initialised POD leaves data uninitialised
    // int uninit;  // garbage value — UB to read; compiler warns correctly
    // cout << uninit << '\n';  // ← DON'T DO THIS
    cout << "  (declaring `int uninit;` inside a real variable scope triggers a "
         << "warning — that WARNING is the compiler HELPING you.)\n";

    // ✅ Value-initialize with {} — zero / default-constructed
    int init{0};
    cout << "value-init: " << init << '\n';

    int arr1[5];       // default-init → uninitialised (also warns)
    (void)arr1;         // suppress unused warning here
    int arr2[5]{};     // value-init  → all zeros
    cout << "arr2[0]=" << arr2[0] << " (guaranteed 0)\n";

    vector<string> v0;           // default-init — empty vector, fine
    vector<int>    v1{};         // value-init — empty, fine; same effect here
    cout << "default & value init vectors are equivalent for containers.\n";
}

// TOPIC 17 — MAGIC NUMBERS
// ════════════════════════════════════════════════════════════
void topic17() {
    cout << "\n=== TOPIC 17: MAGIC NUMBERS ===\n";
    // ❌ Magic numbers — meaning is hidden, compiler cannot help you
    //   if (age > 65) retire();        // What is 65?  What's retire()?
    //   float price = qty * 0.95f;     // What is 0.95? Bulk discount? Late fee?
    // float UNUSED_price = 0;         // ← unused variable — compiler warns; treat as error
    // (void)price;                    // intentionally removed to illustrate warning

    // ✅ Named constants: self-documenting, single source of truth
    constexpr int RETIREMENT_AGE   = 65;
    constexpr double BULK_DISCOUNT = 0.95;
    (void)RETIREMENT_AGE; (void)BULK_DISCOUNT;  // used in real code — illustration here

    // ✅ For values that logically belong together, use enum class (scoped, type-safe)
    enum class CardSuit : int { Clubs=0, Diamonds=1, Hearts=2, Spades=3 };
    CardSuit s = CardSuit::Hearts;
    (void)s;  // unused in this illustration

    // ✅ Static analysis tip: -Wmagic-numbers (Clang) flags unexplained literals
}

// TOPIC 18 — modifying container while iterating over it
// ════════════════════════════════════════════════════════════
void remove_evens_bad(vector<int> &v) {
    (void)v;  // intentionally empty — see topic18 for the correct pattern
}

// ✅ Use erase-remove idiom (or erase_if in C++20)
void topic18() {
    cout << "\n=== TOPIC 18: modifying container while iterating ===\n";
    vector<int> v{1, 2, 3, 4, 5, 6};

    // C++17 erase-remove (standard)
    v.erase(remove_if(v.begin(), v.end(),
                      [](int x){ return x % 2 == 0; }),
            v.end());

    // C++20: simply
    // v.erase_if([](int x){ return x % 2 == 0; });

    cout << "after removing evens: ";
    for (int x : v) cout << x << ' ';
    cout << '\n';
}

// TOPIC 19 — returning std::move of a local variable
// ════════════════════════════════════════════════════════════
vector<int> bad_move(vector<int> v) {
    // ❌ Returning std::move(local) — disables NRVO, forces caller to move
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wredundant-move"
    return std::move(v);   // smelly: suppresses NRVO
#pragma GCC diagnostic pop
}

vector<int> good_move(vector<int> v) {
    // ✅ Just return — compiler applies NRVO or move-elision automatically
    return v;
}

void topic19() {
    cout << "\n=== TOPIC 19: returning std::move of a local ===\n";
    // Return Value Optimisation (RVO) eliminated copies/moves in good_move.
    // Adding std::move turns an eligible NRVO candidate into a move, which is slightly worse.
    cout << "Prefer plain `return v;` over `return std::move(v);`\n";
}

// TOPIC 20 — misunderstanding std::move
// ════════════════════════════════════════════════════════════
void topic20() {
    cout << "\n=== TOPIC 20: std::move doesn't MOVE anything ===\n";
    string s = "hello";
    string t = std::move(s);   // t is "hello"; s is now in valid but unspecified state

    // ❌ std::move is just a static_cast to an rvalue reference — it does NOT touch the obj.
    //    The *move constructor/assignment* that actually "steals" resources is what moves.
    // After std::move, s is still valid; calling s.size() returns something (probably 0 on most libs)
    // but must NOT be relied upon — treat s as "moved-from".

    cout << "t = " << t << "  s.empty() = " << boolalpha << s.empty() << '\n';
}

// TOPIC 21 — evaluation order is NOT necessarily left-to-right
// ════════════════════════════════════════════════════════════
void topic21() {
    cout << "\n=== TOPIC 21: evaluation order ===\n";
    int a = 0;

    // ❌ Old assumption: function args, operator operands evaluated left-to-right
    // f(a++, a);   ← undefined behaviour until C++17; even C++17+ comma-separated init-stmts
    //               still leave side-effect order ambiguous in some contexts

    // ✅ Don't rely on evaluation order — use sequencing (comma operator, or split into statements)
    ++a;
    int res = a;     // clearly sequenced
    (void)res;  // used within this block — illustrates ordering concept

    // ✅ C++17 guarantees left-to-right for:
    //   a || b    a && b    a , b    a ? b : c    some expressions
    // But =, +, -, etc. on non-overlapping operands still have no ordering guarantee pre-C++17
    cout << "a = " << a << "  (split into two statements to guarantee order)\n";
}

// TOPIC 22 — unnecessary heap allocations
// ════════════════════════════════════════════════════════════
struct BigObject { int data[256]; };

// ❌ Old way: heap-allocating small short-lived objects
// BigObject* create() { return new BigObject; }
// use(create());   // easy to forget delete → leak

// ✅ On stack (automatic storage) — RAII, no manual free needed
BigObject create_stack() { return {}; }

// ✅ Modern heap — unique_ptr (Topic 23)
unique_ptr<BigObject> create_heap_unique() { return make_unique<BigObject>(); }

void topic22() {
    cout << "\n=== TOPIC 22: avoid unnecessary heap allocations ===\n";
    const BigObject &ref = create_stack();  // no heap allocation at all
    (void) ref;
    cout << "Prefer stack allocation when object lifetime is local.\n";
}

// TOPIC 23 — unique_ptr vs shared_ptr
// ════════════════════════════════════════════════════════════
void topic23() {
    cout << "\n=== TOPIC 23: unique_ptr vs shared_ptr ===\n";
    // std::unique_ptr<T>
    //   • sole ownership — cheap (no ref-count overhead)
    //   • move-only semantics
    //   • default choice: "I am the only owner"

    // std::shared_ptr<T>
    //   • shared ownership — ref-counted (thread-safe atomic count)
    //   • legal to copy
    //   • costlier & cycles = memory leaks (use std::weak_ptr to break cycles)

    unique_ptr<int> uptr = make_unique<int>(42);   // ✅ sole owner
    // shared_ptr<int> sptr = make_shared<int>(42); // ✅ shared owners

    cout << "unique_ptr has no shared-ownership overhead.\n"
         << "Use unique_ptr first; reach for shared_ptr only when you genuinely need it.\n";
}

// TOPIC 24 — make_unique / make_shared
// ════════════════════════════════════════════════════════════
void topic24() {
    cout << "\n=== TOPIC 24: make_unique and make_shared ===\n";
    // ❌ Old way — exception unsafety: two allocations, type mismatch visible in call site
    // SharedPtrThing *raw = new SharedPtrThing(args);
    // shared_ptr<SharedPtrThing> p(raw);   // if shared_ptr ctor throws → raw leaks

    // ✅ make_shared — ONE allocation for control block + object, perfectly optimised
    auto p = make_shared<vector<int>>(10, 5);  // vector of 10× '5'

    // ✅ make_unique — same principle for unique_ptr
    auto u = make_unique<vector<int>>(5, 99);  // vector of 5× '99'

    cout << "make_unique/make_shared are exception-safe and often single-allocation.\n";
}

// TOPIC 25 — new and delete (manual heap)
// ════════════════════════════════════════════════════════════
void topic25() {
    cout << "\n=== TOPIC 25: avoid new / delete ===\n";
    // ❌ Anti-pattern: raw heap allocation with manual delete everywhere
    // int* p = new int(42);
    // ... many lines later ...
    // delete p;        // easy to forget, double-delete, or delete in wrong exception path

    // ✅ Use automatic storage or smart pointers — no delete keyword ever needed
    //    If heap IS required, always wrap in a smart pointer IMMEDIATELY:
    auto p = make_unique<int>(42);   // automatically deleted when p goes out of scope

    (void) p;
    cout << "Rule: Every new must be paired with a delete — but prefer smart pointers "
            "so you DON'T have to think about it.\n";
}

// TOPIC 26 — manual resource management → RAII
// ════════════════════════════════════════════════════════════
struct File26 {
    string blobs[4] = {"apples", "oranges", "bananas", "grapes"};
};
File26 *open_file_bad() { return new File26; }  // caller must remember delete

// ✅ RAII wrapper (Resource Acquisition Is Initialisation)
class File26_RAII {
public:
    File26 data;          // resource acquired in constructor
    ~File26_RAII() {      // released in destructor — automatic on scope exit
        // cout << "File26_RAII destroyed\n";
    }
};

void topic26() {
    cout << "\n=== TOPIC 26: RAII / manual resource management ===\n";
    File26_RAII f;   // acquired
    (void) f;
    // ... use f ...
}   // ← automatically released — no manual delete, exception-safe

// TOPIC 27 — raw pointers vs smart pointers
// ════════════════════════════════════════════════════════════
void topic27() {
    cout << "\n=== TOPIC 27: raw pointers are OK, just not for ownership ===\n";
    // UniquePtr is owns the obj, raw pointer does not.
    int value = 42;

    // ✅ Raw pointer for non-owning observation — no delete needed
    const int *view = &value;   // non-owning, OK
    (void) view;

    // ❌ Never use raw pointer for ownership transfer
    // int *leak = new int(42);   // ← must remember delete

    // ✅ For ownership: unique_ptr (sole owner)
    auto owned = make_unique<int>(42);

    // Summary table:
    //   raw pointer   = non-owning observer   (may be null)
    //   unique_ptr<T> = sole owner            (moveable, non-copyable)
    //   shared_ptr<T> = shared owner          (copyable, reference-counted)
    //   unique_ptr<T[]> = sole owner of dynamic array
    cout << "Raw pointer ↔ non-owning.  unique_ptr ↔ sole owning.  shared_ptr ↔ shared owning.\n";
};

// TOPIC 28 — shared_ptr when unique_ptr suffices
// ════════════════════════════════════════════════════════════
struct Widget {
    int id;
    Widget(int i = 0) : id(i) {}
};

// ❌ defaulting to shared_ptr everywhere — ref-count overhead for no benefit
// shared_ptr<Widget> make_widget_shared() { return make_shared<Widget>(...); }

// ✅ unique_ptr first — cheaper, conveys intent
unique_ptr<Widget> make_widget() { return make_unique<Widget>(); }

void topic28() {          // free function is NOT const — this was a copy-paste error
    cout << "\n=== TOPIC 28: prefer unique_ptr over shared_ptr ===\n";
    cout << "unique_ptr = 1 pointer + 1 vtable slot, no atomic ref-count.\n"
         << "shared_ptr = atomic ref-count (extra RAM/CPU), implies shared ownership.\n"
         << "Default to unique_ptr; promote to shared_ptr only on evidence of shared need.\n";
}

// TOPIC 29 — shared_ptr is NOT thread-safe for the POINTED-TO object
// ════════════════════════════════════════════════════════════
// ✅ ONLY the reference COUNT is atomic — the P O I N T E D - T O object is NOT protected.
// ❌ Misconception: two threads reading/writing *shared_ptr<T> is safe.
//    NO! The pointer itself (the pointee) requires separate synchronisation.

void topic29() {
    cout << "\n=== TOPIC 29: shared_ptr thread safety ===\n";
    cout << "shared_ptr ref-count updates  → atomic  → safe across threads\n"
         << "shared_ptr pointee data       → NOT atomic → requires mutex or atomics\n";
}

// TOPIC 30 — const pointer vs pointer to const
// ════════════════════════════════════════════════════════════
void topic30() {
    cout << "\n=== TOPIC 30: const pointer vs pointer-to-const ===\n";
    int x = 10, y = 20;

    // ✅ Read from right to left / use spreadsheet rule:
    //   T const * p          → pointer to    const T    (read-only pointee)
    //   T       * const p    → const pointer to        T   (read-only pointer)
    const int *p_to_const   = &x;   // *p_to_const = 5; ← compile error
    (void)p_to_const;               // intentionally declared to show the TYPE
    int       *const cp_const = &y;  // cp_const = &x;         ← compile error
    (void)cp_const;                 // intentionally declared to show the TYPE

    // ✅ Prefer aliases in complex cases (shown here for illustration):
    using const_int_ref = const int&;  (void)sizeof(const_int_ref);
    using int_ptr_const  = int* const; (void)sizeof(int_ptr_const);
    cout << "right-to-left:  'const int *p' = pointer to const int.\n";
}

// TOPIC 31 — ignoring compiler warnings
// ════════════════════════════════════════════════════════════
[[maybe_unused]] void unused_func() {}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
// ❌ Old way: silence specific warning locally while leaving the real issue
int ignored_warning = 42;
#pragma GCC diagnostic pop

void topic31() {
    cout << "\n=== TOPIC 31: compiler warnings ===\n";
    // ❌ Bad: -w (suppress ALL warnings), decorator-based suppression everywhere
    // ❌ Bad: leaving warnings unfixed — they point to REAL bugs (signed/unsigned mismatch,
    //         uninitialised return, narrow conversions, unreachable code …)

    // ✅ Enable the strictest warning flags from day one (compile-fail on warnings):
    //   GCC / Clang: -Wall -Wextra -Werror -Wpedantic
    //   MSVC:       /W4 /WX

    // ✅ Fix warnings; suppress ONLY when you have a justified reason:
    //   (suppress, not disable)
    //   [[maybe_unused]]          — function/param/variable (C++17)
    //   [[nodiscard]]             — caller must check return value
    //   pragma GCC diagnostic ...  — single-line scope suppression
    cout << "Warnings = zero-cost assertions.  Treat them as errors.\n";
}

// ═══════════════════════════════════════════════════════════════
//  MAIN — run all topics
// ═══════════════════════════════════════════════════════════════
int main() {
    topic1();  topic2();  topic3();  topic4();  topic5();  topic6();
    topic7();  topic8();  topic9();  topic10(); topic11(); topic12();
    topic13(); topic14(); topic15(); topic16(); topic17(); topic18();
    topic19(); topic20(); topic21(); topic22(); topic23(); topic24();
    topic25(); topic26(); topic27(); topic28(); topic29(); topic30();
    topic31();

    cout << "\n✅ All 31 topics covered.\n";
    return 0;
}
