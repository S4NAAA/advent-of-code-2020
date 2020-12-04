#include "dataloader.hpp"
#include <unordered_map>
#include <unordered_set>
#include <locale>

static bool is_number(const std::string &s) {
  return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

class Pass {
public:
  Pass(std::vector<std::string> data) : pass_info("") {
    for (const auto &line : data) {
      check_fields(line);
      pass_info.append("  " + line);
    }
  }

  bool is_valid(void) const { 
    int count = 0;
    std::cout << pass_info << '\n';

    for (const auto &field : fields) {
      std::cout << field.first << ' ' << field.second << '\n';
      count += field.second;
    }
    
    if ((count == 7 && !fields.find("cid:")->second) || count == 8) {
      std::cout << "passed\n";
      return true;
    } else {
      std::cout << "failed\n";
      return false;
    }
  }

private:
  std::string pass_info;

  std::unordered_map<std::string, bool> fields { 
      {"byr:", false}, 
      {"iyr:", false}, 
      {"eyr:", false}, 
      {"hgt:", false},
      {"hcl:", false}, 
      {"ecl:", false}, 
      {"pid:", false},
      {"cid:", false} 
  };

  std::unordered_set<std::string> valid_ecl {
    "amb", "blu", "brn", "gry", "grn", "hzl", "oth"
  };

  std::unordered_map<std::string, std::function<bool(std::string)>> field_checker { 
      {"byr:", [](const std::string &data) {
        if (data.size() != 4)
          return false;
        
        if (!is_number(data))
          return false;

        if (int v = std::stoi(data); v < 1920 || v > 2002)
          return false;

        return true;
      }}, 

      {"iyr:", [](const std::string &data) { 
        if (data.size() != 4)
          return false;
        
        if (!is_number(data))
          return false;

        if (int v = std::stoi(data); v < 2010 || v > 2020)
          return false;

        return true;
      }},

      {"eyr:", [](const std::string &data) { 
        if (data.size() != 4)
          return false;
        
        if (!is_number(data))
          return false;

        if (int v = std::stoi(data); v < 2020 || v > 2030)
          return false;

        return true;
      }},

      {"hgt:", [](const std::string &data) {
        if (!(data.size() == 5 || data.size() == 4))
          return false;

        const std::string unit(data.end() - 2, data.end());
          
        if (unit == "cm") {
          const std::string num(data.begin(), data.begin() + 3);
          
          if (!is_number(num))
            return false;

          int val = std::stoi(num);

          if (val < 150 || val > 193)
            return false;
        } else if (unit == "in") {
          const std::string num(data.begin(), data.begin() + 2);
          
          if (!is_number(num))
            return false;

          int val = std::stoi(num);
          if (val < 59 || val > 76)
            return false;
        }

        return true;
      }},

      {"hcl:", [](const std::string &data) {
        if (data[0] != '#' || data.size() != 7)
          return false;

        if (data.find_first_not_of("0123456789abcdef", 1) != std::string::npos)
          return false;

        return true;
      }},

      {"ecl:", [&](const std::string &data) {
          return valid_ecl.find(data) != valid_ecl.end();
      }},

      {"pid:", [](const std::string &data) {
        if (data.size() != 9)
          return false;

        if (!is_number(data))
          return false;

        return true;
      }},

      {"cid:", [](const std::string &data) { 
        return false;
      }}
  };

  void check_fields(const std::string &line) {
    
    if (auto f = fields.find(std::string(line.begin(), line.begin() + 4));
        f != fields.end()) {
        std::string buf;

        for (auto d_it = line.begin() + 4; *d_it != ' ' && d_it != line.end(); ++d_it) {
          buf.push_back(*d_it);
        }


        f->second = field_checker.find(f->first)->second(buf);
    }

    for (auto it = line.begin(); it != line.end(); ++it) {
      if (*it == ' ') {
        const std::string field(it + 1, it + 5);
        if (auto f = fields.find(field); f != fields.end()) {
          std::string buf;

          for (auto d_it = it + 5; *d_it != ' ' && d_it != line.end(); ++d_it) {
            buf.push_back(*d_it);
          }

          f->second = field_checker.find(field)->second(buf);
        }
      }
    }
  }
};


int main(void) {
  std::vector<std::string> data = load_data("input4.txt");

  std::vector<std::string> tmp({});

  int c = 0;

  for (const auto &line : data) {
    if (line.empty()) {
      c += Pass(tmp).is_valid();
      tmp = {};
      continue;
    }
    
    tmp.push_back(line);
  }
  
  c += Pass(tmp).is_valid();
  std::cout << c << '\n';

  return 0;
}
