// enum_temp.cpp : template classes using enum values as template parameters
// Requires C11 or later
// BUILD: clang++ -std=c++11 enum_11.cpp -o tmpl && tmpl
// BUILD: clang++ -std=c++14 enum_11.cpp -o tmpl && tmpl

#include <iostream>
#include <chrono>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::cin;
using std::cout;
using std::endl;

#define SequentialEnum(Name, ...)   \
enum Name { __VA_ARGS__ };          \
namespace                           \
{   const std::initializer_list<Name> Name##List { __VA_ARGS__ }; };

SequentialEnum(Shape,
    Circle,
    Square,
    Triangle,
    Oval,
    Polygon
);

class BaseShaper
{
public:
    virtual void show() {
        cout << "I'm a base shaper." << endl;
    }
};

template<Shape S>
class Shaper : public BaseShaper
{
public:
    virtual void show() {
        cout << "I'm a shaper of type: " << S << endl;
    }
};





template <class TKey, class TType>
class ShaperFactory
{
   // This class implements a generic factory that can be used to create any type with any number of arguments.
   //
   typedef TType* (*CreateObjFn)(std::vector<void*> &args); // Defines the CreateObjFn function pointer that points to the object creation function.
   typedef std::unordered_map<TKey, CreateObjFn> FactoryMap; // Hash table to map the key to the function used to create the object.
public:
   void Register(const TKey &keyName, CreateObjFn pCreateFn)
   {
      // Store the function pointer to create this object in the hash table.
      FactMap[keyName] = pCreateFn;
   }

   TType* CreateObj(const TKey &keyName, std::vector<void*> &args)
   {
      // This method looks for the name in the hash map.  If it is not found, then an exception is thrown.
      // If it is found, then it creates the specified object and returns a pointer to it.
      //
      typename FactoryMap::iterator It = FactMap.find(keyName);
      if (It != FactMap.end())
      {
         return It->second(args);
      }
      throw "GenericFactory::CreateObj: key was not found in hashtable.  Did you forget to register it?";
   }
private:
   FactoryMap FactMap;
};

void shapes()
{
    for (Shape shape : ShapeList)
    {
        cout << "Shape: " << shape << endl;
        BaseShaper *shaper = new Shaper<Oval>();
        shaper->show();
    }
}


typedef enum tagMotionType {
    FIRST_MotionType    = 0,
    SCOOP_TYPE_1 		= 0,
    SCOOP_TYPE_2 		= 1,
    NUM_MotionTypes 	= 2
} MotionType;


template<MotionType M>
class Mover
{
public:
    void showMover() {
        std::cout << "Mover of MotionType " << M << std::endl;
    }
};


template<MotionType M>
class SpecMover : public Mover<M>
{
public:
    void showMover() {
        cout << "Mover of MotionType " << M << endl;
    }
};


void listMotionTypes()
{
    #if __cplusplus <= 199711L
    static const int arr[] = {16,2,77,29};
    std::vector<int> int_vec (arr, arr + sizeof(arr) / sizeof(arr[0]) );
    static const MotionType mta[] = {SCOOP_TYPE_1, SCOOP_TYPE_2};
    std::vector<MotionType> template_arg_vec (mta, mta + sizeof(mta) / sizeof(mta[0]) );
    #else
    // NOTE: g++ -std=c++11   OR   clang++ -std=c++11
    std::vector <int> int_vec = {1, 2, 3, 4, 5};
    std::vector<MotionType> template_arg_vec = {SCOOP_TYPE_1, SCOOP_TYPE_2};
    #endif
    for (int j = FIRST_MotionType; j < NUM_MotionTypes; j++) {
        MotionType typer = template_arg_vec[j];
        printf("MotionType(%d)\n", typer);
        // const int tt = static_cast<int>(typer); // fails because value is not known at compile time
        // Mover<tt> mover;         // error: non-type template argument of type 'int' is not an integral constant expression
        Mover<SCOOP_TYPE_1> mover;
        mover.showMover();
    }
}


int main(int argc, char* argv[])    // NB: This is more a unit test than an app; it does not play ghost!
{
    const unsigned millis = 2222;
    listMotionTypes();
    shapes();

    return 0;
}
