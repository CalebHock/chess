#include "king.h"

#include "../board.h"

std::vector<Move> King::get_candidate_moves(const bool in_check_moves) const {
  std::vector<Move> candidate_moves;

  // Add single moves from king
  for (int rank_offset = -1; rank_offset <= 1; ++rank_offset) {
    for (int file_offset = -1; file_offset <= 1; ++file_offset) {
      if (rank_offset == 0 && file_offset == 0) {
        continue;
      }
      candidate_moves.push_back(Move(
          m_coordinate, Coordinate(m_coordinate.get_rank() + rank_offset,
                                   m_coordinate.get_file() + file_offset)));
    }
  }

  // Add castling moves
  if (!in_check_moves && !m_moved && !(m_board->in_check(m_color))) {
    for (int rook_file : {0, 7}) {
      Piece *rook =
          m_board->piece_at(Coordinate(m_coordinate.get_rank(), rook_file));
      if (rook != nullptr && rook->get_type() == Piece::Type::ROOK &&
          !(rook->get_moved())) {
        int direction = m_coordinate.get_file() < rook_file ? 1 : -1;

        // Check if castling is blocked by any piece.
        bool blocked = false;
        for (int file = m_coordinate.get_file() + direction;
             file != rook_file && !blocked; file += direction) {
          if (m_board->piece_at(Coordinate(m_coordinate.get_rank(), file)) !=
              nullptr) {
            blocked = true;
          }
        }
        if (blocked) {
          continue;
        }

        // Prevent castling through check.
        Board candidate_board(*m_board);
        candidate_board.piece_at(m_coordinate)
            ->move(Move(m_coordinate,
                        Coordinate(m_coordinate.get_rank(),
                                   m_coordinate.get_file() + direction)));
        if (candidate_board.in_check(m_color)) {
          continue;
        }
        candidate_moves.push_back(
            Move(m_coordinate,
                 Coordinate(m_coordinate.get_rank(),
                            m_coordinate.get_file() + direction * 2),
                 m_coordinate.get_file() < rook_file
                     ? Move::MoveType::KING_CASTLE
                     : Move::MoveType::QUEEN_CASTLE));
      }
    }
  }

  return candidate_moves;
}