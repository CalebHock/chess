#include "queen.h"

#include "board.h"

std::vector<Move> Queen::get_candidate_moves(const bool in_check_moves) const {
  std::vector<Move> candidate_moves;
  std::vector<Move> straight_moves(get_straight_moves());
  std::vector<Move> diagonal_moves(get_diagonal_moves());

  candidate_moves.insert(candidate_moves.end(), straight_moves.begin(),
                         straight_moves.end());
  candidate_moves.insert(candidate_moves.end(), diagonal_moves.begin(),
                         diagonal_moves.end());

  return candidate_moves;
}