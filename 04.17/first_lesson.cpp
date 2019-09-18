// Core language is biased to deduce initializer list

#include <iostream>
#include <vector>

using namespace std;

// struct Foo {
//     Foo (int a, int b) {
//         cout << "I have two integer parameters passed." << endl;
//     }
// 
//     Foo (initializer_list<int> list) {}
// };

struct RunParallel { };

struct Foo {
    // instead of this
    // Foo(int a, RunParallel rp) { };
    // Foo(RunParallel rp, int a) { };
    // Foo(int a) { };
    // Foo(RunParallel rp) { };
   
    // Foo() { };
    Foo() = default;

    // do this
    template<typename First, typename ... Rest>
    Foo(const First& first, const Rest& ... rest) : Foo(rest...) {
        // if constexpr(is_same<First, RunParallel>::value)
        if constexpr(is_same_v<First, RunParallel>) {
            cout << "I am running in multi threaded fashion!" << endl;
        }
    }
};

namespace Bad {
    // old school.
    // resolves in runtime.
    // actually what printf does
    // void printAll(const char* title, ...) {
    // runtime va_list if not in Template context
    void print(const char* title...) {
        va_list args;

        // after title, list start
        va_start(args, title);
        while (true) {
            // this works because va_arg is macro which
            // opens to something else that makes it work!
            int value = va_arg(args, int);
            // segfault if can't see -1
            if (value == -1) break;
            cout << value << endl;
        }
    }
};

namespace Better {
    // // doing nothing if no arguments passed
    // // this covers the recursion base case!
    // // will be written inside print(First...)
    // void print() { } 

    // templatised and recursion
    // typename... -> a lot of typenames (... -> flow operator)
    // First -> type
    // Rest -> parameter pack of compiler types
    template<typename First, typename... Rest>
    // iinline, compiler will open up everything
    // and fuse everything together in order to make the
    // operation in one line
    // which will be optimised to --> cout << 10 << endl << "hello" << endl << 20.0 << endl;
    // but this still can be optimised!
    inline void print(First first, Rest... rest) {
        cout << first << endl;
        // if we still have rest
        // we have to make if compile time if,
        // therefore we use constexpr.
        // if we haven't added that, the print()
        // function will be tried to created, which
        // doesn't actually exist
        // We did this in order to delete void print() { }
        if constexpr(sizeof...(Rest) > 0) {
            // expand rest
            // parameter pack expansion.
            // you can't use for loop over parameter packs!
            print(rest...);
        }
    }
};

namespace EvenBetter {
    template<typename T>
    inline void _print(const T& value) {
        cout << value << endl;
    }

    template<typename ... Ts>
    inline void print_meta(Ts ... values) {
        // parameter pack expansion
        // (void)initializer_list<int> { (_print(10), 0), (_print("hello"), 0), (_print(20.0), 0) };
        // same as above!
        (void)initializer_list<int> { (_print(values), 0)... };
    }

    // lambda expression
    // auto print = [](auto ... values) { ((cout << values << endl), ...); };

    template<typename ... Ts>
    inline void print(Ts ... values) {
        // fold expression!
        // (cout << ... << values);
        // implicit for loop!
        // ((cout << values[0] << endl), (cout << values[1] << endl), (cout << values[2] << endl));
        // almost same as above! (almost same because [] is not defined in parameter pack)
        ((cout << values << endl), ...);
    }

};

int
main (int argc, char** argv) {
    // auto foo = Foo(10, 20);
    {
        auto v1 = vector<int>{10, 20, 30};      //  a vector containing 3 integers whose values are 10, 20, 30
        auto v2 = vector<int>{10};              //  a vector containing 1 integer whose value is 10.
        auto v3 = vector<int>(10);              //  a vector containing 10 integers whose values are 0.
        // Q: define your own initializer list!
        auto a = std::initializer_list<int>{1, 5, 10};
      
        // no type.
        // {10, 20, 30} is now deduces as initializer list
        // for(int a : {10, 20, 30})
        //     cout << a << endl;
    }

    // Bad::print("title", 500, 100, 200, -1);
    
    // int, const char*, double -> First == int, Rest == (const char*, double)
    // Better::print(10, "hello", 20.0);
    // Better::print("hello", 20.0);
    // Better::print(20.0);
    // Better::print(); --> can't compile if we don't have a end case!
    
    {
        // int i, j, k;
        // i = 5, j = i, k = i + j     // comma trick. Last one will be returned
        // we are using this trick in (EvenBetter::_print("hello"), 0) which in
        // any case returns 0!

        // template meta programming
        // auto z = initializer_list<int>{ (EvenBetter::_print("hello"), 0) };
        // Warning: result unused if we don't cast to void!
        // we are saying that this returns something and I don't care what it is,
        // don't give me a warning!
        // we are putting 0 because _print returns void. this is abusive to
        // compiler.
        // (void)initializer_list<int>{ (EvenBetter::_print("hello"), 0) };      
    }

    // EvenBetter::print(10, "hello", 20,0);

    // auto f1 = Foo{10, 20, RunParallel{}, 30, RunParallel{}};

    return 0;
}
