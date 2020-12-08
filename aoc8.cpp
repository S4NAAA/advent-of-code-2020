
#include "dataloader.hpp"
#include <unordered_map>

using ll = long long;

struct InstructionData {
  std::string command;
  ll val;
  bool ran;
};

class Interpreter {
public:
  Interpreter(const std::string &path) : pos(0) {
    const auto data = load_data(path);

    const auto get_instruction = [](const std::string &line) {
      return std::string(line.begin(), line.begin() + 3);
    };

    const auto get_value = [](const std::string &line) -> ll {
      return static_cast<ll>(std::stoi(std::string(line.begin() + 4, line.end())));
    };

    for (const auto &line : data)
      instruction_info.push_back({get_instruction(line), get_value(line), false});
  }
  
  void read_line(void) {
    auto &instruction = instruction_info[pos];
    
    if (instruction.ran || 
       (pos >= instruction_info.size() && 
        !(instruction.command == "jmp" &&
          instruction.val < 0)))
      terminated = true;
    else {
      instruction.ran = true;
      instructions.find(instruction.command)->second(instruction.val);
    }
  }

  void run(void) {
    pos = 0;
    terminated = false;
    val = 0;

    for (auto &instruction : instruction_info)
      instruction.ran = false;

    while (!terminated)
      read_line();
  }

  inline const ll &get_val(void) const { return val; }

  inline std::size_t get_size(void) const { return instruction_info.size(); }

  inline const std::size_t &get_pos(void) const { return pos; }

  inline InstructionData &get_instruction(const std::size_t i) { 
    return instruction_info[i];
  }


private:
  std::vector<InstructionData> instruction_info;

  std::unordered_map<std::string, std::function<void(const ll)>> instructions = {
    {"acc", [&](const ll num) {
      val += num;
      ++pos;
    }},
    {"jmp", [&](const ll num) {
      pos = static_cast<std::size_t>(pos + num);
    }},
    {"nop", [&](const ll num) {
      ++pos;
    }}
  };

  std::size_t pos;
  ll val;
  bool terminated;
};



int main(void) {

  auto a = Interpreter("input8.txt");

  for (std::size_t i = 0; i < a.get_size(); ++i) {
    auto &instruction = a.get_instruction(i);

    if (instruction.command == "nop") {
      instruction.command = "jmp";
      a.run();
      if (a.get_size() == a.get_pos()) {
        std::cout << a.get_val() << '\n';
        return 0;
      }
      instruction.command = "nop";
    } else if (instruction.command == "jmp") {
      instruction.command = "nop";
      a.run();
      if (a.get_size() == a.get_pos()) {
        std::cout << a.get_val() << '\n';
        return 0;
      }
      instruction.command = "jmp";
    }
  }

  return 0;
}

