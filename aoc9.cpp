#include "dataloader.hpp"

#include <set>
#include <numeric>

using ull = unsigned long long;


class SumCombos {
public:
  static constexpr std::size_t preamble_size = 25;

  SumCombos(const std::string &path) 
    : combos(preamble_size - 1, std::set<ull>{}), pos(0) {

    const std::vector<std::string> string_data = load_data(path);
    
    for (const auto &line : string_data)
      data.push_back(std::stol(line));
    
    for (std::size_t i = 0; i < preamble_size; ++i)
      for (std::size_t j = i + 1; j < preamble_size; ++j)
        combos[i].insert(data[i] + data[j]);
  } 

  bool exists(const std::size_t num) {  
    for (const auto &combo : combos) {
      if (combo.find(num) != combo.end())
        return true;
    }
    return false;
  }

  void print(void) {
    for (const auto &combo : combos) {
      for (const auto &num : combo)
        std::cout << num << ' ';
      std::cout << '\n';
    }
  }

  std::size_t find_non_compliant_number(void) {
    while (exists(data[pos + preamble_size])) // assumes there is a solution
      update_combos();
    return data[pos + preamble_size];
  }

  std::size_t find_encryption_weakness(const std::size_t key) {
    const auto it = data.begin();
    
    for (std::size_t i = 1; i < data.size(); ++i) {
      const std::size_t w = data.size() - i;
      for (std::size_t j = 0; j < i; ++j) {
        if (std::accumulate(it + j, it + j + w, 0) == key) {
          const std::set<ull> vals(it + j, it + j + w); // there are better ways
          return *vals.begin() + *vals.rbegin();
        }
      }
    }


    return 0;
  }

private:

  void update_combos(void) {
    const std::size_t end = pos + preamble_size;
    
    for (std::size_t i = pos; i < end; ++i)
      combos[i % combos.size()].insert(data[end] + data[i]);

    combos[(pos++) % combos.size()] = std::set<ull>({data[end] + data[end - 1]});
  }

  std::size_t pos;
  std::vector<std::set<ull>> combos;
  std::vector<ull> data;
};

int main(void) {
  auto a = SumCombos("input9.txt");
  const auto key = a.find_non_compliant_number();
  std::cout << key << '\n';
  std::cout << a.find_encryption_weakness(key) << '\n';
}
