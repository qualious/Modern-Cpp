// functional programming: what is it?
// to "if" or not to "if"
// relations to overload resolution and template specializations (to follow)

// working with types: Type Displayer trick.
// template specializations (a functional programming primer)
// is_same, remove_const, remove_volatile, remove_cv, remove_reference, decay

// good to know: template-template types, template auto, type constructors
// typedef     == type aliasing
// std::vector == type constructor

#include <iostream>
#include <functional>
#include <stdint.h>

#include "Matrix.h"

using namespace std;

// auto 
// printImage = [](const Image& img, auto printStyler) {
//     // decltype returns the type of printStyler
//     using FUNC = decltype(printStyler);
//
//     for(int i=0; i<img.nRows; ++i) {
//         for(int j=0; j<img.nCols; ++j) {
//             printStyler(img(i, j));
//             if(j != img.nCols-1)
//                 cout << "|";
//         }
//         cout << endl;
//     }
// };

// template<typename T>
// struct TypeDisplayer;

// parameter pack
template<typename... Ts>
struct TypeDisplayer;

// TypeDisplayer<int, float, double> a;
// TypeDisplayer<int> b;

// this structs can be actually found in std.
namespace my {
    // CHECK IF TYPES ARE SAME
    // generic scenerio
    template<typename T1, typename T2>   struct is_same  { static const bool value = false; }; 
    // template specialization
    // this is called by compiler if it fits.
    // meaning that this is called automatically if
    // two typename is the same
    template<typename K> struct is_same<K, K> { static const bool value = true; };

    // REMOVING CONST
    // removing const or volitile from type 
    template<typename T> struct remove_const { using type = T; };
    // template specialization
    // if the type is const 
    // `special case`
    template<typename T> struct remove_const<const T> { using type = T; }; 

    // REMOVING VOLATILE
    template<typename T> struct remove_volatile { using type = T; };
    template<typename T> struct remove_volatile<volatile T> { using type = T; };

    template<typename T> struct remove_cv { 
        using type = typename remove_volatile<typename remove_const<T>::type>::type; 
    };

    // REMOVING REFERENCE
    template<typename T> struct remove_reference { using type = T; };
    template<typename T> struct remove_reference<T&> { using type = T; };
    template<typename T> struct remove_reference<T&&> { using type = T; };

    // DECAY
    // remove EVERYTHING and get actual type
    template<typename T> struct decay {
        // almost always first clear reference
        using type = typename remove_cv<typename remove_reference<T>::type>::type;
    };
} //end namespace


// TypeDisplayer<const int> v1;
// TypeDisplayer< my::remove_const<const int>::type > v2;
// helping compiler to figure out if its
// value or type.
// TypeDisplayer< typename my::remove_const<const int >::type> v3;

// TypeDisplayer< typename my::remove_cv<const volatile int >::type> v3;

auto 
print = [](const auto& mat, auto printStyler) {
    // missing 'typename' prior to dependent type name 'my::decay<decltype(mat)>::type
    using T = typename my::decay<decltype(mat)>::type;
    // if image
    // constexpr: bu constant bir expressiondur. Compile time'da evaluate et.
    if constexpr(my::is_same<T, Image>::value) {
        cout << "You are printing an image" << endl;
    } else if constexpr(my::is_same<T, Matrix<int>>::value) { 
        cout << "You are printing an Matrix<int>" << endl;
    } else {
        cout << "Printing unknown" << endl;
    }
    // TypeDisplayer<typename my::decay<T>::type> a;

    // decltype returns the type of printStyler
    using FUNC = decltype(printStyler);

    for(int i = 0; i < mat.nRows; ++i) {
        for(int j = 0; j < mat.nCols; ++j) {
            printStyler(mat(i, j));
            if(j != mat.nCols - 1)
                cout << "|";
        }
        cout << endl;
    }
};

template<typename T1, typename T2>
auto
print_old = [](const T1& mat, T2 printStyler) {
    using T = typename my::decay<T1>::type;
    // if image
    // constexpr: bu constant bir expressiondur. Compile time'da evaluate et.
    if constexpr(my::is_same<T1, Image>::value) {
        cout << "You are printing an image" << endl;
    } else if constexpr(my::is_same<T1, Matrix<int>>::value) { 
        cout << "You are printing an Matrix<int>" << endl;
    } else {
        cout << "Printing unknown" << endl;
    }
    // TypeDisplayer<typename my::decay<T>::type> a;

    // decltype returns the type of printStyler
    using FUNC = decltype(printStyler);

    for(int i = 0; i < mat.nRows; ++i) {
        for(int j = 0; j < mat.nCols; ++j) {
            printStyler(mat(i, j));
            if(j != mat.nCols - 1)
                cout << "|";
        }
        cout << endl;
    }
};


// BEFORE TEMPLATE AUTO
template <typename T, T value>
void func() {
    if constexpr(my::is_same<T, int>::value)
        cout << "This is int!" << endl;
    cout << value << endl;
}

// TEMPLATE AUTO
// used for passing parameters in compile time.
// first give me a type T, and add value of type T
template <auto value>
inline void func_improved() {
    using T = decltype(value);
    if constexpr(my::is_same<T, int>::value)
        cout << "This is int!" << endl;
    cout << value << endl;
}

int 
main(int argc, char* argv[]) {
    func<int, 5>();
    func_improved<5>();
    auto img = Image{10, 10};
    img(5, 5) = 100;

    // Hackish way to read type in compile time
    // error: implicit instantiation of undefined template 'TypeDisplayer<Image>'
    // TypeDisplayer<decltype(img)> a;

    // static const bool s = my::is_same<int, float>::value;
    // if (s) cout << "They are the same type" << endl;
    // else cout << "They are not the same type" << endl;

    // char separator = '_';

    // auto printStyler = [=](const Color& col) {
    //     cout << (int)col.r << separator << (int)col.g << separator << (int)col.b;
    // };
    // print(img, printStyler);

    print(img, [=](const Color& col) {
        cout << (int)col.r << "/" << (int)col.g << "/" << (int)col.b;
    });

    auto mat = Matrix<int>{4, 3};
    mat(2, 1) = 2;
    print(mat, [](int value) { cout << "[" << value << "]"; });

    auto matf = Matrix<float>{4, 3};
    matf(2, 1) = 2.0f;
    print(matf, [](float value) { cout << "[" << value << "]"; });


    return 0;
}
