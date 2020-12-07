#include "dataloader.hpp"
#include <unordered_map>
#include <unordered_set>
#include <locale>
#include <utility>

using ull = unsigned long long;

// I used to just ditch out the 1st solution after  I passed it
// and just edit the past code with the 2nd solution

// I'll keep them from now on even if the code is bad
// like this prime example

class Bags {
// Found out there where "no other bags" lines
// ngl suprised this worked because i didn't knew those
// where there. This means a lot of these were full
// of gibberish and im suprised i didn't got a 
// segfault
public:
  Bags(const std::vector<std::string> &data) {
    for (const auto &line : data) {
      auto it = line.cbegin();
      
      const std::string key = search_for_holder_bag_info(it);

      std::unordered_set<std::string> contains;

      while (true) {
        contains.insert(search_for_holding_bags_info(it));
        if (*it == '.')
          break;
        else {
          it += 2;
        }
      }
      bag_info.insert({key, contains});
    }
  }

  bool find_if_can_hold(const std::string &key, const std::string &search_key) {
    std::unordered_set<std::string> found_bags;

    traverse_holding(found_bags, key);

    if (key == search_key) // for when the search starts, bye bye 2 hrs
      return false;

    return found_bags.find(search_key) != found_bags.end();
  }

  unsigned long long find_count(const std::string &search_key) {
    unsigned long long c = 0;

    for (const auto &k : bag_info)
      c += static_cast<unsigned long long>(find_if_can_hold(k.first, search_key));

    return c;
  }
  

private:
  std::unordered_map<std::string, std::unordered_set<std::string>> bag_info;

  std::string search_for_holder_bag_info(std::string::const_iterator &it) {
    std::string key;

    while (!((it[0] == 'b') && 
             (it[1] == 'a') && 
             (it[2] == 'g') && 
             (it[3] == 's'))) {
      key.push_back(it[0]);
      it = std::next(it);
    }

    it = it + 8 + 5;
    
    key.pop_back();     
    return key;
  }
  
  std::string search_for_holding_bags_info(std::string::const_iterator &it) {
    
    std::string key;
    unsigned char next_p;
    
    if (*it == '1') {
      it += 2;
      while (!((it[0] == 'b') && 
               (it[1] == 'a') && 
               (it[2] == 'g'))){
        key.push_back(it[0]);
        it = std::next(it);
      }
      next_p = 3;
    } else {
      it += 2;
      while (!((it[0] == 'b') && 
               (it[1] == 'a') && 
               (it[2] == 'g') && 
               (it[3] == 's'))) {
        key.push_back(it[0]);
        it = std::next(it);
      }
      next_p = 4;
    }
    it = it + next_p;

    key.pop_back();     
    return key; 
  }
  
  void traverse_holding(std::unordered_set<std::string> &found_bags, 
                        const std::string &key) {
    if (found_bags.find(key) == found_bags.end()) {
      found_bags.insert(key);
      if (auto s = bag_info.find(key); s != bag_info.end()) {
        for (const auto &holding_key : s->second) {
          traverse_holding(found_bags, holding_key);
        }
      }
    }
  }
};

class Bags2 {
public:
  Bags2(const std::vector<std::string> &data) {
    for (const auto &line : data) {
      auto it = line.cbegin();
      
      const std::string key = search_for_holder_bag_info(it);

      std::unordered_map<std::string, ull> contains;

      while (true) {
        ull v = [&]() {
          if (*it != 'n')
            return static_cast<ull>(*it - '0');
          else
            return static_cast<ull>(0);
        }();
        contains.insert({search_for_holding_bags_info(it), v});
        if (*it == '.')
          break;
        else {
          it += 2;
        }
      }
      bag_info.insert({key, contains});
    }
  }
  
  ull get_capacity(const std::string &key) {
    if (const auto &b = bag_info.find(key); b != bag_info.end()) {
      ull sum = 0;
      for (const auto &holding : b->second) {
        const ull v = get_capacity(holding.first);
        sum += holding.second * v + holding.second;
      }

      return sum;
    }
    return 1;
  }

private:
  std::unordered_map<std::string, std::unordered_map<std::string, ull>> bag_info;

  std::string search_for_holder_bag_info(std::string::const_iterator &it) {
    std::string key;

    while (!((it[0] == 'b') && 
             (it[1] == 'a') && 
             (it[2] == 'g') && 
             (it[3] == 's'))) {
      key.push_back(it[0]);
      it = std::next(it);
    }

    it = it + 8 + 5;
    
    key.pop_back();     
    return key;
  }
  
  std::string search_for_holding_bags_info(std::string::const_iterator &it) {
    
    std::string key;
    unsigned char next_p;
    
    if (*it == '1') {
      it += 2;
      while (!((it[0] == 'b') && 
               (it[1] == 'a') && 
               (it[2] == 'g'))){
        key.push_back(it[0]);
        it = std::next(it);
      }
      next_p = 3;
    } else {
      it += 2;
      while (!((it[0] == 'b') && 
               (it[1] == 'a') && 
               (it[2] == 'g') && 
               (it[3] == 's'))) {
        key.push_back(it[0]);
        it = std::next(it);
      }
      next_p = 4;
    }
    it = it + next_p;

    key.pop_back();     
    return key; 
  }
};


int main(void) {

  std::vector<std::string> data = load_data("input7.txt");
  
  auto a = Bags(data).find_count("shiny gold");

  std::cout << a << '\n';

  auto b = Bags2(data).get_capacity("shiny gold");

  std::cout << b << '\n';
  
  return 0;
}
