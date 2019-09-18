//  Copy-Swap Idiom:
//  -   Any class that manages a resource )a wrapper, like a smart pointer)
//  needs to implement The Big Three. (And The Big Four after C++11: move semantics)
//  Conceptually, it works by using the copy-constructor's functionality to
//  create a local copy of the data, then takes the copied data with a swap
//  function, swapping the old data with the new data. The temprorary copy then
//  destructs, taking the old data with it. 

// RWO: return value optimization
// CRTP: curiously reucurring template pattern

#include <iostream>
#include <variant>

using namespace std;

struct Dummy {
    size_t size;
    int* mem;

    // default cotr
    Dummy() : size{0}, mem{nullptr} {
        cout << "Default cotr" << endl;
    };
    
    // Copy-Cotr
    Dummy(const Dummy& other) : size{other.size} {
        cout << "Copy ctor" << endl;
        mem = new int[size];
        std::copy(other.mem, other.mem + other.size, mem);
    }

    // Destructor
    ~Dummy() { 
        cout << "Destuctor" << endl;
        delete[] mem;
        size = 0;
    }

    friend void swap(Dummy& first, Dummy& second) {
        using std::swap;
        swap(first.size, second.size);
        swap(first.mem, second.mem);
        // std::swap(first.size, second.size);
        // std::swap(first.mem, second.mem);
    }

    // Move-Ctor
    Dummy(Dummy&& other) : Dummy() { 
        cout << "Move cotr" << endl;
        swap(*this, other);
    };

    // Compiler selects the correct cotr.
    // It decides between copy and move cotr.
    void operator=(Dummy other) {
        cout << "Assignment" << endl;
        swap(*this, other);
    }

    //  void operator=(const Dummy& other) {
    //     auto temp = other;  // copy cotr, not copy assignemnt
    //     swap(*this, temp);
    // } // copy assign

    // void operator=(const Dummy&& other) {
    //     swap(*this, other);
    // } // move assign
};

// Compiler optimizes the code.
// It understands that c is actually
// d here. 
// this is called return value optimization
inline 
Dummy func(int i) { 
    auto d = Dummy{};
    return d;
}

int main(void) { 
    auto a = Dummy{};
    auto b = Dummy{};

    auto c = func(10);

    return 0;
}
