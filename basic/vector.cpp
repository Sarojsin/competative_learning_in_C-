// =============================================================================
// VECTOR ESSENTIALS FOR CODING COMPETITIONS - TEACHING VERSION
// =============================================================================
// This file teaches the most important std::vector usage patterns used in
// competitive programming (Codeforces, AtCoder, LeetCode, etc.).
// 
// std::vector is a dynamic array that can grow and shrink at runtime.
// It is the #1 container you will use in 90% of problems.
// 
// Key advantages over raw arrays (int arr[100]):
//   - Automatic memory management (no manual new/delete)
//   - Can change size dynamically
//   - Rich set of methods + works with STL algorithms
// =============================================================================

#include <bits/stdc++.h> 
// <bits/stdc++.h> is a GCC-specific header that includes ALL standard C++ headers.
// In competitive programming we use it to save time typing individual includes.
// It brings: vector, algorithm (sort, unique, lower_bound...), iostream, etc.
// WARNING: Never use in production code or other compilers - it is non-standard.

using namespace std;
// "using namespace std;" lets us write "vector" instead of "std::vector"
// and "cout" instead of "std::cout".
// In small contest files this is acceptable and saves typing.
// In large projects we avoid it to prevent name clashes.

int main() {
    // ========================================================================
    // SECTION 1: BASIC VECTOR CREATION AND push_back
    // ========================================================================
    // Syntax: vector<datatype> name;
    // vector is a template class: vector<T> where T is the element type.
    // Most common: vector<int>, vector<long long>, vector<pair<int,int>>, vector<vector<int>>
    vector<int> v;                    // creates an empty vector of integers

    // push_back(value) appends a new element at the END of the vector.
    // Time complexity: amortized O(1) - occasionally O(n) when reallocation happens.
    v.push_back(1);                   // v = [1]
    v.push_back(2);                   // v = [1, 2]
    v.push_back(3);
    v.push_back(4);                   // v = [1, 2, 3, 4]

    // size() returns the number of elements currently stored (not capacity).
    // Return type is size_t (unsigned integer type, usually 64-bit).
    cout << "size=" << v.size() << "\n";   // prints: size=4

    // ========================================================================
    // SECTION 2: RANGE-BASED FOR LOOP (C++11 "for-each")
    // ========================================================================
    // Modern and clean way to iterate over containers.
    // Syntax: for (element_type variable : container) { ... }
    // "auto" keyword lets compiler deduce the type automatically.
    // Here each element is copied into "pt" (good for small types like int).
    cout << "Elements (range-based for): ";
    for (auto pt : v) {
        cout << pt << " ";            // prints each element separated by space
    }
    cout << "\n";

    // ========================================================================
    // SECTION 3: CLASSICAL ITERATOR-BASED LOOP
    // ========================================================================
    // Every STL container provides iterators.
    // vector<int>::iterator is the type, but we use "auto".
    // begin() returns iterator to FIRST element.
    // end()   returns iterator to "one-past-the-last" element (sentinel).
    // We dereference with *it to get the value.
    // This style is useful when we need to modify elements or use erase/insert.
    cout << "Elements (iterator loop):   ";
    for (auto it = v.begin(); it != v.end(); ++it) {
        cout << *it << " ";           // *it gives the int at current position
    }
    cout << "\n";

    // ========================================================================
    // SECTION 4: ACCESSING FIRST AND LAST ELEMENTS
    // ========================================================================

//     [v[0]] [v[1]] [v[2]] [v[3]] [v[4]]  [end()]
//       ↑                                     ↑
//    begin()                                end()
    // v.begin()   -> iterator to first element
    // v.end()     -> iterator PAST the last element 
    // *(v.end() - 1) is a common idiom to get last element via iterator arithmetic.
    // Safer modern alternatives: v.front() and v.back()  (shown later)
    cout << "first=" << *v.begin() << " last=" << *(v.end() - 1) << "\n";

    // ========================================================================
    // SECTION 5: INITIALIZER LIST + SORT + front/back
    // ========================================================================
    // We can initialize directly with curly braces (C++11).
    vector<int> a = {5, 3, 9, 1, 5};

    cout << "Before sort: ";
    for (int x : a) cout << x << " ";
    cout << "\n";

    // sort(first_iterator, last_iterator) from <algorithm>
    // Sorts the range [first, last) in ASCENDING order by default.
    // Uses IntroSort (hybrid of quicksort + heapsort + insertion sort).
    // Time: O(n log n) average and worst-case.
    sort(a.begin(), a.end());

    cout << "After sort:  ";
    for (int x : a) cout << x << " ";
    cout << "\n";

    // front() returns REFERENCE to the first element (no bounds check)
    // back()  returns REFERENCE to the last element  (no bounds check)
    // Both are O(1).
    cout << "min=" << a.front() << " max=" << a.back() << "\n";

    // ========================================================================
    // SECTION 6: reserve() vs capacity() - PERFORMANCE OPTIMIZATION
    // ========================================================================
    // When you know approximate size in advance, call reserve() to avoid
    // multiple expensive reallocations during push_back.
    // capacity() = how many elements memory is currently allocated for.
    // size()     = how many elements are actually stored.
    vector<int> b;
    b.reserve(100);                   // pre-allocate space for at least 100 ints

    for (int i = 0; i < 100; ++i) {
        b.push_back(i);
    }

    // After this loop: size == 100, capacity >= 100 (often exactly 100 or more)
    cout << "b.size=" << b.size() << " cap=" << b.capacity() << "\n";

    // ========================================================================
    // SECTION 7: REMOVING DUPLICATES - unique() + erase() IDIOM
    // ========================================================================
    // unique() (from <algorithm>) requires the range to be SORTED first.
    // It moves all unique elements to the front and returns an iterator to
    // the new logical end (first duplicate onward).
    // It does NOT actually shrink the vector - the "garbage" remains at the end.
    // Therefore we ALWAYS combine it with erase( new_end, old_end ).
    vector<int> c = {1, 2, 2, 3, 4, 4, 4, 5};
    sort(c.begin(), c.end());         // must sort before unique
    c.erase( unique(c.begin(), c.end()), c.end() );
    // After this: c = [1,2,3,4,5]  and size() == 5
    cout << "After unique: size=" << c.size() << "  elements=";
    for (int x : c) cout << x << " ";
    cout << "\n";

    // ========================================================================
    // SECTION 8: SEARCHING IN SORTED VECTOR
    // ========================================================================
    // binary_search(first, last, value) - returns true if value exists.
    // Requires the range to be sorted in ascending order.
    // Time: O(log n)
    bool has3 = binary_search(c.begin(), c.end(), 3);
    cout << "binary_search for 3: " << (has3 ? "found" : "not found") << "\n";

    // lower_bound(first, last, value)
    // Returns iterator to the FIRST position where value can be inserted
    // without violating sorted order (i.e. first >= value).
    // If all elements < value, returns end().
    // Time: O(log n)
    auto lb = lower_bound(a.begin(), a.end(), 6);
    // Convert iterator to 0-based index using pointer arithmetic:
    // iterator - begin() gives the distance.
    size_t index = lb - a.begin();
    cout << "lower_bound(6) index = " << index << "\n";

    // upper_bound would give first strictly > value.

    // ========================================================================
    // SECTION 9: VECTOR OF PAIRS - VERY COMMON IN CONTESTS
    // ========================================================================
    // pair<T1, T2> is a simple struct holding two values.
    // vector<pair<int,int>> is the standard way to store (x, y) points,
    // edges (u, v), or (value, index) for sorting with original positions.
    vector<pair<int, int>> vp = {{2, 3}, {1, 5}, {2, 2}};

    // sort on vector of pairs sorts first by .first, then by .second.
    sort(vp.begin(), vp.end());

    cout << "Sorted pairs: ";
    for (auto &p : vp) {
        cout << "(" << p.first << "," << p.second << ") ";
    }
    cout << "\n";

    // front() on vector of pairs returns a reference to the pair.
    cout << "Smallest pair after sort: " << vp.front().first << "," << vp.front().second << "\n";

    // ========================================================================
    // SECTION 10: PREFIX SUM (CUMULATIVE SUM) - CLASSIC TECHNIQUE
    // ========================================================================
    // Prefix sum lets us answer range sum queries in O(1) after O(n) preprocessing.
    // pre[i+1] = sum of first i elements (0-based).
    // Then sum from L to R inclusive = pre[R+1] - pre[L]
    vector<long long> pre(v.size() + 1, 0);   // +1 because we want pre[0] = 0
    for (size_t i = 0; i < v.size(); ++i) {
        pre[i + 1] = pre[i] + v[i];
    }
    cout << "Prefix sum array: ";
    for (auto x : pre) cout << x << " ";
    cout << "\n";
    cout << "Total sum (pre.back()) = " << pre.back() << "\n";

    // ========================================================================
    // BONUS QUICK REFERENCE - OTHER COMMON VECTOR OPERATIONS
    // ========================================================================
    // pop_back()     -> removes last element in O(1)
    // emplace_back() -> constructs element in place (more efficient than push_back for objects)
    // insert(pos, val) -> inserts before position (O(n) shift)
    // erase(pos)     -> removes at iterator position (O(n) shift)
    // resize(n)      -> changes size, may default-construct or truncate
    // clear()        -> removes all elements, size becomes 0 (capacity may stay)
    // empty()        -> returns true if size() == 0
    // at(index)      -> access with bounds checking (throws out_of_range)
    // reverse(first, last) from <algorithm>
    // max_element(first, last) -> returns iterator to largest element
    // min_element(first, last) -> returns iterator to smallest element
    // accumulate(first, last, init) from <numeric> -> sum (or other reduction)

    // Example of a few bonus operations:
    vector<int> demo = {10, 20, 30};
    demo.pop_back();                  // now [10, 20]
    demo.emplace_back(40);            // more efficient push
    reverse(demo.begin(), demo.end()); // [40, 20, 10]
    auto maxIt = max_element(demo.begin(), demo.end());
    cout << "max in demo = " << *maxIt << "\n";

    // ========================================================================
    // 2D VECTORS (MATRICES / GRIDS)
    // ========================================================================
    // Very common in graph and grid problems.
    // vector<vector<int>> grid(rows, vector<int>(cols, initial_value));
    vector<vector<int>> grid(3, vector<int>(4, 0));   // 3 rows, 4 columns, all 0s
    grid[1][2] = 7;                                   // set middle-ish cell
    cout << "grid[1][2] = " << grid[1][2] << "\n";
 
    map<string, string> mp;   // Just to show that vectors work with any type, including pairs and maps.
    mp["key"] = "value";
    cout << "mp[\"key\"] = " << mp["key"] << "\n";
    mp["1"] = "value2";
    mp["2"] = "value3";
    mp["3"] = "value4";
    cout << "mp[\"1\"] = " << mp["1"] << "\n";
    for(auto it: mp){
        cout<<it.first<<" "<<it.second<<"\n";
    }

    return 0;
}
