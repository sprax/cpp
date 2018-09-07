// enum_temp.cpp : template classes using enum values as template parameters
// Requires C11 or later
// BUILD: clang++ -std=c++11 cmap.cpp -o tmpl.out && tmpl.out
// BUILD: clang++ -std=c++14 cmap.cpp -o tmpl.out && tmpl.out

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

template<typename TK, typename TV>
std::vector<TK> extract_keys(std::map<TK, TV> const& input_map) {
  std::vector<TK> retval;
  for (auto const& element : input_map) {
    retval.push_back(element.first);
  }
  return retval;
}

template <typename TV>
class MapTraj {
public:
    static const unsigned num_keys = 4;
    static const std::string map_keys[num_keys];

    MapTraj() {}
    virtual ~MapTraj() {}

    const std::string* getKeys()
    {
        return map_keys;
    }

    /**
     * Updates the value of an existing key and returns true;
     * else fails and returns false.
     */
    bool updateTraj(const std::string& key, TV val)
    {
        auto it = cmap.find(key);
        if (it == cmap.end()) {
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
        auto it = cmap.find(key);
        if (it == cmap.end()) {
            return default_value;
        }
        return it->second;
    }


    void show(int verbose=1)
    {
        if (verbose > 0) {
            cout << "Using keys array:" << endl;
            for (auto key : map_keys) {
                std::cout << key << " :: " << cmap.at(key) << std::endl;
            }
            std::cout << std::endl;
        }
        if (verbose > 1) {
            std::cout << std::endl;
            cout << "Using map entry it:" << endl;
            for (auto entry : cmap) {
                std::cout << entry.first << " :: " << entry.second << std::endl;
            }
            std::cout << std::endl;
        }
    }

    void showMap() {
        for (auto& entry: cmap) {
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
        cmap[key] = val;
        return true;
    }

public:

    std::map<std::string, TV> cmap = {
        {map_keys[1], 1},
        {map_keys[0], 0},
        {map_keys[3], 3},
        {map_keys[2], 2},
    };

};

template<>
const std::string MapTraj<int>::map_keys[MapTraj::num_keys] = {
    "zero",
    "one",
    "two",
    "three"
};


int main(int argc, char* argv[])    // NB: This is more a unit test than an app; it does not play ghost!
{
    MapTraj<int> mapit;
    mapit.show();
    cout<< "updateTraj('three', 4) returns: " << mapit.updateTraj("three", 4) << "; now show:" << endl;
    mapit.show();
    cout<< "setTraj('four', 4) returns: " << mapit.setTraj("four", 4) << "; now show:" << endl;
    mapit.show();
    cout<< "getTraj('forty') returns: " << mapit.getTraj("forty", -9999) << "; now show:" << endl;
    mapit.show();

    mapit.showMap();

    return 0;
}
