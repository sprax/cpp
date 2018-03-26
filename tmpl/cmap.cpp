// enum_temp.cpp : template classes using enum values as template parameters
// Requires C11 or later
// BUILD: clang++ -std=c++11 cmap.cpp -o tmpl && tmpl
// BUILD: clang++ -std=c++14 cmap.cpp -o tmpl && tmpl

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
class MapIt {
public:
    static const unsigned num_keys = 3;
    static const std::string map_keys[num_keys];

    MapIt() {}
    virtual ~MapIt() {}

    const std::string* getKeys()
    {
        return map_keys;
    }

    /**
     * Sets the value of an existing key and returns true;
     * else fails and returns false.
     */
    bool setIt(const std::string& key, TV val)
    {
        cmap[key] = val;
        return true;
    }

    /**
     * Sets the value of an existing key and returns true;
     * else fails and returns false.
     */
    int getIt(const std::string& key)
    {
        return cmap[key];
    }


    void show() {
        cout << "Using keys array:" << endl;
        for (auto key : map_keys) {
            std::cout << key << " :: " << cmap.at(key) << std::endl;
        }
        std::cout << std::endl;
        cout << "Using map entry it:" << endl;
        for (auto entry : cmap) {
            std::cout << entry.first << " :: " << entry.second << std::endl;
        }
        std::cout << std::endl;
    }

private:
    std::map<std::string, TV> cmap = {
        {map_keys[1], 1},
        {map_keys[0], 0},
        {map_keys[2], 2},
    };

};

template<>
const std::string MapIt<int>::map_keys[MapIt::num_keys] = {
    "one",
    "two",
    "three"
};


int main(int argc, char* argv[])    // NB: This is more a unit test than an app; it does not play ghost!
{
    MapIt<int> mapit;
    mapit.show();
    return 0;
}
