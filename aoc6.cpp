#include "dataloader.hpp"
#include <unordered_map>
#include <unordered_set>

// super inefficient solution for 6th day

class Group {
public:
  Group(const std::vector<std::string> &data) {


    for (const auto &question : data[0])
      questions.insert(question);

    for (std::size_t i = 1; i < data.size(); ++i) {
      
      //shouldn't need this many structures, pretty sure there is a better way
  
      std::unordered_set<char> answered;
      std::vector<char> to_erase;

      for (const auto &question : data[i])
        answered.insert(question);

      for (const auto &question : questions)
        if (answered.find(question) == answered.end())
          to_erase.push_back(question);


      for (const auto &question : to_erase)
        questions.erase(question);
    }
  }

  inline const std::size_t get_count(void) const { return questions.size(); }

private:

  std::unordered_set<char> questions;
};


int main(void) {

  std::vector<std::string> data = load_data("input6.txt");

  std::vector<std::string> tmp({});

  int c = 0;

  for (const auto &line : data) {
    if (line.empty()) {
      c += Group(tmp).get_count();
      tmp = {};
      continue;
    }
    
    tmp.push_back(line);
  }

  c += Group(tmp).get_count();

  std::cout << c << '\n';

  return 0;
}
