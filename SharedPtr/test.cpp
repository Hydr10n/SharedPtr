#include "SharedPtr.h"
#include <iostream>
#include <memory>

using namespace std;
using namespace Hydr10n::Memory;

struct MyClass {
    MyClass() { cout << "MyClass() at " << this << " called" << endl; }

    ~MyClass() { cout << "~MyClass() at " << this << " called" << endl; }

    friend ostream &operator<<(ostream &os, const MyClass &src) { return os << &src; }
};

template <class T> struct MyDeleter {
    void operator()(T *ptr) { delete ptr; }
};

template <class T> struct MyDeleter<T[]> {
    void operator()(T *ptr) { delete[] ptr; }
};

int main() {
    {
        shared_ptr<MyClass> x(new MyClass);
        shared_ptr<MyClass[]> a(new MyClass[2]), b(a);
        b.swap(a);
        cout << "a[0] = " << a[0] << endl << "b[1] = " << b[1] << endl;
    }
    cout << endl;
    {
        SharedPtr<MyClass, MyDeleter<MyClass>> x(new MyClass);
        SharedPtr<MyClass[], MyDeleter<MyClass[]>> a(new MyClass[2]), b(a);
        b.Swap(a);
        cout << "a[0] = " << a[0] << endl << "b[1] = " << b[1] << endl;
    }
}