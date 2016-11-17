#include <iostream>
#include <memory>

using namespace std;

class Base
{
public:
    Base() {}                                   /// Base class constructor
    virtual ~Base()                             /// Base class destructor
    {
        cout << "Destroyed Base" << endl;
    }

    virtual void foo() = 0;                     /// Pure virtual function

    float var = 6.0;
};

class A : public Base
{
public:
    A() {}                                      /// Base class constructor
    virtual ~A()                                /// Base class destructor
    {
        cout << "Destroyed A" << endl;
    }

    void foo()                                  /// Defining the pure virtual function declared in Base class
    {
        cout << "Hello Foo" << endl << endl;
    }

    float var = 5;
};

int main()
{
    shared_ptr<Base> base = nullptr;            ///Declare base class, and assigning nullptr value.
                                                ///pure virtual classes cannot be instanced.
    /*_____________________________________________________________________________________________*/

    shared_ptr<A> a = (shared_ptr<A>)new A();   ///Declare and initializing inherited class
    /*_____________________________________________________________________________________________*/

    base = a;                                   /// Assiging A to Base
                                                /// Note that because we are using smart pointers, no dynamic cast is required
                                                /// And we avoid object slicing
    /*_____________________________________________________________________________________________*/

    base->foo();                                /// foo() is called from Base object, overrided by A declaration
    /*_____________________________________________________________________________________________*/

    dynamic_pointer_cast<A> (base)->var;        /// Accesing the var Variable in A
    dynamic_pointer_cast<Base> (base)->var;     /// Accesing the var Variable in Base
    /*_____________________________________________________________________________________________*/

    return NULL;                                /// Exit
    /*_____________________________________________________________________________________________*/
}
