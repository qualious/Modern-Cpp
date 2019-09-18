// implement Image class:: ctor, dtor, copy-ctor, move-ctor, copy-assign,
// move-assign
//
// inline functions
// make it a unique instance: deleted copy-ctor, deleted copy-assign?
// smart pointers: unique_ptr, shared_ptr
//
// Split image class into Matrix and Image
// inheritance: abstract functions
//
// explicit constructors
//
// dynamic_cast
//
// trailer of future lectures:
// static_cast, const_cast, reinterpet_cast
// curiously recurring template pattern (CRTP)
// template parameters binding with runtime parameter types
// template-template types, template auto, type constructors


#include <iostream>
// coming from  C. For uint8_t
#include <stdint.h>
// unique pointer
#include <memory>

using namespace std;

class Image {
    public:
        // using T = unsigned char;
        // in any architecture this is 8 bits.
        // this makes the code architecturally agnostic
        // if you want to print as a int, you need to cast it
        using T = uint8_t; // mapped to unsigned char. Sending to cout prints a char
        int nRows, nCols;
        T* mem;

        // nullptr: keyword that specifies extra type.
        // Gives your intention to compiler.
        // nullptr_t is the type of nullptr
        // Image() : nRows(0), nCols(0), mem(nullptr) {
        //     cout << "Created" << endl;
        // }

        // I'm doing a read-only job.
        // VERY IMPORTANT! always think about const correctness

        // if we were to store the elements in class.
        // we would've done an over optimisation.
        // This is better because multiplication is 1 cycle.
        // memory access however can go up to 30 cycle.
        //
        // Because this is inline, it's now a macro. Directly
        // copies the func.
        //
        // This is already inline tho because we declared it
        // under a class
        //
        // size_t == unsigned long long
        // size_t is used when you want to represent size
        /*inline*/ /*int*/ size_t numElements() const { return nRows * nCols; }

        // auto numElemnts() const { return (size_t)(nRows * nCols); }
        // int numElements() const; // declared outside of class

        void printMemoryUsage() const {
            cout << "memory allocated: (" << nRows << ", " << nCols << ") = " << (numElements() * sizeof(T)) << " bytes" << endl;
        }


        // I can now change the class runtime
        void init(int nRows, int nCols) {
            clear();
            this->nRows = nRows;
            this->nCols = nCols;
            // T[...] expects unsigned long
            // but we previously provided int
            // by numElements().
            mem = new T[numElements()];
            printMemoryUsage();
        }

        // we are creating an array. we choose to make it one dimensional
        // because we will handle the two dimensionality ourselves
        // { } is more strict than ( )
        Image(int nRows, int nCols) : nRows{nRows}, nCols{nCols}, mem{nRows*nCols == 0 ? nullptr : new T[numElements()]} {
            cout << "Created" << endl;
            printMemoryUsage();
        }

        // Delegated cotr (dtor),
        Image() : Image(0, 0) { }

        // Copy cotr (ctor).
        // If you don't put ref it's copies the image. We will
        // access the other image read-only
        // I DIDN'T COPY THE CONTENTS OF `other`.
        Image(const Image& other) : Image(other.nRows, other.nCols) {
            // copying contents of `other`.
            // starts from other.mem, goes to last. the last
            // is exclusive. final argument is where to copy
            //
            // since I already got nRows and nCols I could've just called
            // numElements() in this context. But this is more readable.
            std::copy(other.mem, other.mem + other.numElements(), mem);
        }

        // Unique pointer: The thing I created must be unique.
        // no copy can be made. no assignment can be made.
        // only move cotr and move assign can be used.
        // This implies intent
        //
        // This can also be done with unique pointers.
        // Image(const Image& other) = delete;

        // Just like copy cotr
        void operator=(const Image& other) {
            // This fails if (other.nRows and nRows) or (other.nCols and nCols)
            // are different.
            // nRows = other.nRows;
            // nCols = other.nCols;
            // Therefore we need to init first.
            init(other.nRows, other.nCols);

            copy(other.mem, other.mem + other.numElements(), mem);
        }

        // Move cotr.
        // && differentiates move cotr from copy ctor
        // && == reference to temp thing.
        // mem(other.mem) copies the pointer. Memory stays the same
        Image(Image&& other) : nRows{other.nRows}, nCols{other.nCols}, mem(other.mem) {
            // we do this because when `other` is destructed it
            // does nothing and `this` has the memory
            other.mem = nullptr;
        }

        // Just like move cotr
        void operator=(Image&& other) {
            // clear because we need to initilized already.
            // we need to delocate.
            clear();
            nRows = other.nRows;
            nCols = other.nCols;
            mem = other.mem;
            other.mem = nullptr;
        }

        // old way of getting value from matrix
        T get(int row, int col) const {
            return mem[row * nCols + col];
        }

        // old way of setting value to matrix
        void set(int row, int col, T value) {
            // two dimentionality.
            // old way of doing matrix access
            mem[row * nCols + col] = value;
        }

        // do this instead.
        // this is not const because it works both ways.
        // this operator returns a reference to a image pixel.
        // if you use that reference to reading, thats const.
        // but you are also setting the value
        T& operator()(int row, int col) {
            static T dummy;
            // Better way: Throw an exception
            if (mem == nullptr) {
                cout << "No memory!" << endl;
                return dummy;
            }
            return mem[row * nCols + col];
        }

        void clear() {
            delete[] mem;
            // clear the internal memory.
            // If you clean a memory, assign nullptr to it.
            mem = nullptr;
            cout << "Memory cleared" << endl;
        }

        ~Image() {
            clear();
            cout << "Destroyed" << endl;

        }
};

// this is not inline
// func calling saves local things to stack before func call
// to remember them because you want to continue executing.
// this is why we use inline.
//
// when inline, the function literally gets copied and pasted
// by compiler
// int Image::numElements() const { return nRows * nCols }

int
main() {
    {
        // Image img{10, 20};       // allocate image of 10x20;
        auto img = Image{10, 20};   // almost always use auto

        // old
        // img.set(5, 6, 100);      // go to r(5) and c(6) and write 100 there.
        // new
        img(5, 6) = 100;

        // img.clear();             // clear mem
        // img.init(30, 10);        // re-initilize already created img

        Image img2{img};            // copy ctor.
        Image img3 = img;           // copy ctor
        // object of type 'Image' cannot be assigned because its copy assignment operator is implicitly deleted
        // Image img4;
        // img4 = img;              // this is plain assignment

        Image img4;
        img4 = img2;                // copy assign

        // This is equal to Image img3((Image&&)img)
        Image img5(std::move(img)); // move cotr

        Image img6;
        img6 = std::move(img4);     // move assign. img4 is crippled.

        // old
        // cout << (int)img.get(5, 6) << endl;
        // cout << (int)img2.get(5, 6) << endl;

        // new
        cout << (int)img(5, 6) << endl;
        cout << (int)img2(5, 6) << endl;
    }

    {
        // We created an image in heap memory.
        // and then we wrap it with unique pointer.
        // This overloads `->` operator and makes it
        // impossible to copy.
        //
        // This is bad tho.
        // unique_ptr<Image> unique_image(new Image(10, 20));
        // this is old.
        // auto unique_image = unique_ptr<Image>(new Image(10, 20));
        // this is new.
        auto unique_image = make_unique<Image>(10, 20);     // C++17
        // unique_image->nCols;
        (*unique_image)(5, 6) = 100;
    }

    {
        // Share pointer:
        // Normally, if you don't want to copy the
        // when passing obj as a argument to func, you
        // pass the reference instead of the obj itself.
        // Big object passing around in function
        //
        // Difference from normal ref: It tracks who accesses
        // it. In multithreading, if you pass a ref,
        // the time of deletion of ref is ambiguous.
        // At times like this, use a share pointer.
        //
        // When everyone done reading the shared ptr,
        // the ptr is destroyed.
        auto img = make_shared<Image>(100, 200);
        auto img2 = img;
        auto img3 = img;

        (*img)(5, 6) = 100;

        cout << (int)(*img)(5, 6) << endl;      // 100
        cout << (int)(*img2)(5, 6) << endl;     // 100
        cout << (int)(*img3)(5, 6) << endl;     // 100
    }

    return 0;
}
