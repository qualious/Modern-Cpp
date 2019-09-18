// Short string optimization
//
// std::string s = "x";
// size, memptr, and string itself is hold as 
// union in memory. 

#include <iostream>
#include <variant>
#include <vector>

using namespace std;

union Value { 
    int i;
    float f;
    double d;
};

enum ValueType { INT, FLOAT, DOUBLE };

struct Foo { 
    Value value;
    ValueType type;

    explicit Foo(int v) : type{INT} { value.i = v; }
    explicit Foo(float v) : type{FLOAT} { value.f = v; }
    explicit Foo(double v) : type{DOUBLE} { value.d = v; }

    void print() const {
        if (type == INT) cout << value.i << endl;
        else if (type == FLOAT) cout << value.f << endl;
        else if (type == DOUBLE) cout << value.d << endl;
    }
};

int
main(void) {
//     auto value = Foo{};
//     value.i = 10;
//     cout << value.i << endl;
//     cout << value.f << endl;
//     cout << endl;
// 
//     value.f = 3.14f;
//     cout << value.i << endl;
//     cout << value.f << endl;
//     cout << endl;
    
    auto a = Foo{10};
    a.print();

    a.value.f = 3.14f;
    a.type = FLOAT;
    a.print();

    // USE VARIANT INSTEAD

    auto v = variant<int, float, double>{};
    v = 10;
    v = 3.14f;
    v = 8.0;

    try { 
        cout << std::get<double>(v) << endl;
    } catch(...) {
        cout << "Error!" << endl;
    }
    
    std::visit([](auto&& value) {
        using T = std::decay_t<decltype(value)>;

        if constexpr(std::is_same_v<T, int>) cout << "int" << endl;
        else if constexpr(std::is_same_v<T, float>) cout << "float" << endl;
        else if constexpr(std::is_same_v<T, double>) cout << "double" << endl;
    }, v);
   
    using VAR = variant<int, float, double, const char*>;
    auto vec = vector<VAR>{3, 10.0, 3.14f, "hello"};

    for (auto&& var : vec) {
        std::visit([](auto&& value) {
            using T = std::decay_t<decltype(value)>;

            if constexpr(std::is_same_v<T, int>) cout << "int" << endl;
            else if constexpr(std::is_same_v<T, float>) cout << "float" << endl;
            else if constexpr(std::is_same_v<T, double>) cout << "double" << endl;
            else if constexpr(std::is_same_v<T, const char*>) cout << "string" << endl;
        }, var);
    }

    return 0;
}
