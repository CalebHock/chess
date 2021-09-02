#include "coordinate.h"

#include "util.h"

bool operator==(const Coordinate &c1, const Coordinate &c2) {
  return c1.get_rank() == c2.get_rank() && c1.get_file() == c2.get_file();
}

std::ostream &operator<<(std::ostream &os, const Coordinate &c) {
  os << coordinate_to_uci(c);
  return os;
}