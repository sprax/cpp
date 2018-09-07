// enum_temp.cpp : template classes using enum values as template parameters
// Requires C11 or later
// BUILD: clang++ -std=c++11 enum_11.cpp -o tmpl.out && tmpl.out
// BUILD: clang++ -std=c++14 enum_11.cpp -o tmpl.out && tmpl.out

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
// FIXME: duplication is not DRY
const std::string ShapeNames[]{ "Circle", "Square", "Triangle", "Oval", "Polygon"};

class BaseUser
{
public:
    virtual void show() {
        cout << "I'm a base shaper." << endl;
    }
    virtual
};

template<Shape S>
class ShapeUser : public BaseUser
{
public:
    virtual void show() {
        cout << "I'm a shape-user of type: " << S << endl;
    }
};

class IUserMaker
{
public:
    virtual BaseUser * Make() const = 0;
    virtual ~IUserMaker() {}
};


template <class Key, class Type>
class UserFactory
{
   // This class implements a generic factory that can be used to create any type with any number of arguments.
   //
   typedef Type* (*TypeMaker)(std::vector<void*> &argvec);  // Defines the TypeMaker function pointer that points to the object creation function.
   typedef std::unordered_map<Key, TypeMaker> MakerMap;   // Hash table to map the key to the function used to create the object.

public:
   void Register(const Key &key, TypeMaker maker)
   {
      // Store the maker, that is, map the key to a pointer to a function that can make an object of type Type.
      maker_map_[key] = maker;
   }

   Type* Make(const Key &key, std::vector<void*> &argvec)
   {
      // This method looks for the name in the hash map.  If it is not found, then an exception is thrown.
      // If it is found, then it creates the specified object and returns a pointer to it.
      //
      typename MakerMap::iterator it = maker_map_.find(key);
      if (it != maker_map_.end())
      {
         return it->second(argvec);
      }
      throw "UserFactory::Make: key was not found in hashtable.  Did you forget to register it?";
   }

   int Size() {
       return maker_map_.size();
   }

private:
   MakerMap maker_map_;
};

template<typename K, typename T>
int register_shapes(UserFactory<K,T> &factory)
{
    std::vector<void*> argvec;      // dummy args
    for (Shape shape : ShapeList)
    {
        cout << "Regsistering shape: " << shape << endl;
        factory.Register(shape, thing_maker<shape>);
        auto shaper = factory.Make(shape, argvec);
        shaper->show();
    }
    return factory.Size();
}

void list_shapes()
{
    for (Shape shape : ShapeList)
    {
        cout << "Shape: " << shape << endl;
        BaseUser *shaper = new User<Oval>();
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
    // listMotionTypes();
    UserFactory<Shape, User> factory;
    int num_reg = register_shapes(factory);
    list_shapes();

    return 0;
}
