#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

std::vector<std::string> load_data(std::string path) {

  std::ifstream file;
  std::vector<std::string> data;
  
  file.open(path);

  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      data.push_back(std::string(line));
    }
  }

  return data;
}
