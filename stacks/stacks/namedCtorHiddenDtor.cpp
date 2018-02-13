
#include "stdafx.h"

#include <iostream>
using namespace std;

/**
 *  Two ways of ensuring class objects can be instantiated only in the heap,
 *  not on the stack.
 */

/**
 * Named Constructor idiom
 */
class NamedCtor
{
public:
    ~NamedCtor()                { cout << "in NamedCtor destructor" << endl;}
    void suicide()              { delete this; }
    static NamedCtor* create()  { return new NamedCtor; }
private:
    NamedCtor()                 { cout << "in NamedCtor constructor" << endl;}

};

static int testNamedCtor()
{
    NamedCtor *pNc = NamedCtor::create();
    if (pNc != NULL)
        pNc->suicide();
    //NamedCtor nc;             // illegal because constructor is inaccessible
    return 0;
}

/**
 * Hidden Destructor idiom
 */
class HiddenDtor
{
public:
    HiddenDtor()                { cout << "in HiddenDtor constructor" << endl;}
    void suicide()              { delete this;}
    //static HiddenDtor* create()  { return new HiddenDtor; }
private:
    ~HiddenDtor()               { cout << "in HiddenDtor destructor" << endl;}
};

static int testHiddenDtor()
{
    HiddenDtor *pHd = new HiddenDtor;
    // delete pHd;              // illegal because destructor is inaccessible
    pHd->suicide();

    //HiddenDtor hd;            // illegal because destructor is inaccessible
    return 0;
}


int testNamedCtorHiddenDtor()
{
    testNamedCtor();
    testHiddenDtor();
    return 0;
}