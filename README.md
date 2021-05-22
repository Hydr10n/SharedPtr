# SharedPtr
```SharedPtr``` is a simple thread-unsafe smart pointer that retains shared ownership of an object through a pointer. Several ```SharedPtr``` objects may own the same object. The object is destroyed and its memory deallocated when either of the following happens:
* The last remaining ```SharedPtr``` owning the object is destroyed;
* The last remaining ```SharedPtr``` owning the object is assigned another pointer via operator= or Reset().
The object is destroyed using delete-expression or a custom deleter that is supplied to ```SharedPtr``` during construction.

A ```SharedPtr``` can share ownership of an object while storing a pointer to another object. This feature can be used to point to member objects while owning the object they belong to. The stored pointer is the one accessed by Get(), the dereference and the comparison operators. The managed pointer is the one passed to the deleter when use count reaches zero.

```CPP
// Defined in header "SharedPtrBase.h"
namespace Hydr10n {
    namespace Memory {
        template <class T>
        struct DefaultDeleter;

        template <class T>
        struct DefaultDeleter<T[]>;

        template <class T, class Deleter>
        class SharedPtrBase;
    }
}

// Defined in header "SharedPtr.h"
namespace Hydr10n {
    namespace Memory {
        template <class T, class Deleter = DefaultDeleter<T>>
        struct SharedPtr : SharedPtrBase<T, Deleter>;

        template <class T, class Deleter>
        struct SharedPtr<T[], Deleter> : SharedPtrBase<T, Deleter>;
    }
}
```

### Template Parameters
|Name|Description|
|-|-|
|```T```|Data type|
|```Deleter```|A custom deleter used to destroy the managed object|

## Members

### Public Methods
|Name|Description|
|-|-|
|```SharedPtr```|Constructs new SharedPtr|
|```~SharedPtr```|Destructs the owned object if no more ```SharedPtr``` objects link to it|
|```operator=```|Assigns the SharedPtr|
|```operator->```|Dereferences the stored pointer|
|```operator*```|Dereferences the stored pointer|
|```operator[]```|Provides indexed access to the stored array|
|```operator bool```|Checks if the stored pointer is not null|
|```Get```|Dereferences the stored pointer|
|```GetReferenceCount```|Returns the number of ```SharedPtr``` objects referring to the same managed object|
|```Swap```|Swaps the managed objects|
|```Reset```|Replaces the managed object|


## Example
```CPP
#include "SharedPtr.h"
#include <iostream>

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
    SharedPtr<MyClass, MyDeleter<MyClass>> x(new MyClass);
    SharedPtr<MyClass[], MyDeleter<MyClass[]>> a(new MyClass[2]), b(a);
    b.Swap(a);
    cout << "a[0] = " << a[0] << endl << "b[1] = " << b[1] << endl;
}
```