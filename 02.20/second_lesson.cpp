#include <iostream>
#include <vector>

// Free functions       ->  Doesn't bind to any class, old school c func.
// Member functions     ->  Function that belongs to class or struct

static int globalVariable = 0;      // this global variable can be accessed
                                    // from this .cpp file only

void
doIt() {
    static int v = 100;     // in the actuality, this is global.
                            // but compiler protects other functions from
                            // accessing it. Initialized only once when they are
                            // first used
    static bool isInitialized = false;
    if (!isInitialized) {
        isInitialized = true;
        std::cout << "Now initializing..." << std::endl;
    }
    {
        using namespace std;
        cout << ++v << endl;
    }
}

int
jump(int height) {      // height is local variable
    return height / 2;
}

// Standart
std::vector<std::vector<int>>
someFunction(){
    std::vector<std::vector<int>> intVector;
    return intVector;
}

// C++11
auto someFunction11() -> std::vector<std::vector<int>> {
    std::vector<std::vector<int>> intVector;
    return intVector;

}

// C++14
auto someFunction14() {
    return 0;
}

// struct {        // anonymous struct
//  int x;        // must be a class member
//  int y;
// };

// typedef struct {
//     int x;
//     int y;
// } Coordinate;

// starts with protected.
// class Coordinate {
// public:
//     int x;
//     int y;
// }

// starts with public
struct Coordinate {
    int x = 0;      // initialize to 0
    int y = 0;

    void print() {
        std::cout << "This is: " << this << std::endl;
        // arrow because x and y is not object instance itself.
        // std::cout << "x: " << this->x << ", y: " << this->y << std::endl;
        // if int x = 5;, it'll print 5 and ignore the actual memory value
        int x = 5;
        std::cout << "x: " << x << ", this->x: " << this->x << ", y: " << y << std::endl;
    }
};

void
print(Coordinate coord) {
    std::cout << "x: " << coord.x << ", y: " << coord.y << std::endl;
}

// overloading print
void
print() {
    std::cout << "Hi there, how are you?" << std::endl;
}

// creating a new type
typedef int MyType;
using MyAnotherType = int;

int
main(int argc, char* argv[]) {
    MyType i = 15;
    MyAnotherType j = 25;
    std::cout << "MyType i: " << i << std::endl;
    std::cout << "MyAnotherType j: " << j << std::endl;

    // Standart
    Coordinate first_coord;
    first_coord.x = 100;
    first_coord.y = 200;
    print(first_coord);
    first_coord.print();

    // C++14
    Coordinate second_coord{100, 200};
    print(second_coord);
    second_coord.print();

    print();

    // doIt();
    // doIt();
    // doIt();

    int actual_height = jump(10);   // when you call this, a stack frame is created.
                                    // 10 is pushed to stack first. Destroyed
                                    // after execution
    return 0;
}
