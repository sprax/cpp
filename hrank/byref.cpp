#include<iostream>
#include<unistd.h> // for sleeping

struct Foo {
  Foo() { }
  Foo(const Foo&) {
    sleep(10); // assume that building from a copy takes TEN seconds!
  }
  void describe() const {
    std::cout<<"Foo at address "<<this<<std::endl;
  }
};

void byvalue(Foo foo) {
  std::cout<<"called byvalue"<<std::endl;
  foo.describe();
}

void byreference(Foo& foo) {
  std::cout<<"called byreference"<<std::endl;
  foo.describe();
}

int main() {
  Foo foo;
  std::cout<<"Original Foo"<<std::endl;
  foo.describe();
  byreference(foo);
  byvalue(foo);
}
