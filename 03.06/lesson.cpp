// Compiler --std=C++11 automatically.

// Almost always auto - AAA rule: move type to the right
//
// C: free/malloc, old C++: new/delete
//
// l-value, r-value
// l-value references, r-value references
// double references
// move-ctor, move-assignment
//
// operator overloading
// user-defined literals: 9 m/s^2
//
// inheritance
// dynamic polymorphism, v-table

// pre C++11
//
// auto
// static
// volitile ->  embeded systemlerde kullaniliyor.
// register ->  hiz kazanmak icin yapiyor. memory'de bulunan
//              bir register'a direkt erisim ve yazma (L1 cache'den hizli)
//              compiler optimize ettigi icin gerek yok.

// post C++11
//
// auto     ->  automatically deduced.
// static   ->
// volitile ->  Compiler'a bu degiskenin degeri senin kontrolun olmadan
//              her an degisebilir diyoruz. Makes compiler cautious.
// register ->



#include <iostream>
#include <vector>
#include <array>

struct Int {
    int value;

    Int() : value{0} { }
    Int(int value) : value{value} { }
    Int(const Int& other) : value{other.value} { }

    // Int a(100);
    // int b = a;
    operator int() { return value; }

    operator float() { return value * 2; }
};

struct Vector2d {
    Int x;
    Int y;
};

struct Huge {
    Int x;
    Int y;
    // move cotr used if we don't want to copy the whole vector
    // to a new memory.
    // We just take (cut) the v from the `other` class and make
    // it our own.
    std::vector<int> v;

    Huge() : x{10}, y{20}, v(100) {
        // std::cout << "ctor" << std::endl;
    }

    // copy ctor.
    // If move contructor is not defined, this will be called.
    Huge(const Huge& other) : x{other.x}, y{other.y}, v(other.v) {
        // std::cout << "copy ctor" << std::endl;
    }

    // move ctor
    // move ctor will not be called unless strictly specified
    // std::move deduces type, casts to double ref and returns
    // v((std::vector<int>&&)other.v) === v((std::move(other.v))
    Huge(Huge&& other) : x{std::move(other.x)}, y{std::move(other.y)}, v((std::move(other.v)) {
        std::cout << "move ctor" << std::endl;
    }

    void
    operator=(const Huge& right) {
        x = right.x;
        y = right.y;
        v = right.v;

        // std::cout << "copy assignemnt" << std::endl;
    }

    void
    operator=(Huge&& right) {
        x = std::move(right.x);
        y = std::move(right.y);
        v = std::move(right.v);

        // std::cout << "move assignemnt" << std::endl;
    }

   ~Huge() {
        // std::cout << "deleting.." << std::endl;
    }
};

int f1() { return 10; }
int f2() { return 20; }

struct Acceleration {
    long double value;
    Acceleration(long double value) : value{value} { }
}

// user defined literal

// Acceleration operator"" _acc(double value) {
//     return Acceleration{value};
// }

// underscore is necessary

auto operator"" _ms2(long double value) -> Acceleration {
    return {value};
}

auto operator"" _ms2(unsigned long long value) -> Acceleration {
    return {(long double)value};
}

int
main(int argc, char* argv[]){

    {
        // user literals
        auto g = Acceleration{9.81};
        auto g = 9.81_ms2;
    }

    {
        // Int a(100);
        auto a = Int(100);

        // int b = a;
        auto b = (int)a;            // 100
        auto c = (float)a;          // 200

    {
        auto h1 = Huge{};           // ctor
        Huge h2(h1);                // copy construction
        Huge h3((Huge&&)Huge{});    // same as line--, but it uses temp
                                    // move ctor
        Huge h4(std::move(Huge{})); // move ctor
    }

    // AAA -> Almost always auto
    auto h = Huge{};        // Huge();
    // Huge h;              // default contruction

    auto h2 = Huge{h};
    // Huge h2(h);          // copy-construction

    auto h3 = h2;
    // Huge h3 = h2;        // copy-assignment

    auto h4 = Huge{};
    // Huge h4;
    h4 = h3;

    //========================================================================

    // C

    // a pointer to memory. type is unknown.
    void* p1 = malloc(250000 * sizeof(int));
    free(p1);

    // pointed type is int
    int* p2 = (int*)malloc(250000 * sizeof(int));
    free(p2);

    // pointed type is int. Auto works only if right hand type is ptr to smt
    auto* p3 = (int*)malloc(250000 * sizeof(int));
    free(p3);

    // we did not specified that right hand size should be pointer. this is the
    // same as above. but above throws failure if right hand side is not
    // pointer
    auto p4 = (int*)malloc(250000 * sizeof(int));
    free(p4);

    //========================================================================

    // OLD C++

    // pointed type is int
    int* p2_cpp = new int[250000];
    // auto p2_cpp = new int[250000];   // left hand modern, right hand old school!
    delete[] p2_cpp;

    Huge* p5 = new Huge();
    delete p5;                          // calls ~Huge();

    Huge* p6 = new Huge[1000];          // uses default cotr.
    delete[] p6;

    //========================================================================

    // Modern C++

    // vectors allocated from Heap memory
    // std::list -> her eleman baska bir memory adresinden aliniyor.
    // v1 itself is in Stack memory. Pointers inside points to heap memory.
    // Huge itself and it's properties is in heap memory
    std::vector<Huge> v1(1000);         // when v1 goes out of scope, this is
                                        // destroyed automatically.
    // we don't care about delete when using vector.

    // h is in stack. therefore h.x, h.y and h.v is in stack.
    // but the pointers inside v is inside heap
    // auto h = Huge{};
    // Huge{};  -> r-value, temporary

    {
        // if you can get address of it, it is a l-value
        auto v1 = f1();     // returned value from f1 is r-value
        auto v2 = f2();     // returned value from f2 is r-value
        // v1, v2 and v3 are l-value
        auto v3 = v1 + v2;

        // not safe
        int* v1_ptr = &v1;      // C and old C++
        // old school, doesn't allow compiler optimization
        v1_ptr++;
        ++v1_ptr;
        v1_ptr += 10;

        // safe
        auto& v1_ref_cpp = v1;  // both old and modern C++

        // int& ir = 10;    // can't do this because 10 is r-value

        const int& ir_const = 10;   // I'll only use this pointer as read only
                                    // I'll not write to it.
                                    // When this happens, compiler creates a temp
                                    // with a random name for you

        int a = 10;
        int& ir = a;
        ir = 20;            // a = 20

        int&& irr = 10;     // r-value referance


        // int val = 100;
        // int&& irr2 = val;   // not okay.

        // don't put values of f1 and f2 in memory
        // we tell compiler that make the calculation as
        // smart and efficiently as possible
        auto v4 = f1() + f2();
    }

    // std::array does run time bound check if it's in debug mode
    // in release mode bound checking is abondoned

    {
        // C
        int L[100];     // allocated in stack
        L[0] = 1000;
        // L[100] = 2000;  // out of bounds, UB
    }

    {
        // Modern C++
        auto L = std::array<int, 100>{};
        L[0] = 1000;
        L[100] = 2000;  // out of bounds, UB
                        // caught in run time if compiled with debug mode
    }

    return 0;
}
