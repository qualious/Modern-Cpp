#include <iostream>
#include <variant>

using namespace std;

class Bar;
class Foo { 
    int value;
public:
    Foo(int v) : value{v} {};
    // this function can access private entities 
    // because its a friend
    friend bool getValues(const Foo&, const Bar&);
    // int getValue(int value) { value = value; }
};

class Bar {
    int value;
public:
    Bar(int v) : value{v} {};  
    // this function can access private entities 
    // because its a friend
    friend bool getValues(const Foo&, const Bar&);
    // int getValue(int value) { value = value; }
};

bool getValues(const Foo& f, const Bar& b) {
    return f.value > b.value;
}

int
main(void) {
    auto bar = Bar{15};
    auto foo = Foo{10};
    cout << "getValues(foo, bar): " << getValues(foo, bar) << endl;
    return 0;
}
