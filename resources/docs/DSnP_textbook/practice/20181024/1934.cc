#include <iostream>

using namespace std;

class foo{
  public:
    virtual void f() { cout << "base f" << endl; }
    void g() { cout << "base g" << endl; }
    virtual void h() { cout << "base h" << endl; }
    virtual ~foo() { cout << "base dtor" << endl; }
};

class bar : public foo {
  public:
    void f() { cout << "derived f" << endl;}
    void g() { cout << "derived g" << endl;}
    void h() { cout << "derived h" << endl;}
    // if no implementation of "void h()" here, compile error if we want to create an instance of derived class bar.
    // 1913.o: In function `bar::bar()':
    // 1913.cc:(.text._ZN3barC2Ev[_ZN3barC5Ev]+0x1b): undefined reference to `vtable for bar'
    // collect2: error: ld returned 1 exit statu
    ~bar() { cout << "derived dtor" << endl; }
};


int main(){

  foo* ptr = new bar;
  ptr -> f(); ptr -> g(); ptr -> h(); 
  // class foo have no member named 'i';
  delete ptr;

  // ptr = new bar;
  // ptr -> f(); ptr -> g(); ptr -> h();
  // delete ptr;

  return 0;
}
