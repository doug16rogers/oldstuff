#include <stdiostr.h>


class A            { public: virtual void printOn() { puts("A"); } };
class B : public A { public: virtual void printOn() { } };
class C : public B { public: virtual void printOn() { puts("C"); } };

void Show(A* a) { a->printOn(); }

void main()
{
   A* a = new A;
   B* b = new B;
   C* c = new C;
   Show(a);
   Show(b);
   Show(c);
}
