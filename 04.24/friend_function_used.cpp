//  Friend functions
//  ADL: Argument Dependent Lookup
//  Always prefer non-member (free) non-friend functions first!
//  Copy-Swap Idiom:
//  -   Any class that manages a resource )a wrapper, like a smart pointer)
//  needs to implement The Big Three. (And The Big Four after C++11: move semantics)
//  Conceptually, it works by using the copy-constructor's functionality to
//  create a local copy of the data, then takes the copied data with a swap
//  function, swapping the old data with the new data. The temprorary copy then
//  destructs, taking the old data with it. 

#include <iostream>
#include <variant>

using namespace std;

class Bar;
class Foo {
    int value;
public:
    Foo() { }
    friend void setValue(Foo& f, int value) {
        f.value = value;
    }
};

int 
main(void) {
    auto f = Foo{};
    // f.setValue(10);
    // Argument Dependent Lookup: 
    // setValue is found through ADL, because of its 1st parameter.
    // ADL always takes priority
    setValue(f, 10);
    
    return 0;
}
