// enum_temp.cpp : template classes using enum values as template parameters
// Requires C11 or later
// BUILD: clang++ -std=c++11 map_var.cpp -o tmpl.out && tmpl.out
// BUILD: clang++ -std=c++14 map_var.cpp -o tmpl.out && tmpl.out

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

using namespace std;

/// https://stackoverflow.com/questions/1443793/iterate-keys-in-a-c-map/35262398#35262398
/// Below the more general templated solution to which Ian referred...

#include <map>

template<typename Key, typename Value>
using Map = std::map<Key, Value>;

template<typename Key, typename Value>
using MapIterator = typename Map<Key, Value>::iterator;

template<typename Key, typename Value>
class MapKeyIterator : public MapIterator<Key, Value> {

public:

    MapKeyIterator ( ) : MapIterator<Key, Value> ( ) { };
    MapKeyIterator ( MapIterator<Key, Value> it_ ) : MapIterator<Key, Value> ( it_ ) { };

    Key *operator -> ( ) { return ( Key * const ) &( MapIterator<Key, Value>::operator -> ( )->first ); }
    Key operator * ( ) { return MapIterator<Key, Value>::operator * ( ).first; }
};

template<typename Key, typename Value>
class MapValueIterator : public MapIterator<Key, Value> {

public:

    MapValueIterator ( ) : MapIterator<Key, Value> ( ) { };
    MapValueIterator ( MapIterator<Key, Value> it_ ) : MapIterator<Key, Value> ( it_ ) { };

    Value *operator -> ( ) { return ( Value * const ) &( MapIterator<Key, Value>::operator -> ( )->second ); }
    Value operator * ( ) { return MapIterator<Key, Value>::operator * ( ).second; }
};

/// https://stackoverflow.com/questions/1443793/iterate-keys-in-a-c-map
/// Lots of good answers here, below is an approach using a couple of them which lets you write this:
/// If that's what you always wanted, then here is the code for MapKeys():

template <class MapType>
class MapKeyIterator {
public:
    class iterator {
    public:
        iterator(typename MapType::iterator it) : it(it) {}
        iterator operator++() { return ++it; }
        bool operator!=(const iterator & other) { return it != other.it; }
        typename MapType::key_type operator*() const { return it->first; }  // Return key part of map
    private:
        typename MapType::iterator it;
    };
private:
    MapType& map;
public:
    MapKeyIterator(MapType& m) : map(m) {}
    iterator begin() { return iterator(map.begin()); }
    iterator end() { return iterator(map.end()); }
};
template <class MapType>
MapKeyIterator<MapType> MapKeys(MapType& m)
{
    return MapKeyIterator<MapType>(m);
}

void use_map_keys()
{
    std::map<std::string, int> m { {"jim", 1000}, {"sally", 2000} };
    for (auto key : MapKeys(m))
        std::cout << key << std::endl;
}

////----////----////----////----////----////----////----////----////----////----////----////----////
static const vector<string> default_map_keys {
    "zero",
    "one",
    "two",
    "three"
};

/// vector streaming
namespace std {
    template<typename T>
    string to_string(vector<T> vec) { return "std::vector(size=" + to_string(vec.size()) + ")"; }
}


static void testMapVecRef()
{
    map<string, vector<int>> svi;
    vector<int>& vec = svi[default_map_keys[1]];
    cout << "vec[0]: " << (vec.empty() ? "empty" : to_string(vec[0])) << endl;
    vec.push_back(0);
    vec.push_back(3);
    vec.push_back(2);
    vec.push_back(1);
    vector<int> cpy = svi[default_map_keys[1]];
    cout << "vec[0]: " << (vec.empty() ? "empty" : to_string(vec[0])) << endl;
    cout << "cpy[0]: " << (cpy.empty() ? "empty" : to_string(cpy[0])) << endl;
    cpy.push_back(4);
    cout << "vec[4]: " << (vec.size() <= 4 ? "short" : to_string(vec[4])) << endl;
    cout << "cpy[4]: " << (cpy.size() <= 4 ? "short" : to_string(cpy[4])) << endl;
    vec.push_back(14);
    vec.push_back(15);
    cpy = svi[default_map_keys[1]];
    cout << "vec[5]: " << (vec.size() <= 5 ? "short" : to_string(vec[5])) << endl;
    cout << "cpy[5]: " << (cpy.size() <= 5 ? "short" : to_string(cpy[5])) << endl;

    auto& nur = svi["haha"];
    cout << "nur ? " << (nur.empty() ? "empty" : "not empty") << endl;
    nur.push_back(117);
    cout << "nur ! " << nur[0] << endl;
}

template <typename TV>
class MapTraj
{
public:
    MapTraj( TV default_value, const std::string& name="UNK")
           : name_(name != "" ? name : "ANONYMOUS_" + std::to_string(time(NULL)) )
    {
        map_keys_ = default_map_keys;
        for (auto& key : map_keys_) {
            traj_map_[key] = default_value;
        }
    }
    virtual ~MapTraj() {}

    const std::vector<std::string> getKeys()
    {
        return map_keys_;
    }

    /**
     * Updates the value of an existing key and returns true;
     * else fails and returns false.
     */
    bool updateTraj(const std::string& key, TV val)
    {
        auto it = traj_map_.find(key);
        if (it == traj_map_.end()) {
            return false;
        }
        it->second = val;
        return true;
    }

    /**
     * Sets the value of an existing key and returns true;
     * else fails and returns false.
     */
    TV getTraj(const std::string& key, const TV& default_value)
    {
        auto it = traj_map_.find(key);
        if (it == traj_map_.end()) {
            return default_value;
        }
        return it->second;
    }


    void showSizes(int verbose=1)
    {
        if (verbose > 0) {
            cout << "Using keys array:" << endl;
            for (auto key : map_keys_) {
                std::cout << key << " :: " << traj_map_.at(key).size() << std::endl;
            }
            std::cout << std::endl;
        }
        if (verbose > 1) {
            std::cout << std::endl;
            cout << "Using map entry it:" << endl;
            for (auto entry : traj_map_) {
                std::cout << entry.first << " :: " << entry.second.size() << std::endl;
            }
            std::cout << std::endl;
        }
    }

    void showMap(const std::string& label="") {
        cout<< (label == "" ? name_ : label) << ": showMap..." << endl;
        for (auto& entry: traj_map_) {
            cout << "KEY: " << entry.first << " => " << entry.second << endl;
        }
    }

public:
    /**
     * Sets the value of a new or existing key and returns true;
     * else fails and returns false.
     */
    bool setTraj(const std::string& key, TV val)
    {
        traj_map_[key] = val;
        return true;
    }

private:
    std::vector<std::string>  map_keys_;
    std::map<std::string, TV> traj_map_;
    const std::string name_;
};

typedef std::vector<int> vint_t;

class MapTrajVec : public MapTraj<vint_t>
{
public:
    MapTrajVec() : MapTraj(std::vector<int>())
    {}

};

typedef struct Duf {
    int Int;
    std::string Str;
    Duf(int i, std::string str) : Int(i), Str(str) {}
    std::string to_string() {
        return "<Duf: " + std::to_string(Int) + " => " + Str + ">";
    }
} Duf;


namespace std {
    string to_string(Duf duf) { return duf.to_string(); }
}

void test_duf()
{
    Duf dA = Duf(1, "one");
    cout<< dA.to_string() << endl;
    Duf dB = Duf(2, "two");
    cout<< std::to_string(dB) << endl;
}

namespace zoid {
inline static int64_t getHash() { return 1; };
}
typedef struct ___lcm_hash_ptr __lcm_hash_ptr;
struct ___lcm_hash_ptr
{
    const __lcm_hash_ptr *parent;
    int64_t (*v)(void);
};

typedef struct ___reversed_ptr ___reversed_ptr;
struct ___reversed_ptr
{
    int64_t (*v)(void);
    const void *parent;
};

int main(int argc, char* argv[])    // NB: unit tests for MapTraj
{
    const __lcm_hash_ptr *p = 0;
    const __lcm_hash_ptr cp = { p, zoid::getHash };
    // void *z = 0;
    const ___reversed_ptr rp = { zoid::getHash, };  // p defaults to 0x0

    cout << "CP: " << cp.parent << endl;
    cout << "RP: " << rp.parent << endl;

    const std::vector< std::string > vecs { "one", "TWO", "tHrEe", "FoUr"};
    const std::vector< std::string > copy(vecs);
    const std::string sp = ", ";
    cout<< "The copy of vecs: " << copy[0] << sp << copy[1] << sp << copy[2] << sp << copy[3] << endl;

    test_duf();
    // return 0;

    MapTrajVec map_vec;
    map_vec.showSizes();
    vint_t vintA = {};
    std::vector<int> vintB = {1, 2, 3};
    cout<< "updateTraj('three', 4) returns: " << map_vec.updateTraj("three", vintA) << "; now show:" << endl;
    map_vec.showSizes();
    //map_vec.showMap();

    MapTraj<int> map_int(-1);
    map_int.showMap();
    cout<< "updateTraj('three', 4) returns: " << map_int.updateTraj("three", 4) << "; now show:" << endl;
    map_int.showMap();
    cout<< "setTraj('four', 4) returns: " << map_int.setTraj("four", 4) << "; now show:" << endl;
    map_int.showMap();
    cout<< "getTraj('forty') returns: " << map_int.getTraj("forty", -9999) << "; now show:" << endl;
    map_int.showMap();


    MapTraj<std::string> map_str("DEF_STR", "");
    map_str.showSizes();
    cout<< "updateTraj('three', 'FOUR') returns: " << map_str.updateTraj("three", "FOUR") << "; now show:" << endl;
    map_str.showSizes();
    cout<< "setTraj('four', 'FOUR') returns: " << map_str.setTraj("four", "FOUR") << "; now show:" << endl;
    map_str.showSizes();
    cout<< "getTraj('forty') returns: " << map_str.getTraj("forty", "NaN") << "; now show:" << endl;
    map_str.showSizes();
    map_str.showMap();

    cout<< "vector vintB: (" << std::to_string(vintB) << ")" << endl;

    testMapVecRef();

    return 0;
}
