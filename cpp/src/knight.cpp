#include "knight.h"

#include "board.h"

std::vector<Move> Knight::get_candidate_moves(const bool in_check_moves) const {
  std::vector<Move> candidate_moves;

  // Possible relative moves for a knight (counterclockwise)
  int rank_offsets[8] = {2, 1, -1, -2, -2, -1, 1, 2};
  int file_offsets[8] = {1, 2, 2, 1, -1, -2, -2, -1};
  for (int i = 0; i < 8; ++i) {
    candidate_moves.push_back(Move(
        m_coordinate, Coordinate(m_coordinate.get_rank() + rank_offsets[i],
                                 m_coordinate.get_file() + file_offsets[i])));
  }

  return candidate_moves;
}