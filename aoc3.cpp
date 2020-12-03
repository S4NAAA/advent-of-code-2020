#include <iostream>
#include <vector>
#include "dataloader.hpp"


unsigned long long  count_trees(std::vector<std::string> &data, std::vector<std::size_t> off) {
  
  std::size_t l = data[0].size();
  std::size_t pos = 0, ctr = 0;
  
  for (std::size_t i = 0; i < data.size(); i += off[1]) {
    if (data[i][pos % l] == '#')
      ctr++;
    
    pos += off[0];
  }
  
  return ctr;
}



int main(void) {
  
  std::vector<std::string> data = load_data("input3.txt");
  
  std::cout << count_trees(data, {1, 1}) *
               count_trees(data, {3, 1}) * 
               count_trees(data, {5, 1}) * 
               count_trees(data, {7, 1}) * 
               count_trees(data, {1, 2}) << '\n';
}
