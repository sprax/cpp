// slicingProblem.cpp

#include "stdafx.h"
#include "tests.h"

#include<iostream>

using namespace std;

class Parent
{
public:
  virtual void func_a(void);
};

class Child : public Parent
{
public:
  void func_a(void);

  Child() {};

// non-standard:
  Child(Parent p)
  { 
      
  }
};

void func_1(Parent p);
void func_2(Parent* p);
void func_3(Parent& p);

int test_slicingProblem()
{
  //Using the Parent class
  Parent p;
  cout<<"\nParent Class"<<endl;
  func_1(p);
  func_2(&p);
  func_3(p);

  //Using the Child class
  Child c;
  cout<<"\n\nChild Class"<<endl;
  func_1(c);
  func_2(&c);
  func_3(c);

  //Using the Parent class that is initialised to the Child class
  Parent p2 = c; //Note that in p2, additional info about c is lost
  cout<<"\n\nParent p2 Class"<<endl;
  func_1(p2);
  func_2(&p2);
  func_3(p2);

  //Not possible to initialise directly from base to derived class. 
  //Child c2 = p2; - ERROR: cannot convert from 'Parent' to 'Child'
  //The way round it would be to have a constructor or overload operator = 
  Child c2 = p2;
  cout<<"\n\nChild c2 Class"<<endl;
  func_1(c2);
  func_2(&c2);
  func_3(c2);

  printf("==== end of slicingProblem_test ====\n");
  return 0;
}

void Parent::func_a()
{
  cout<<"Parent::func_a()"<<endl;
}

void Child::func_a(void)
{
  cout<<"Child::func_a()"<<endl;
}

void func_1(Parent p) 
{ 
  cout<<"func_1 -> ";
  p.func_a(); 
} 

void func_2(Parent* p) 
{ 
  cout<<"func_2 -> ";
  p->func_a(); 
} 

void func_3(Parent& p) 
{ 
  cout<<"func_3 -> ";
  p.func_a(); 
} 

