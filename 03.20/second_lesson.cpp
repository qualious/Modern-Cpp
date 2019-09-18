// Split image class into Matrix and Matrix
// inheritance: abstract functions
// explicit constructors
// dynamic_cast
// static_cast, const_cast, reinterpet_cast
//
// Future lectures:
// curiously recurring template pattern (CRTP)
// template parameters binding with runtime parameter types
// template-template types, template auto, type constructors

// If you are using const_cast, there is something wrong.
// Const_cast is only used when you are using an old library
// that didn't care about const correctness. Only used when
// using legacy libraries.

// Reinterpret cast. Cast anything to anything. 

// Static cast. The first thing that comes to mind
// when you think about cast.

#include <iostream>
// coming from  C. For uint8_t 
#include <stdint.h>
// unique pointer, shared pointer
#include <memory>
// vector
#include <vector>

using namespace std;

struct MatrixCore {
    // virtual void load() { };
    // this must be implemented. 
    virtual void load() = 0;
};

template<typename T>
class Matrix : public MatrixCore {
    public:
        int nRows, nCols;
        T* mem;

        size_t numElements() const { return static_cast<size_t>(nRows * nCols); } 
        
        void printMemoryUsage() const {
            cout << "memory allocated: (" << nRows << ", " << nCols << ") = " << (numElements() * sizeof(T)) << " bytes" << endl;
        }

        // I can now change the class runtime
        void init(int nRows, int nCols) {
            clear();
            this->nRows = nRows;
            this->nCols = nCols;
            mem = new T[numElements()];
            printMemoryUsage();
        }

        Matrix(int nRows, int nCols) : nRows{nRows}, nCols{nCols}, mem{numElements() == 0 ? nullptr : new T[numElements()]} {
            cout << "Created" << endl;
            printMemoryUsage();
        }

        Matrix() : Matrix(0, 0) { }

        Matrix(const Matrix& other) : Matrix(other.nRows, other.nCols) {
           std::copy(other.mem, other.mem + other.numElements(), mem);
        }

        void operator=(const Matrix& other) {
            init(other.nRows, other.nCols);
            copy(other.mem, other.mem + other.numElements(), mem);
        }
       
        Matrix(Matrix&& other) : nRows{other.nRows}, nCols{other.nCols}, mem(other.mem) {
            other.mem = nullptr;
        }
       
        void operator=(Matrix&& other) {
            clear();
            nRows = other.nRows;
            nCols = other.nCols;
            mem = other.mem;
            other.mem = nullptr;
        }
       
        T get(int row, int col) const {
            return mem[row * nCols + col];
        }

        void set(int row, int col, T value) {
            mem[row * nCols + col] = value;
        }

        T& operator()(int row, int col) { 
            static T dummy;
            if (!mem) { 
                cout << "No memory!" << endl; 
                return dummy;
            }
            return mem[row * nCols + col];
        }

        void clear() {
            delete[] mem;
            mem = nullptr;
            cout << "Memory cleared" << endl;
        }
        
        ~Matrix() {
            clear();
            cout << "Destroyed" << endl;
        }

        // There is a virtual function. 
        // If vf in MatrixCore is changed,
        // the compiler will warn us
        //
        // There for almost always use override
        // when implementing virtual functions.
        void load() override {
            cout << "Matrix loaded!" << endl; 
        }
};

struct Color {
    uint8_t r, g, b;

    Color() : Color(0) { };

    // If we put explicit, you are telling compiler that
    // "Don't implicitly call cotr"
    /*explicit*/ Color(uint8_t illum) : r{illum}, g{illum}, b{illum} { };
    
    Color(uint8_t r, uint8_t g, uint8_t b) : r{r}, g{g}, b{b} { }; 
};

struct Image : public Matrix<Color> {
    using Matrix<Color>::Matrix;
    using Matrix<Color>::operator=;
    
    // If we haven't implemented this, 
    // we would've called the load func of Matrix.
    void load() override { 
        cout << "Image loaded!" << endl; 
    }
};

int
main() {
    Matrix<uint16_t> mat{10, 10};
    Image img1;
    Image img2;
    
    img1(5, 6) = Color{10, 20, 255};
    // grayscale
    // compiler automatically calls 
    // cotr of Color. This is called
    // implicit construction
    img1(5, 6) = 100;   // unless otherwise specified with `explicit` keyword
                        // this is equal to img1(5, 6) = Color(100);

    // We don't know the type of objects in runtime
    auto objects = vector<MatrixCore*>{ &img1, &mat, &img2 };
    
    for (auto obj : objects) {
        // if obj is an instance of Image, 
        // do nothing to obj and assign
        // obj to img_ptr. if obj is not 
        // an instance of obj, return nullptr
        auto img_ptr = dynamic_cast<Image*>(obj);
        if (img_ptr) obj->load();    
    }

    return 0;
}
