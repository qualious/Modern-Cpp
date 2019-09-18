// Vec2d, Vec3d, templates (typename, non-type param)
// CV qualifiers: const, volotile
// const, mutable
// inheritance
// dynamic polymorphism, v-table, __v_ptr attribute
// C++ clang padding alignment, struct alignment!!

#include <iostream>
#include <array>

#include <utility>
#include <tuple>

using namespace std;

int dot_product_2d(int x1, int x2, int y1, int y2)                      { return x1*x2 + y1*y2; }
int dot_product_3d(int x1, int x2, int y1, int y2, int z1, int z2)      { return x1*x2 + y1*y2 + z1*z2; }

void print_2d(int x, int y) { cout << "[" << x << ", " << y << "]" << endl; }

struct Vec2d { int x, y; };

int dot_product_2d(Vec2d a, Vec2d b) { return a.x * b.x + a.y * b.y; }

struct Vec3d { int x, y, z; };

// int dot_product_3d(Vec3d a, Vec3d b) { return a.x * b.x + a.y * b.y + a.z * a.z; }

// not copying anything. You are telling compiler to the fact that
// this read is only temporary. so temporary accessed reference read
// once and then disgarded.
int dot_product_3d(Vec3d a, Vec3d b) { return a.x * b.x + a.y * b.y + a.z * a.z; }


namespace Better {

    struct Vec2d {
        int x, y;
        int dot_product(const Vec2d b) const { return x * b.x + y * b.y; }
        // const is `this`
        void print() const { }
    };


    struct Vec3d {
        int x, y, z;
        int dot_product(const Vec3d b) { return x * b.x + y * b.y + z * b.z; }
        void print() const { }
    };

} // namespace Better

// typedef int extra_data_t;
using extra_data_t = int;

namespace EvenBetter {

    struct VecBase {
        // virtual means manual polymorphism is done automatically.
        // making function virtual makes it so that
        // we don't have to cast the void pointer coming
        // from an outside vector to a Vec<2> or Vec<3> first
        // before executing the print() function. It dynamically
        // selects the correct run function in runtime
        virtual void print() const { }
    };

    // template<typename T, int a, short b, ....>
    // struct Vac { };         ->  class template
    // void func(...) {...}    ->  function template
    // variable template.
    //      Pi might be a template. And C-TP is precision
    //      Pi<double> -> Create a pi with precision of double
    //      Pi<float> -> Create a pi with precision of float

    // compile-time parameter
    // template creates a class just like Vec2d, where
    // functions reside inside it. This increases
    // binary size.
    template<int nDims, typename T = int >
    // next variable is dependent on nDims
            // struct alignment
    struct __attribute__((packed, aligned(1))) Vec : public VecBase {
        array<T, nDims> values;

        // smarter cotr
              // parameter pack
        template<typename... Ts>
        Vec(Ts... values) : values{values...} {  }

        // dumb cotr
        Vec(array<T, nDims> values) : values{values} {  }

        T dot_product(const Vec<nDims, T> b) const {
            auto sum = T{};
            for (int i = 0; i < nDims; ++i) {
                sum += values[i] * b.values[i];
            }
            return sum;
        }

        void print() const {
           // values[0] = T{}:  Error since this->values.
           // and we promised not to modify this
           cout << "[";
           for (int i = 0; i < nDims; ++i) {
               if (i != 0) cout << ", ";
               cout << values[i];
           }
           cout << "]" << endl;
        }

    };
} // namespace EvenBetter


// std::pair
namespace PairV001{
    template<typename T1, typename T2>
    struct Pair {
        T1 first;
        T2 second;
    };
}

namespace PairV002{
    template<typename T1, typename T2>
    struct Pair : public std::tuple<T1, T2> {
        Pair(T1 val1, T2 val2) : tuple<T1, T2>{val1, val2} { }
    };
}

int
main(int argc, char* argv[]) {
    cout << dot_product_2d(10, 20, 5, 3) << endl;
    cout << dot_product_3d(10, 20, 5, 3, 1, 2) << endl;
    print_2d(10, 20);

    cout << dot_product_2d(Vec2d{10, 20}, Vec2d{5, 3}) << endl;
    cout << dot_product_3d(Vec3d{10, 20, 30}, Vec3d{5, 3, 1}) << endl;

    cout << Better::Vec2d{10, 20}.dot_product({5, 3}) << endl;
    cout << Better::Vec3d{10, 20, 1}.dot_product({5, 3, 2}) << endl;

    cout << EvenBetter::Vec<2>{10, 20}.dot_product({5, 3}) << endl;
    cout << EvenBetter::Vec<3>{10, 20, 1}.dot_product({5, 3, 2}) << endl;

    // EvenBetter::Vec<3, float>{10.5, 20.2, 1.1}.print();

    /*const*/ auto v = EvenBetter::Vec<3>{10, 20, 30};
    // if const auto v, we can't do below
    // v.values[0] = 20;


    auto p1 = PairV001::Pair<int, float>{10, 5.0f};
    auto p2 = std::pair<int, float>{10, 5.0f};
    p2.first = 20;

    auto p3 = PairV002::Pair<int, float>{10, 5.0f};
    auto pair_value_first = std::get<0>(p3);

    cout << "pair_value_first: " << pair_value_first << endl;

    auto t1 = tuple<int, int, float, double, char>{10, 20, 0.5f, 23.5, 'f'};
    auto first_value = std::get<0>(t1);     // int
    auto third_value = std::get<1>(t1);     // float

    auto fourth_value = std::get<double>(t1);     // if and only if we have a single typename from tuple

    cout << "first_value: " << first_value << endl;
    cout << "third_value: " << third_value << endl;
    cout << "fourth_value: " << fourth_value << endl;

    {
        // DYNAMIC POLYMORPHISM manual implementation
        auto v1 = EvenBetter::Vec<2>{10, 20};
        auto v2 = EvenBetter::Vec<3>{10, 20, 1};
        // auto vec_array = array<extra_data_t*, 2>{ (extra_data_t*)&v1, (extra_data_t*)&v2 };
        auto vec_array = array<void*, 2>{ &v1, &v2 };

        for (int i = 0; i < 2; ++i) {
            // vec_array.at(i)[0];
            // *vec_array.at(i);
            // *((int*)vec_array[i])
            auto nDims = ((int*)vec_array[i])[0];
            if (nDims == 2)
                ((EvenBetter::Vec<2>*)vec_array[i])->print(); // (*(Vec<2>*)vec_array[i]).print();
            else
                ((EvenBetter::Vec<3>*)vec_array[i])->print();
            // ...
        }
    }


    {
        auto v1 = EvenBetter::Vec<2>{10, 20};
        auto v2 = EvenBetter::Vec<3>{10, 20, 1};
        auto vec_array = array<EvenBetter::VecBase*, 2>{ &v1, &v2 };

        for (int i = 0; i < 2; ++i) {
            // VecBase virtual allowed this to happen
            vec_array[i]->print();
        }

        cout << "Size of V1: " << sizeof(v1) << endl;
        cout << "Size of V2: " << sizeof(v2) << endl;

    }

  return 0;
}
