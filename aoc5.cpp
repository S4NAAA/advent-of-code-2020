#include "dataloader.hpp"
#include <algorithm>
#include <cassert>
#include <set>

class BoardingPass {
public:
  BoardingPass(const std::string &data) {
    std::size_t lower = 127, upper = 0;


    const auto update_row_boundaries = [&](const char &letter) -> void {
      if (letter == 'F' ) {
        lower = upper + (lower - upper) / 2;
      } else if (letter == 'B') {
        upper = upper + (lower - upper) / 2 + 1;
      }
    };

    const auto update_col_boundaries = [&](const char &letter) -> void {
      if (letter == 'R' ) {
        lower  = lower + (upper - lower) / 2 + 1;
      } else if (letter == 'L') {
        upper = lower + (upper - lower) / 2;
      }
    };

    std::for_each(data.begin(), data.begin() + 7, update_row_boundaries);

    assert(upper == lower);

    ID = lower * 8;
  
    upper = 7;
    lower = 0;

    std::for_each(data.begin() + 7, data.end(), update_col_boundaries);

    assert(upper == lower);

    ID += lower;
  }

  const std::size_t &get_ID(void) const { return ID; }
  bool operator< (const BoardingPass &rhs) const { return ID < rhs.get_ID(); }

  friend std::ostream& operator<< ( std::ostream& os, const BoardingPass& rhs );
private:
  std::size_t ID;
};

std::ostream& operator<< (std::ostream& os, const BoardingPass& rhs) {
  os << rhs.get_ID();
  return os;
}

int main(void) {
 
  std::vector<std::string> data = load_data("input5.txt");

  std::set<BoardingPass> boarding_passes;

  for (const auto &line : data)
    boarding_passes.insert(BoardingPass(line));

  std::cout << *boarding_passes.rbegin() << '\n';
 
  const auto exists = [&](std::size_t i) {
    return boarding_passes.find(*reinterpret_cast<BoardingPass*>(&i)) 
            != boarding_passes.end(); 
  };
  
  for (std::size_t i = 32; i < boarding_passes.rbegin()->get_ID(); ++i) {
    if (!exists(i))
      std::cout << i << '\n';
  }

  return 0;
}
