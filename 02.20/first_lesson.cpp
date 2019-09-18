//  variables, function and scope
//  primitive types
//  local, global, static variables
//  free, member, static functions
//  functions return types
//
//  auto
//
//  console: std::cout, std::cin
//
//  function overloading
//  operator overloading
//  
//  struct, class, ctor/dtor
//  inheritance
//
//  Preprocessor is wrong because it burdens the compiler 
//  UB --> Undefined Behavior

#include <iostream>             // # is preprocessor sign

int a;                          // if bit is 1, it's negative, if 0 it's positive.
unsigned int b;                 // we gain one extra bit. no nagative values. 
unsigned int c{0};              // universal initialization. Not c = {0}    
                                // universal initilization is better in modern C++. 
                                // C++11 and up

unsigned short d = 65536;       // this will overflow and the result will be 0
unsigned short e = 65537;       // this will overflow and the result will be 1
double f = 65537.5;             // double
float g = 65537.5f;             // float
                                // double can have double the amount of
                                // precision

void doNothing();               // forward declaration

void
doIt() {
    std::cout << a << std::endl;
    std::cout << ++a << std::endl;          // undefined behavior if a is not assumed as 0
    std::cout << a << '\n' << std::flush;   // equivalent with top 
    std::cout << d << std::endl;            // 0
    std::cout << e << std::endl;            // 1
    std::cout << f << std::endl;            // 65537.5
    std::cout << g << std::endl;            // 65537.5
    {
        using namespace std;
        cout << "This scope has std context" << endl;
    }
}

int
main(int argc, char* argv[]) {
    doIt();
    doNothing();
    return 0;
}

void
doNothing() {
    { 
        using namespace std;
        cout << "Doing nothing" << endl;
    }
}
