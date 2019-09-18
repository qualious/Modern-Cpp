// CRTP: curiously recurring template pattern
// Used in library design, used instead of dynamic polymorphism

// union, std::variant
// std::map, std::any,
// structured bindings

#include <iostream>
#include <vector>

using namespace std;

// This makes a runtime overhead
// Creates vptr, v-table, etc..
namespace OldSchool {
    struct Base { 
        // you have to implement print! 
        virtual void print() const = 0;
    };

    struct Foo : public Base {
        void print() const override {
            cout << "Foo" << endl;
        }
    };

    struct Bar : public Base {
        void print() const override {
            cout << "Bar" << endl;
        }
    };
} // end namespace OldSchool

// if we know this in compile time,
// in order to make it more efficient
// use this.
namespace Better {
    template<typename Derived>
    class Base {
        Base() { };
    public:
        void print() const {
            // this is Base*, but we already know that it is a Derived*
            ((Derived*)this)->PRINT();
        }
    };

    struct Foo : public Base<Foo> {
        void PRINT() const {
            cout << "Foo" << endl;
        }
    };

    struct Bar : public Base<Bar> {
        void PRINT() const {
            cout << "Bar" << endl;
        }
    };
} // end namespace Better


int main(void) {
    {
        auto f1 = OldSchool::Foo{};
        auto f2 = OldSchool::Foo{};
        auto b1 = OldSchool::Bar{};

        auto v = vector<OldSchool::Base*>{&f1, &f2, &b1};

        // dynamic polymorphism
        // v-table makes this work. 
        // Resolved in runtime
        for (auto ptr : v) {
            ptr->print();
        }
    }

    {
        auto f1 = Better::Foo{};
        f1.print();
        auto f2 = Better::Foo{};
        f2.print();
        auto b1 = Better::Bar{};
        b1.print();
    }

    return 0;
}
