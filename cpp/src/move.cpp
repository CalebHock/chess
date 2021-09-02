#include "move.h"

#include "util.h"

bool operator==(const Move &m1, const Move &m2) {
  return m1.get_from() == m2.get_from() && m1.get_dest() == m2.get_dest();
}

std::ostream &operator<<(std::ostream &os, const Move &m) {
  os << move_to_uci(m);
  return os;
}