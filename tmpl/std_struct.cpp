// enum_temp.cpp : template classes using enum values as template parameters
// Requires C11 or later
// BUILD: clang++ -std=c++11 traj_map_.cpp -o tmpl && tmpl
// BUILD: clang++ -std=c++14 traj_map_.cpp -o tmpl && tmpl

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

static const std::vector<std::string> default_map_keys {
    "zero",
    "one",
    "two",
    "forty-two",
    "three",
    "four",
    "ninety"
};

template<class Container, typename T>
int copy_into(Container &container, std::vector<T> vec)
{
    for (auto& elt : vec) {
        container.push_back(elt);
    }
    return container.size();
}

template <typename TV>
class MapTraj {
public:

    MapTraj(TV default_value)
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


    void show(int verbose=1)
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

    void showMap() {
        cout<< "showMap..." << endl;
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

public:
    std::vector<std::string>  map_keys_;
    std::map<std::string, TV> traj_map_;

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


namespace std{
    string to_string(Duf duf) { return duf.to_string(); }
}

void test_duf()
{
    Duf dA = Duf(1, "one");
    cout<< dA.to_string() << endl;
    Duf dB = Duf(2, "two");
    cout<< std::to_string(dB) << endl;
}


int main(int argc, char* argv[])    // NB: unit tests for MapTraj
{
    int j = 2;
    for (auto& key : default_map_keys) {
        cout << "key: " << key << endl;
        if (--j == 0) {
            break;
        }
    }

    std::list<std::string> name_list;
    int count = copy_into(name_list, default_map_keys);
    cout << "copy_into copied " << count << " keys from default_map_keys into name_list:" << endl;
    for (auto& name : name_list) {
        cout << name << endl;
    }

    //std::sort(name_list.begin(), name_list.begin());
    name_list.sort();
    cout << "After sorting, name_list:" << endl;
    for (auto& name : name_list) {
        cout << name << endl;
    }

    std::vector<std::string> myvector(default_map_keys);
    std::sort( myvector.begin(), myvector.end()
             , [](std::string& aa, std::string& bb) -> bool{ return aa.length() > bb.length(); });
    cout << "sorted vector copy:" << endl;
    for (auto& str : myvector) {
        cout << str << endl;
    }


    return 0;



    test_duf();
    MapTrajVec map_vec;
    map_vec.show();
    vint_t vintA = {};
    std::vector<int> vintB = {1, 2, 3};
    cout<< "updateTraj('three', 4) returns: " << map_vec.updateTraj("three", vintA) << "; now show:" << endl;
    map_vec.show();

    const std::vector< std::string > vecs { "one", "TWO", "tHrEe", "FoUr"};
    const std::vector< std::string > copy(vecs);
    const std::string sp = ", ";
    cout<< "The copy of vecs: " << copy[0] << sp << copy[1] << sp << copy[2] << sp << copy[3] << endl;


    // cout<< "setTraj('four', 4) returns: " << map_vec.setTraj("four", 4) << "; now show:" << endl;
    // map_vec.show();
    // cout<< "getTraj('forty') returns: " << map_vec.getTraj("forty", -9999) << "; now show:" << endl;
    // map_vec.show();
    // map_vec.showMap();

    // MapTraj<int> map_int(-1);
    // map_int.show();
    // cout<< "updateTraj('three', 4) returns: " << map_int.updateTraj("three", 4) << "; now show:" << endl;
    // map_int.show();
    // cout<< "setTraj('four', 4) returns: " << map_int.setTraj("four", 4) << "; now show:" << endl;
    // map_int.show();
    // cout<< "getTraj('forty') returns: " << map_int.getTraj("forty", -9999) << "; now show:" << endl;
    // map_int.show();
    // map_int.showMap();
    //
    //
    // MapTraj<std::string> map_str("DEF_STR");
    // map_str.show();
    // cout<< "updateTraj('three', 'FOUR') returns: " << map_str.updateTraj("three", "FOUR") << "; now show:" << endl;
    // map_str.show();
    // cout<< "setTraj('four', 'FOUR') returns: " << map_str.setTraj("four", "FOUR") << "; now show:" << endl;
    // map_str.show();
    // cout<< "getTraj('forty') returns: " << map_str.getTraj("forty", "NaN") << "; now show:" << endl;
    // map_str.show();
    // map_str.showMap();


    return 0;
}
