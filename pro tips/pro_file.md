# PRO C++ TIPS — 31 Topics

> Each topic shows ❌ **Bad / Old Way** → ✅ **Good / Modern Way**

---

## Topic 1 — `using namespace std`

### ❌ Bad / Old Way
- Placing `using namespace std;` at file level (or especially in a header) pollutes the
  **global namespace** for every translation unit that includes it.
- Names like `count`, `reserve`, `distance`, `find` may silently clash with project identifiers,
  causing hard-to-diagnose bugs.

### ✅ Good / Modern Way
- Favour **fully-qualified names** (`std::cout`, `std::vector`) — safest for large projects.
- Or use **selective `using`** declarations to bring in only what you need:
  `using std::cout; using std::endl;`
- Avoid `using namespace std;` entirely in headers; restrict it to `.cpp` files
  if absolutely necessary and keep it inside a narrow scope.

```cpp
// ❌ Bad — pollutes entire file
using namespace std;

// ✅ Good — fully qualified
std::string t = "safe";

// ✅ Good — selective using
using std::cout;
cout << "Topic 1: selectively using std::cout is fine.\n";
```

---

## Topic 2 — `std::endl` inside a loop

### ❌ Bad / Old Way
- `std::endl` **writes a newline AND flushes the stream** on every call.
- Flushing is an expensive system call; inside a tight loop it can be **orders of magnitude slower**
  than writing `'\n'`.

### ✅ Good / Modern Way
- Use the character literal `'\n'` for a plain newline — **no flush**.
- Call `std::flush` explicitly **only** when you actually need output to appear immediately
  (e.g. before a long computation or during interactive prompts).
- **Rule of thumb:** `std::endl` ≡ `'\n' + flush()`

```cpp
// ❌ Bad — flushes every iteration; very slow for large N
for (int i = 0; i < 1'000'000; ++i)
    cout << i << std::endl;

// ✅ Good — newline only; flush manually if needed
for (int i = 0; i < 1'000'000; ++i)
    cout << i << '\n';

// Flush explicitly when output integrity matters (e.g. before waiting for user input)
cout << "Loading..." << flush;
```

---

## Topic 3 — Index-based `for` when range-based `for` fits better

### ❌ Bad / Old Way
- Manual index arithmetic (`i < vec.size()`, `vec[i]`) is verbose and **prone to off-by-one errors**,
  unsigned/signed mismatches, and accidental use of the wrong variable.

### ✅ Good / Modern Way
- **Range-based `for`** iterates directly over elements — no indices to manage,
  no iterator invalidation worries at the highest level.
- Even better: delegate to a standard algorithm (`std::accumulate`, `std::count_if`, …).

```cpp
vector<int> nums{10, 20, 30, 40};

// ❌ Bad — manual index
int sum_old = 0;
for (size_t i = 0; i < nums.size(); ++i)
    sum_old += nums[i];

// ✅ Good — range-for
int sum_new = 0;
for (int n : nums)
    sum_new += n;

// ✅ Even better — standard algorithm
int sum_algo = accumulate(nums.begin(), nums.end(), 0);
```

---

## Topic 4 — Rewriting standard library algorithms by hand

### ❌ Bad / Old Way
- Hand-rolled loops for searching, sorting, counting, and transforming are **longer, harder to read,
  easy to get wrong, and usually slower** than the optimised versions in `<algorithm>`.
- Every custom loop is a new testing burden.

### ✅ Good / Modern Way
- The C++ standard library provides well-tested, high-performance algorithms.
  **Compose them with lambdas** instead of writing imperative loops.
- Common replacements:
  | Hand-rolled loop | Standard algorithm |
  |-----------------|--------------------|
  | `for` seek       | `find` / `find_if` |
  | `for` bool cond  | `any_of` / `all_of` / `none_of` |
  | `for` sort       | `sort` / `stable_sort` |
  | `for` transform  | `transform` |
  | `for` accumulate | `accumulate` / `reduce` |

```cpp
vector<string> words{"the", "quick", "brown", "fox"};

// ❌ Bad — hand-rolled search
bool has_fox_old = false;
for (auto &w : words)
    if (w == "fox") has_fox_old = true;

// ✅ Good — expressive, tested, optimised
bool has_fox_new = any_of(words.begin(), words.end(),
                          [](const string &w){ return w == "fox"; });
sort(words.begin(), words.end());
auto fox_idx = find(words.begin(), words.end(), "fox");
```

---

## Topic 5 — C-style array vs `std::array`

### ❌ Bad / Old Way
- C-style arrays (`int a[5]`) **decay to pointers**, losing size information.
- No `.size()`, no `.begin()`/`.end()`, no range-for without effort.
- Calculating length with `sizeof(a) / sizeof(a[0])` is correct for the array type but
  fails once decay has occurred.

### ✅ Good / Modern Way
- `std::array<T, N>` is a **thin, zero-overhead wrapper** that:
  - carries its size (`.size()`)
  - works with all STL algorithms (`.begin()`, `.end()`)
  - **does not decay** to a pointer
  - can be passed by reference to functions without losing length

```cpp
// ❌ Bad — C-style array; decays; fragile size calculation
int c_arr[5] = {1, 2, 3, 4, 5};
// sizeof(c_arr) / sizeof(c_arr[0])  ← easy to get wrong

// ✅ Good — std::array; STL-compatible; carries size
array<int, 5> cpp_arr = {1, 2, 3, 4, 5};
for (int x : cpp_arr) cout << x << ' ';
cout << " | size=" << cpp_arr.size() << '\n';
```

---

## Topic 6 — `reinterpret_cast`

### ❌ Bad / Old Way
- `reinterpret_cast` **bypasses the type system** — it can produce arbitrary bit patterns
  that violate strict aliasing rules.
- The result is **undefined behaviour** (portable crashes, silent data corruption,
  or "it works on my machine").

### ✅ Good / Modern Way
- Use `std::memcpy` for **portable bitwise copies** across unrelated types — defined behaviour.
- In C++20+, use `std::bit_cast<T>()` — the compiler translates it to the optimal
  load/store sequence while remaining strictly conforming.
- Restrict `reinterpret_cast` to truly low-level scenarios (hardware register mapping)
  and **document** the rationale and platform assumptions in a comment nearby.

```cpp
int x = 0xDEADBEEF;

// ❌ Bad — strict-aliasing violation; UB
// uint32_t raw = *reinterpret_cast<uint32_t*>(&x);

// ✅ Good — portable bitwise copy via std::memcpy
uint32_t safe;
memcpy(&safe, &x, sizeof safe);
cout << hex << "reinterpreted via memcpy: 0x" << safe << '\n';

// ✅ C++20 — std::bit_cast (preferred when available)
// uintptr_t addr = std::bit_cast<uintptr_t>(&x);
```

---

## Topic 7 — Casting away `const`

### ❌ Bad / Old Way
- `const_cast` is used to remove `const` qualifications so a value can be mutated through
  a `const` reference/pointer.
- If the **original** object really is `const` (e.g. a literal, or a `const` variable),
  writing through a cast reference is **undefined behaviour** and may crash on read-only memory.

### ✅ Good / Modern Way
- Accept `const` — treat a `const` reference as read-only by contract.
- Only drop `const` through `const_cast` when you **know for certain** that the pointed-to
  object is non-const (e.g. a non-const object passed to a legacy API that lacks `const`).
- **Always document** the reason for the cast.
- Prefer refactoring the API to be `const`-correct instead of compromising safety at call sites.

```cpp
const int ci = 42;

// ❌ Bad — UB if original is truly const
// int &mutable_ref = const_cast<int&>(ci);
// mutable_ref = 99;   // ← RUNTIME CRASH or silent corruption

// ✅ Good — accept const; copy if mutation is needed
int non_const = ci;      // normal copy — no cast
(void)non_const;

// If a legacy API forces you to write... document why:
// void legacy_api(int* p);   // not const-correct
// int mutable_val = ci;
// legacy_api(&mutable_val);  // still no cast; safer copy
```

---

## Topic 8 — `map::operator[]` silently inserts a default element

### ❌ Bad / Old Way
- `map[k]` does **double duty**: looks up key `k`, and if key is **absent**, it **inserts**
  `k` with a **value-initialised** value (`int{}` → `0` for numeric types).
- This is a very common source of **silent logic bugs** — you think you're reading
  a pre-existing entry, but you've just created one.

### ✅ Good / Modern Way
- For **read-only** lookup use: `find` / `at` / `contains` (C++20).
- For **insertion** use: `emplace` / `try_emplace` / `insert_or_assign`.
- Rule: **`find` when you need to distinguish absent vs zero; `at` for exception-on-miss.**

```cpp
map<string, int> scores{{"alice", 10}};

// ❌ Bad — silently inserts "bob" with score 0
cout << scores["bob"] << '\n';   // prints 0; "bob" is NOW in the map

// ✅ Good — find returns iterator; no insertion on miss
auto it = scores.find("carol");
if (it != scores.end())
    cout << it->second << '\n';
else
    cout << "carol not found — no insertion.\n";

// ✅ C++20 — write once; no insert on miss
if (scores.contains("alice"))
    cout << "alice found.\n";
```

---

## Topic 9 — Ignoring `const`-correctness

### ❌ Bad / Old Way
- Passing large objects (containers, strings, structs) **by value** causes a full copy on
  every call — unnecessary allocation and data movement.
- Functions that only read the argument should not accept a mutable copy.

### ✅ Good / Modern Way
- Pass **read-only** large objects as `const T&` (or `const auto&` in generic code).
- This avoids copies while guaranteeing the function cannot accidentally mutate the argument.
- For small trivially-copyable types (`int`, `double`, small POD structs), passing by value
  is actually fine — the compiler optimises it.

```cpp
vector<int> data{1, 2, 3};

// ❌ Bad — copies entire vector every call
// void print_sum(vector<int> v) { ... }

// ✅ Good — no copy; read-only guarantee
void print_sum_correct(const vector<int> &v);   // (void)v; — signature only
print_sum_correct(data);
```

---

## Topic 10 — String literal lifetime

### ❌ Bad / Old Way
- A temporary `std::string` is destroyed at the end of the full expression.
  Calling `.c_str()` on it and storing the pointer **dangles** immediately.
- This is one of the most common sources of use-after-free bugs in C++ beginners.

### ✅ Good / Modern Way
- **String literals** (`"hello"`) have **static storage duration** — the pointer is valid
  for the entire lifetime of the program, so storing a `const char*` to a literal is fine.
- For `std::string` objects, keep the `std::string` alive for as long as you need the buffer;
  the `c_str()` pointer is only valid while the owning string exists.

```cpp
// ❌ Bad — c_str() dangles as soon as the full expression ends
// const char* dangling = std::string("hello").c_str();

// ✅ Good — string literals have static (program-lifetime) storage
const char *stat = "hello, static";    // safe for the whole program

// ✅ Good — keep the owning string alive; c_str() is valid while s lives
std::string s = "hello, wrapped";
cout << "safe: " << s.c_str() << " (while s is alive)\n";
```

---

## Topic 11 — Structured Bindings (C++17)

### ❌ Bad / Old Way
- Manually declaring an iterator (`map<string, int>::const_iterator`) and accessing
  `.first` / `.second` is verbose, easy to mistype, and obscures intent.
- Decomposing tuples requires `std::tie()` and separate variable declarations.

### ✅ Good / Modern Way
- **Structured bindings** (`auto [a, b]`) decompose:
  - `std::pair` / `std::tuple`
  - Aggregate structs
  - Array elements
- They work in `for` loops, `if` initialisers, and regular declarations.
- Name the elements from the binding **meaningfully** — the compiler infers the types.

```cpp
map<string, int> inventory{{"apples", 5}, {"bananas", 12}};

// ❌ Bad — verbose iterator
for (auto it = inventory.begin(); it != inventory.end(); ++it)
    cout << it->first << " => " << it->second << '\n';

// ✅ Good — structured binding in range-for
for (auto [fruit, count] : inventory)
    cout << fruit << " => " << count << '\n';

// Works with std::tuple too
tuple<int, string, double> t{1, "hello", 3.14};
auto [num, txt, pi] = t;
cout << num << ' ' << txt << ' ' << pi << '\n';
```

---

## Topic 12 — Out-params vs returning a `struct`

### ❌ Bad / Old Way
- Functions that "return multiple values" via **out-parameters** force the caller to
  declare and pre-allocate variables for every output — easily forgotten.
- The function signature becomes unwieldy and the intent is obscured.

### ✅ Good / Modern Way
- Return a **small struct** to group related outputs.
- The compiler applies **NRVO (Named Return Value Optimisation)** and **copy elision**,
  so there is **zero runtime cost** compared to out-params.
- At the call site, the return value is a single named variable, or — with C++17
  structured bindings — a single de-structured line.

```cpp
struct PersonInfo { string name; string email; int age; };

// ❌ Bad — out-param tuple hack
// bool get_person_info_old(int id, string& name, string& email, int& age);

// ✅ Good — return a struct; NRVO eliminates copy
PersonInfo get_person_info(int id) {
    (void)id;
    return {"Alice", "alice@example.com", 30};
}

// Call site
PersonInfo p = get_person_info(1);
// Or with structured bindings (C++17):
// auto [name, email, age] = get_person_info(1);
```

---

## Topic 13 — `constexpr`

### ❌ Bad / Old Way
- Magic values hardcoded in runtime code have no compile-time guarantee and cannot be used
  in constant-expression contexts (array sizes, template arguments).
- "Magic constants" are silently `int` and re-evaluated wherever they are read.

### ✅ Good / Modern Way
| C++ version | Improvement |
|-------------|-------------|
| C++11 | `constexpr` variables and simple functions |
| C++14 | Relaxed `constexpr` — loops, local variables inside `constexpr` functions |
| C++20 | `consteval` (must execute at compile time); `constinit` (must be initialised at compile time, prevents static init order issues) |

```cpp
// ❌ Bad — runtime constant; inspected only by the compiler; not usable at compile time
const int BUF_SIZE = 1024;

// ✅ Good — compile-time constant; inlined everywhere; usable in array sizes, templates
constexpr int BUF_SIZE = 1024;

// ✅ constexpr functions — evaluated at compile time when arguments are known
constexpr int square(int n) { return n * n; }   // computes at compile time for literal args

int arr[square(8)] = {};   // array sized at compile time — zero runtime overhead

// constinit (C++20) — guarantees static var initialisation at compile time
// prevents the "static initialisation order fiasco"
```

---

## Topic 14 — Virtual destructor in polymorphic base classes

### ❌ Bad / Old Way
- Forgetting to mark a base-class destructor as `virtual` when you **delete through a base
  pointer** results in **undefined behaviour** — only the base destructor runs,
  derived-class fields and resources are **never released**.

### ✅ Good / Modern Way
- **Any** class that is intended to be a polymorphic base (has any `virtual` function)
  **must** declare a **`virtual` destructor**.
- Mark it `override` in every derived class — the compiler checks the signature matches
  and warns you at compile time if it doesn't.
- Prefer `= default` when no custom cleanup is needed:
  `virtual ~Base() = default;`

```cpp
// ❌ Bad — no virtual dtor; delete through Base* is UB
// struct Base { ~Base() {} };

// ✅ Good — virtual destructor in all polymorphic bases
struct Base14 {
    virtual ~Base14() { cout << "Base14 dtor\n"; }
    virtual void greet() const { cout << "Hello from Base14\n"; }
};

struct Derived14 : Base14 {
    ~Derived14() override { cout << "Derived14 dtor — releases resources\n"; }
    void greet() const override { cout << "Hello from Derived14\n"; }
};

// Correctly deletes both base and derived sub-objects
Base14 *p = new Derived14;
p->greet();
delete p;   // ✅ both destructors fire
```

---

## Topic 15 — Member initialisation order vs init-list order

### ❌ Bad / Old Way
- C++ always initialises class members in the **order they are declared** in the class body,
  **not** in the order they appear in the constructor's initialiser list.
- If initialiser list order is reordered for "readability" without matching the declaration order,
  one member can be initialised using the **default-constructed value** of another member —
  silent and hard to trace.
- Enable `-Wreorder` (GCC / Clang) to have the compiler flag mismatches.

### ✅ Good / Modern Way
- Always write the initialiser list **in the same order as the declarations**.
- Treat the init-list as programmatically paired by position, not name.

```cpp
// ❌ Bad — init-list order contradicts declaration order
struct Confusing {
    int y{};   // declared 1st
    int x{};   // declared 2nd
    Confusing() : y(42), x(y + 1) {}   // reads y before it is "officially" init — fragile
    // gcc -Wreorder will flag this
};

// ✅ Good — init-list mirrors declaration order
struct Clean {
    int a{};
    int b{};
    int c{};
    Clean() : a(1), b(2), c(3) {}   // order = declaration order → no confusion
};
```

---

## Topic 16 — Default vs value initialisation

### ❌ Bad / Old Way
| Expression | Kind of init | Result |
|-----------|-------------|--------|
| `int x;` | Default | **Indeterminate / garbage** value — reading it is UB |
| `int a[5];` | Default | All elements **garbage** |
| `T t;` (no default ctor) | Default | Compile error (better than silent UB) |

### ✅ Good / Modern Way
| Expression | Kind of init | Result |
|-----------|-------------|--------|
| `int x{};` | Value | **Zero** (guaranteed) |
| `int a[5]{};` | Value | All elements **zero** |
| `vector<int> v;` | Default | Empty vector (safe — default-ctor exists) |
| `vector<int> v{};` | Value | Empty vector (same result; intent clearer) |

```cpp
// ❌ Bad — garbage; reading it is UB
// int uninit;
// cout << uninit << '\n';

// ✅ Good — value-init via {} (zero / default-constructed)
int init{0};
cout << "value-init: " << init << '\n';

int arr2[5]{};   // all zeros guaranteed
cout << "arr2[0]=" << arr2[0] << " (guaranteed 0)\n";
```

---

## Topic 17 — Magic Numbers

### ❌ Bad / Old Way
- **Magic numbers** are unnamed numeric literals whose meaning is hidden at the point of use.
- Easy to mistype, easy to forget to update in all places when the business rule changes.
- The compiler cannot help — it has no idea `65` means *retirement age*.

### ✅ Good / Modern Way
- Replace magic numbers with **named constants**:
  - `constexpr int RETIREMENT_AGE = 65;`
  - `constexpr double BULK_DISCOUNT = 0.95;`
- For sets of related integer values, use **scoped enums** (`enum class`) —
  type-safe, self-documenting, and IDE-friendly.
- Enable `-Wmagic-numbers` (Clang) to highlight unexplained literals.

```cpp
// ❌ Bad — meaning is hidden
// if (age > 65) retire();
// float price = qty * 0.95f;

// ✅ Good — self-documenting, single source of truth
constexpr int RETIREMENT_AGE   = 65;
constexpr double BULK_DISCOUNT = 0.95;

// ✅ Scoped enum — type-safe, grouped values
enum class CardSuit : int { Clubs = 0, Diamonds = 1, Hearts = 2, Spades = 3 };
CardSuit s = CardSuit::Hearts;
```

---

## Topic 18 — Modifying a container while iterating over it

### ❌ Bad / Old Way
- Erasing elements **from the same container** you are iterating over **invalidates iterators**
  (and potentially end iterators too).
- The classic mistake — skipping the element following an erased one:
  ```cpp
  for (auto it = v.begin(); it != v.end(); ++it)
      if (*it % 2 == 0) v.erase(it);  // ← it is now invalid; UB on next ++it
  ```

### ✅ Good / Modern Way
- Use the **erase-remove idiom** — `remove_if` reorders the range (it *does not erase*),
  then `erase` removes the tail in one call:
  ```cpp
  v.erase(remove_if(v.begin(), v.end(), [](int x){ return x % 2 == 0; }), v.end());
  ```
- In **C++20** the standard library provides `std::erase_if(v, pred)` which hides the idiom.
- This pattern works for `vector` and `string`. For `std::list`/`std::forward_list`,
  use their own `remove_if` member (which actually erases, no separate call needed).

```cpp
vector<int> v{1, 2, 3, 4, 5, 6};

// ✅ erase-remove idiom (C++98 onward)
v.erase(remove_if(v.begin(), v.end(),
                  [](int x){ return x % 2 == 0; }),
        v.end());

// ✅ C++20 — library hides the pattern
// std::erase_if(v, [](int x){ return x % 2 == 0; });
```

---

## Topic 19 — Returning `std::move(local)` disables NRVO

### ❌ Bad / Old Way
```cpp
vector<int> bad_move(vector<int> v) {
    return std::move(v);   // smelly — marks implicit move as rvalue, disables NRVO
}
```
- Adding `std::move` to a local return **turns an eligible NRVO (copy elision) candidate
  into a move**, which is one step worse for performance than what the compiler would do silently.
- NRVO === **N**amed **R**eturn **V**alue **O**ptimisation — the compiler elides the copy/move
  entirely when it can construct the return object at the call site.

### ✅ Good / Modern Way
```cpp
vector<int> good_move(vector<int> v) {
    return v;   // plain return — NRVO or move-elision applies automatically
}
```

| Return style | What the compiler does |
|-------------|------------------------|
| `return v;` | NRVO (no copy, no move) — **best** |
| `return std::move(v);` | Forces a move — **NRVO blocked** |
| `return T(args);` | RVO (constructing temporary) — usually elided too |

```cpp
vector<int> good_move(vector<int> v) {
    return v;   // ✅ plain return — best
}
```

---

## Topic 20 — `std::move` doesn't **move** anything

### ❌ Misconception
- Many believe `std::move` physically relocates memory or (like `memmove`) copies bytes.
- `std::move` is actually just a **`static_cast` to an rvalue reference**.
- It does **not** touch the object at all. The **move constructor / move assignment operator**
  of the *target* type is what actually "steals" the resources.
- After `std::move`, the source is in a **valid but unspecified state** (call `.empty()`,
  `.size()`, `.clear()`, but don't read its contents).

```cpp
string s = "hello";
string t = std::move(s);   // t owns the buffer; s is "moved-from"

// ❌ std::move = "cast to rvalue reference"; the move *ctor* steals resources
// After this, s.empty() may be true — but DON'T RELY ON specific state.
cout << "t = " << t << "  s.empty() = " << boolalpha << s.empty() << '\n';
```

---

## Topic 21 — Evaluation order is NOT necessarily left-to-right

### ❌ Bad / Old Way
- C++ does **not guarantee left-to-right evaluation** of function arguments, `=` right-hand side,
  `+`/`-` operands, or almost any binary operator (pre-C++17).
- Side-effects on the same variable between unsequenced evaluations → **undefined behaviour**.

### ✅ Good / Modern Way
- **Split into separate sequenced statements** to enforce order.
- Or use the **comma operator** when you need a single expression:
  `func(++a, a)` is UB; `(++a, func(a))` is well-defined.
- C++17 brought sequencing guarantees for: `a || b`, `a && b`, `a , b`, `a ? b : c`,
  and the related-left-to-right-of-`=` for *class* member initialisers using `()`,
  but most binary operators still have no ordering guarantee on non-overlapping operands.

```cpp
int a = 0;

// ❌ Bad — order of evaluation of function args is unspecified
// f(a++, a);   // UB or silently wrong

// ✅ Good — split into sequenced statements
++a;             // happens first
int res = a;     // happens after
cout << "a = " << a << '\n';
```

---

## Topic 22 — Unnecessary heap allocations

### ❌ Bad / Old Way
- Heap-allocating objects with `new` when the **lifetime is local** adds allocation overhead
  and makes ownership cleanup manual — easy to leak, easy to double-free.

### ✅ Good / Modern Way
- Prefer **automatic (stack) storage** whenever the object's lifetime is bounded by its scope.
- The compiler handles allocation and deallocation — RAII at zero cost.
- If the lifetime genuinely needs to extend beyond the scope, use `unique_ptr` / `shared_ptr`.

```cpp
struct BigObject { int data[256]; };

// ❌ Bad — manual new; caller responsible for delete
// BigObject* create() { return new BigObject; }
// use(create());   // easy to forget delete → leak

// ✅ Good — automatic storage; RAII; no manual free
BigObject create_stack() { return {}; }

void topic22() {
    const BigObject &ref = create_stack();   // no heap allocation at all
    (void)ref;
}
```

---

## Topic 23 — `std::unique_ptr` vs `std::shared_ptr`

### Background
| Smart pointer | Ownership model | Cost | Copyable? | Default choice? |
|---------------|----------------|------|-----------|-----------------|
| `std::unique_ptr<T>` | Exactly **one** owner | Low (no ref-count) | ❌ Move-only | **Yes** |
| `std::shared_ptr<T>` | **Multiple** owners | Higher (atomic ref-count) | ✅ Yes | Only when shared ownership is needed |
| `std::weak_ptr<T>` | Non-owning observer of `shared_ptr` | Low | ✅ Yes | Break reference cycles |

### ✅ Good / Modern Way
- Default to **`std::unique_ptr`** — it is the cheapest option and clearly expresses intent.
- Reach for **`std::shared_ptr`** only when the object genuinely needs shared ownership.
- **Danger:** circular strong `shared_ptr` references (A→B→A) cause permanent memory leaks.
  Break cycles with `std::weak_ptr`.

```cpp
// ✅ sole owner — move-only, cheap
unique_ptr<int> uptr = make_unique<int>(42);

// ✅ shared owners — ref-counted, slightly more overhead
// shared_ptr<int> sptr = make_shared<int>(42);
```

---

## Topic 24 — `make_unique` / `make_shared` over manual `new`

### ❌ Bad / Old Way
- Creating a `shared_ptr` from a raw `new`-allocated pointer is **not exception-safe**.
  The `shared_ptr` constructor runs AFTER `new`; if the constructor throws,
  the raw pointer **leaks** because there is no `delete` to free it.
- Requiring the type name at the call site (`shared_ptr<Foo>(new Foo(args))`) also
  duplicates the type — a DRY violation.

### ✅ Good / Modern Way
- `std::make_shared<T>(args...)` performs **one single allocation** for both the control block
  and the object — better cache locality, less fragmentation.
- `std::make_unique<T>(args...)` mirrors the same pattern for `unique_ptr`.
- Both are **exception-safe** — if an argument throws, no memory has been allocated yet.

```cpp
// ❌ Bad — not exception-safe; type name repeated
// shared_ptr<Vector> p(new Vector(10, 5));
// shared_ptr<Vector> p(raw);   // if shared_ptr ctor throws → raw leaks

// ✅ Good — single allocation; exception-safe; no repeated type name
auto p = make_shared<vector<int>>(10, 5);   // 10 elements, initialised to 5

auto u = make_unique<vector<int>>(5, 99);   // 5 elements, initialised to 99
```

---

## Topic 25 — Avoid raw `new` / `delete`

### ❌ Bad / Old Way
- Raw `new` / `delete` require the programmer to pair them manually — on every code path,
  including exception paths, destructors, and early-return branches.
- Forgetting `delete` → **memory leak**. Calling `delete` twice → **undefined behaviour**.

### ✅ Good / Modern Way
- If an object must live on the heap, wrap it in a smart pointer **immediately after
  allocation**, so deallocation is automatic — even on exceptions.
- In practice: **never write `delete` manually**; either use automatic storage or a
  smart pointer.

```cpp
// ❌ Bad — manual new/delete; easy to forget or double-delete
// int *p = new int(42);
// ...many lines later...
// delete p;

// ✅ Good — smart pointer handles deallocation automatically
auto p = make_unique<int>(42);   // automatically deleted when p leaves scope
(void)p;
```

---

## Topic 26 — Manual resource management → RAII

### ❌ Bad / Old Way
- Manually allocating and freel Owning a resource (file handle, socket, mutex, memory)
  couples every calling path with a manual `free`/`close`/`delete`.
- An exception thrown between allocation and release **leaks the resource**.
- Forgetting the release on any code path — early returns, errors — also leaks it.

### ✅ Good / Modern Way
- **RAII (Resource Acquisition Is Initialisation):**
  - **Acquire** the resource in the constructor.
  - **Release** it in the destructor.
- The destructor runs automatically on normal scope exit **and** during stack unwinding
  after an exception — the resource is always freed.
- C++ standard library types (`unique_ptr`, `vector`, `fstream`, `thread`, `lock_guard`)
  follow RAII — prefer them over raw handles.

```cpp
File26 *open_file_bad() { return new File26; }  // ❌ caller must remember delete

// ✅ RAII — acquire in ctor, release in dtor, automatic on scope exit
class File26_RAII {
public:
    File26 data;
    ~File26_RAII() {}
};

void topic26() {
    File26_RAII f;   // resource acquired
    // ... use f ...
}                    // ← automatically released — exception-safe
```

---

## Topic 27 — Raw pointers: ownership vs non-ownership

### ❌ Misconception
- Equating **raw pointers with ownership** — a `T*` requires manual `delete`, easy to forget,
  easy to double-free, easy to pass the wrong ownership around.

### ✅ Good / Modern Way
- **Raw pointers** are perfectly valid **for non-owning observation**:
  - Looking at an object owned by someone else (e.g. a `parent` pointer in a GUI tree).
  - Optional / nullable references (use `std::optional<T&>` in C++26; raw pointer for today).
- **Never use `new` + raw pointer return** for ownership — always wrap in a smart pointer.

| Pointer type | Ownership | Who deletes? |
|--------------|-----------|--------------|
| `T*` (raw) | **None** — non-owning | Nobody (must outlive the owner) |
| `unique_ptr<T>` | **Sole** owner | `unique_ptr` destructor |
| `shared_ptr<T>` | **Shared** owner | Last `shared_ptr` → destroys |

```cpp
int value = 42;

// ✅ Non-owning raw pointer — fine for observation; no delete involved
const int *view = &value;

// ✅ Sole ownership — smart pointer
auto owned = make_unique<int>(42);

// ❌ Never — raw pointer for ownership
// int *leak = new int(42);   // must remember delete
```

---

## Topic 28 — Don't default to `shared_ptr`; use `unique_ptr` first

### ❌ Bad / Old Way
- Reaching for `shared_ptr` as the "default smart pointer" adds **atomic ref-count overhead**
  on every copy, and — more importantly — **implies shared ownership** when there is none.
- Reference-counted pointers mask design questions about who truly owns the object.

### ✅ Good / Modern Way
| | `unique_ptr` | `shared_ptr` |
|--|-------------|--------------|
| Ownership | Exactly **one** owner | **Multiple** owners |
| Copyable? | ❌ Move-only | ✅ Copyable (atomic ref-count) |
| Cache cost | Low (single pointer) | Higher (control block + atomics) |
| Cycles | N/A | Leaks (use `weak_ptr` to break) |
| Default? | ✅ Use first | Only when shared ownership is proven |

- **Algorithm:** start with `unique_ptr`; promote to `shared_ptr` only at the call site
  where you prove you need shared ownership — never the other way around.

```cpp
struct Widget { int id = 0; };

// ❌ Bad — shared_ptr by default; unnecessary overhead
// shared_ptr<Widget> make_widget_shared();

// ✅ Good — unique_ptr first; upgrade only if proven needed
unique_ptr<Widget> make_widget() { return make_unique<Widget>(); }
```

---

## Topic 29 — `shared_ptr` thread safety: ref-count ≠ pointee

### ❌ Misconception
- A common misunderstanding: *"`shared_ptr` is thread-safe, so I can read and write
  the pointee from multiple threads without a mutex."*
- **Only the reference count updates are atomic** — the data the pointer refers to is
  completely unprotected.

### ✅ Good / Modern Way
- **Atomic guarantee:** `shared_ptr::use_count()` and internal ref-count updates are
  thread-safe — you can copy / destroy `shared_ptr` instances from multiple threads safely.
- **Not atomic:** reading/writing `*ptr` from two threads concurrently requires
  external synchronisation (`std::mutex`, `std::atomic<T>`, etc.), exactly as if you
  had a raw pointer.
- **Tip:** keep `shared_ptr` copies short-lived in threads — pass the *raw pointer*
  (`ptr.get()`) into the worker; synchronise access to `*ptr` explicitly.

```
shared_ptr<int> sp = make_shared<int>(0);

// ✅ SAFE — ref-count updates are atomic across threads
// shared_ptr<int> sp_copy = sp;   // OK from multiple threads

// ❌ NOT SAFE — the p O I N T E E d requires separate synchronisation
// *sp += 1;   // concurrent writes from 2 threads = data race, UB

// ✅ SAFE — protect pointee with mutex or atomic<int>
```

---

## Topic 30 — `const` pointer vs pointer-to-const

### ❌ Bad / Old Way
- The placement of `const` in pointer declarations is notoriously confusing:
  - `const Foo* p` — is `p` const? or `*p`?
  - `Foo* const p` — which one is const?
- Reading `const` from left-to-right leads to wrong conclusions.

### ✅ Good / Modern Way
- **Read declarations from right to left** — this is the "spreadsheet rule" used by the
  C++ standard committee:
  ```
  T const * p   →  "p is a pointer to          const T"   → *p is read-only
  T       * const p  →  "p is a const pointer to         T"  → p itself is read-only
  ```
- Prefer writing `T const` instead of `const T` — it composes predictably with `*const`.

| Declaration | Meaning |
|-------------|---------|
| `const int * p` | `p` can be reassigned; `*p` is read-only |
| `int * const p` | `p` cannot be reassigned; `*p` is writable |
| `const int * const p` | `p` cannot be reassigned; `*p` is read-only |

```cpp
int x = 10, y = 20;

// *p_to_const cannot be modified through the pointer — compiler enforces immutability
const int *p_to_const   = &x;

// p_const cannot be reseated — pointer itself is fixed
int *const cp_const = &y;

// Use type aliases for readability in complex cases
using const_int_ref = const int&;
using int_ptr_const  = int* const;
```

---

## Topic 31 — Ignoring compiler warnings

### ❌ Bad / Old Way
- Suppressing or ignoring warnings lets **real bugs** hide — signed/unsigned mismatches,
  uninitialised return values, unreachable code, narrow conversions, and more.
- Adding `#pragma` suppressions or disabling `-Werror` in CI turns warnings into technical debt.
- Passing `-w` (suppress ALL warnings) is an anti-pattern in production code.

### ✅ Good / Modern Way
- **Enable the strictest warning flags on day one** — compile should **fail** on any warning:
  | Compiler | Flags |
  |----------|-------|
  | GCC / Clang | `-Wall -Wextra -Werror -Wpedantic` |
  | MSVC | `/W4 /WX` |
- Fix the root cause before suppressing.
- When a warning is justified, **suppress locally** (not globally):
  - `[[maybe_unused]]` — marks functions/variables as intentionally unused (C++17)
  - `[[nodiscard]]` — compiler warns if the return value is discarded
  - `#pragma GCC diagnostic push/pop` — single-line suppression in a narrow scope

```cpp
[[maybe_unused]] void intentionally_unused() {}   // explicit — compiler is happy

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
// Suppress a single warning at the point of use — do NOT use this to hide real issues
int justified_local = 42;
#pragma GCC diagnostic pop

void topic31() {
    // Treat every warning as a zero-cost assertion from your compiler.
}
```
---

## Topic 32 — RAII (Resource Acquisition Is Initialization)

### What is RAII?
RAII ties a resource's **lifetime** directly to an object's **scope lifetime**:

- **Construct** the object → **acquire** the resource.
- **Destroy** the object → **release** the resource (automatic, via destructor).

Resources managed this way:
- Memory
- Files
- Mutexes
- Network sockets

### ❌ Bad / Old Way — Manual `fclose` / `free`

Explicit manual cleanup is brittle: an early `return`, a thrown exception, or a branching code path
leaving `fclose()` unreachable causes a **resource leak**.

```cpp
FILE* file = fopen("data.txt", "r");

if (file == nullptr)
    return;

// use file

// 💥 If an exception or return happens before here → fclose() never runs
fclose(file);
```

### ✅ Good / Modern Way — RAII wrapper

Standard-library types already follow RAII. `std::ifstream` acquires the file handle in its
constructor and closes it automatically in its destructor.

```cpp
#include <fstream>

void readFile()
{
    std::ifstream file("data.txt");   // file opened here

    // use file (automatic cleanup even if exception is thrown)

}                                     // ← destructor fires; file automatically closed
```

### ✅ Good / Modern Way — RAII with a custom class

```cpp
#include <iostream>

class Resource
{
public:
    Resource()
    {
        std::cout << "Resource acquired\n";
    }

    ~Resource()
    {
        std::cout << "Resource released\n";
    }
};

int main()
{
    Resource r;       // "Resource acquired"
}                    // ← "Resource released" — destructor fires automatically

// Output:
// Resource acquired
// Resource released
```

### Why RAII is powerful
| Without RAII | With RAII |
|--------------|-----------|
| Memory leaks | Automatic cleanup |
| Dangling resources | Safer code |
| Exception-unsafe | Modern C++ foundation |

---

## Topic 33 — Move Semantics

### Background
Before C++11, objects were **copied** everywhere. Copying large objects is expensive.

**Move semantics** allows **transfer of ownership** instead of copying.

| Operation | Cost |
|-----------|------|
| Copy | Allocates + duplicates data → slow |
| Move | Transfers internal pointer → cheap |

### ❌ Bad / Old Way — Copy (expensive)

```cpp
std::string a = "Hello";

std::string b = a;    // memory duplicated

// slow for large strings / vectors
```

### ✅ Good / Modern Way — Move (fast)

```cpp
std::string a = "Hello";

std::string b = std::move(a);    // internal memory transferred

// very fast — no allocation, no data copy
```

### ⚠️ Important — `std::move` does NOT move anything

`std::move` is just a `static_cast` to an rvalue reference. It does **not** touch the object.
The **move constructor / move assignment operator** of the *target* type is what actually "steals"
the resources.

After `std::move`, the source is in a **valid but unspecified state** — don't read its contents
but calling `.empty()`, `.size()`, or `.clear()` is fine.

### Move Constructor Example

```cpp
#include <iostream>
#include <vector>

class MyClass
{
    std::vector<int> data;

public:
    MyClass(std::vector<int> d)
        : data(std::move(d))    // transfers ownership of d's buffer into data
    {
        std::cout << "Moved\n";
    }
};

// Why move semantics matters:
// Huge performance improvement in:
//   vectors, strings, containers, return values
```

---

## Topic 34 — Smart Pointers

Smart pointers **automatically manage memory** — no manual `delete` required.

### Three main types
| Pointer | Ownership | Cost | Copyable? |
|---------|-----------|------|-----------|
| `std::unique_ptr<T>` | Exactly **one** owner | Low (no ref-count) | ❌ Move-only |
| `std::shared_ptr<T>` | **Multiple** owners | Higher (atomic ref-count) | ✅ Yes |
| `std::weak_ptr<T>` | Non-owning observer of `shared_ptr` | Low | ✅ Yes |

### ❌ Bad / Old Way — Raw pointer (manual delete)

```cpp
MyClass* p = new MyClass();

delete p;          // ← must remember to call this on every code path

// Problems:
//   memory leak risk
//   manual delete
//   exception-unsafe
```

### ✅ Good / Modern Way — `unique_ptr` (single owner)

`unique_ptr` is the **default smart pointer** — one owner, automatic deallocation.

```cpp
#include <memory>

auto p = std::make_unique<int>(10);   // automatically deleted when p goes out of scope
```

#### Ownership Transfer

```cpp
auto p1 = std::make_unique<int>(5);

auto p2 = std::move(p1);     // p2 now owns the object

// p1 becomes nullptr
// p2 owns object
```

### ✅ Good / Modern Way — `shared_ptr` (multiple owners)

When more than one piece of code must hold a reference.

```cpp
auto p1 = std::make_shared<int>(10);

auto p2 = p1;                // p1 and p2 both own it

// Reference count increases to 2.
// Object is destroyed automatically when count drops to 0.
```

### ✅ Good / Modern Way — `weak_ptr` (break cycles)

`weak_ptr` observes a `shared_ptr` without increasing the reference count — used to break
strong circular references that would otherwise leak.

```cpp
std::weak_ptr<int> wp = p1;   // does not increase ownership count
```

### Which one to use?
| Pointer | Use |
|---------|-----|
| `unique_ptr` | ✅ Default / sole owner |
| `shared_ptr` | Shared ownership |
| `weak_ptr` | Break reference cycles |

---

## Topic 35 — STL Algorithms

The C++ Standard Library already provides highly optimised algorithms in `<algorithm>`.
**Compose them with lambdas** instead of writing imperative loops by hand.

### ❌ Bad / Old Way — Manual loop

```cpp
int count = 0;

for (int n : nums)
{
    if (n % 2 == 0)
        count++;
}
```

### ✅ Good / Modern Way — STL algorithm

```cpp
#include <algorithm>

int count = std::count_if(
    nums.begin(),
    nums.end(),
    [](int n)
    {
        return n % 2 == 0;
    }
);
```

### Common STL Algorithms
| Task | Algorithm |
|------|-----------|
| Sort | `std::sort(v.begin(), v.end());` |
| Find | `auto it = std::find(v.begin(), v.end(), 5);` |
| Reverse | `std::reverse(v.begin(), v.end());` |
| Transform | `std::transform(v.begin(), v.end(), v.begin(), [](int x){ return x * 2; });` |
| Count if | `std::count_if(...)` |

### Benefits
- Optimised
- Expressive
- Less bugs

---

## Topic 36 — Const Correctness

`const` means: **this should not change**.

### ❌ Bad / Old Way — Non-const reference

```cpp
void print(std::string& s)    // mutable reference — cannot accept const / temporaries
{
    std::cout << s;
}

// ❌ Cannot print a const string or a string literal
```

### ✅ Good / Modern Way — `const` reference

```cpp
void print(const std::string& s)
{
    std::cout << s;
}

// ✅ Accepts both non-const and const strings; reads are enforced by the compiler
```

### Const Member Function

```cpp
class Test
{
    int x;

public:
    int get() const           // ← const after the parameter list
    {
        return x;             // compiler guarantees this function cannot modify *this
    }
};

// const means: the member function is NOT allowed to change the object's state.
```

At runtime, `get()` cannot modify any non-mutable member of `*this` — the compiler enforces it.

### Why Const Correctness is important
| Benefit | Explanation |
|---------|-------------|
| Safer APIs | Consumers know what is and isn't modified |
| Compiler catches bugs | Can't accidentally mutate through a `const` ref |
| Easier reasoning | Read-only interfaces are clearly documented in the signature |

---

## Topic 37 — Templates

Templates allow **generic programming** — write once, use with any type.

### ❌ Without Templates — Code duplication

```cpp
int add(int a, int b)
{
    return a + b;
}

double addDouble(double a, double b)
{
    return a + b;
}

// Same logic; every type needs a separate function.
```

### ✅ Template Version — Write once, use with any type

```cpp
template<typename T>
T add(T a, T b)
{
    return a + b;
}

// Usage:
// add(2, 3);      → int
// add(2.5, 3.1);  → double
```

### Template Class

```cpp
template<typename T>
class Box
{
    T value;

public:
    Box(T v) : value(v) {}

    T get()
    {
        return value;
    }
};

// Usage:
// Box<int>    b1{10};
// Box<double> b2{3.14};
```

### STL itself uses templates

```cpp
std::vector<int>     vi;   // vector of ints
std::vector<double>  vd;   // vector of doubles

// Same container, different types — powered by templates.
```

---

## Topic 38 — Memory Model (Memory Areas & Layout)

C++ program memory is divided into distinct areas:

| Area | Purpose |
|------|---------|
| **Stack** | Local variables — fast, automatic cleanup |
| **Heap** | Dynamic memory — manual / smart pointer managed |
| **Code** | Program instructions |
| **Data** | Global / static variables |

### Stack — Fast and Automatic

```cpp
int x = 5;   // stored on the stack

// Fast access.
// Automatically freed when the function returns.
```

### Heap — Manual Allocation

```cpp
int* p = new int(5);   // stored on the heap

// Slower access.
// Must be freed (smart pointers do this automatically).
```

### Stack vs Heap
| Stack | Heap |
|-------|------|
| Fast | Slower |
| Automatic cleanup | Manual cleanup (RAII / smart ptrs) |
| Limited size | Large size |

### Memory Alignment

CPU hardware prefers aligned memory — accessing a 4-byte `int` at a 4-byte boundary is cheaper
than at an arbitrary boundary. Compilers may insert **padding** between struct members to
satisfy alignment requirements.

```cpp
struct Test
{
    char a;    // 1 byte
    int  b;    // 4 bytes (compiler may insert 3 bytes padding after 'a')
};
```

### Cache Locality

Contiguous memory is significantly faster to iterate.

- `std::vector` → elements stored contiguously → **cache-friendly**
- Linked list → elements scattered in memory → **less cache-friendly**

Prefer containers with good cache locality when performance matters.

---

## Topic 39 — Object Lifetime

The **lifetime** of an object defines the period during which it exists in memory and may be
accessed safely.

### Local Object — Scoped lifetime

```cpp
void test()
{
    int x = 5;
}                       // ← x is destroyed here; memory reclaimed

// Accessing x after this point → dangling reference
```

### Dangling Pointer — Object already destroyed

```cpp
int* func()
{
    int x = 5;
    return &x;     // BAD: x is destroyed when func() returns
}                  // returning pointer to stack memory

// Use of the returned pointer → undefined behaviour
```

### Static Lifetime — Exists for whole program

```cpp
static int x = 5;   // initialised once; lives until program exits
```

### Temporary Objects

```cpp
std::string("hello");   // temporary — destroyed at the end of the full expression
```

### Lifetime Extension with `const` reference

```cpp
const std::string& ref = std::string("hello");

// The lifetime of the temporary is EXTENDED to match the reference's scope.
// Safe — no dangling reference.
```

---

## Topic 40 — Undefined Behavior (UB)

**Undefined Behaviour** is the most dangerous concept in C++ — when UB occurs, **the compiler
is free to do anything**: crash, silently corrupt memory, optimise away code, or produce
misleading results.

### Examples of UB

| # | Code | Reason |
|---|------|--------|
| 1 | `int x; std::cout << x;` | Reading uninitialised variable |
| 2 | `int a[3]; a[5] = 10;` | Out-of-bounds array access |
| 3 | `int* p = new int(5); delete p; std::cout << *p;` | Use-after-free |
| 4 | `int x = INT_MAX; x++;` | Signed integer overflow |
| 5 | `int a[3]; delete[] a;` | `delete[]` on stack-allocated array |

### ❌ Example 1 — Uninitialised variable

```cpp
int x;
std::cout << x;     // UB — x has indeterminate value
```

### ❌ Example 2 — Out of bounds

```cpp
int arr[3];

arr[5] = 10;        // UB — index 5 is beyond the array
```

### ❌ Example 3 — Use after free

```cpp
int* p = new int(5);

delete p;

std::cout << *p;    // UB — p now a dangling pointer
```

### ❌ Example 4 — Signed integer overflow

```cpp
int x = INT_MAX;

x++;                // UB — signed overflow is not defined
```

### How to Avoid UB
- Use **smart pointers** — automatic lifetime management
- Use **STL containers** — bounds-checked access (`at()`) available
- Run **sanitizers** (`-fsanitize=address,undefined`)
- Pay attention to **compiler warnings** (`-Wall -Wextra`)
- Follow **modern C++ rules** — rules of thumb in this document cover the most common cases

---

## Topic 41 — Modern C++ (C++11 → C++23)

A brief overview of the evolution of Modern C++ and the most impactful features by standard.

### C++11 — The game-changer

| Feature | Example |
|---------|---------|
| `auto` type deduction | `auto x = 5;` |
| Range-based `for` | `for (auto n : nums) …` |
| Lambda expressions | `[](int x){ return x * 2; }` |
| Smart pointers | `std::unique_ptr`, `std::shared_ptr` |
| Move semantics | `std::move` |

### C++14 — Refinements

- Improved lambdas (generic captures)
- More relaxed `constexpr` (loops, local variables now allowed)

### C++17 — Major quality-of-life

| Feature | Example |
|---------|---------|
| Structured bindings | `auto [a, b] = pair;` |
| `if constexpr` | Compile-time branching |
| `std::optional` | `std::optional<int> maybe;` |
| `std::variant` | Type-safe union |
| `std::string_view` | Non-owning string slice |
| Parallel STL | `std::execution::par` |

### C++20 — The largest release

| Feature | Example |
|---------|---------|
| Concepts | `template<std::integral T> T add(T a, T b);` |
| Ranges | `std::ranges::sort(v);` |
| Coroutines | Async / generator programming |
| `std::span` | View over contiguous sequence |
| `std::format` | Python-style string formatting |
| `std::jthread` | Auto-joining thread |

### C++23 — The current standard (GCC 13+, Clang 16+)

| Feature | Purpose |
|---------|---------|
| `std::expected<T, E>` | Error handling without exceptions |
| `std::mdspan` | Multi-dimensional span |
| Deducing `this` | Cleaner member function declarations |
| `std::print` | Compile-time checked `printf`-style output |

---
