#include "pawn.h"

#include "board.h"

std::vector<Move> Pawn::get_candidate_moves(const bool in_check_moves) const {
  std::vector<Move> candidate_moves;
  int direction = (m_color == Piece::Color::WHITE) ? 1 : -1;

  // Add single move, if single move ends up on other side of board, add
  // promotion.
  Coordinate destination = Coordinate(m_coordinate.get_rank() + (1 * direction),
                                      m_coordinate.get_file());
  if (!m_board->piece_at(destination) && m_board->contains(destination)) {
    if ((destination.get_rank() == m_board->SIZE - 1 &&
         m_color == Piece::Color::WHITE) ||
        (destination.get_rank() == 0 && m_color == Piece::Color::BLACK)) {
      candidate_moves.push_back(
          Move(m_coordinate, destination, Move::MoveType::KNIGHT_PROMOTION));
      candidate_moves.push_back(
          Move(m_coordinate, destination, Move::MoveType::BISHOP_PROMOTION));
      candidate_moves.push_back(
          Move(m_coordinate, destination, Move::MoveType::ROOK_PROMOTION));
      candidate_moves.push_back(
          Move(m_coordinate, destination, Move::MoveType::QUEEN_PROMOTION));
    } else {
      candidate_moves.push_back(Move(m_coordinate, destination));
    }

    // Add double move from starting position.
    destination = Coordinate(m_coordinate.get_rank() + (2 * direction),
                             m_coordinate.get_file());
    if (!m_board->piece_at(destination) && m_board->contains(destination) &&
        !m_moved) {
      candidate_moves.push_back(
          Move(m_coordinate, destination, Move::MoveType::DOUBLE));
    }
  }

  // Add capturing moves (regular and em passant).
  for (int offset : {-1, 1}) {
    destination = Coordinate(m_coordinate.get_rank() + (1 * direction),
                             m_coordinate.get_file() + offset);

    if (m_board->contains(destination) &&
        !(m_board->m_empassant_target == Coordinate(-1, -1)) &&
        destination == m_board->m_empassant_target) {
      candidate_moves.push_back(
          Move(m_coordinate, destination, Move::MoveType::EM_PASSANT));
    } else if (m_board->contains(destination) &&
               m_board->piece_at(destination)) {
      if ((destination.get_rank() == m_board->SIZE - 1 &&
           m_color == Piece::Color::WHITE) ||
          (destination.get_rank() == 0 && m_color == Piece::Color::BLACK)) {
        candidate_moves.push_back(
            Move(m_coordinate, destination, Move::MoveType::KNIGHT_PROMOTION));
        candidate_moves.push_back(
            Move(m_coordinate, destination, Move::MoveType::BISHOP_PROMOTION));
        candidate_moves.push_back(
            Move(m_coordinate, destination, Move::MoveType::ROOK_PROMOTION));
        candidate_moves.push_back(
            Move(m_coordinate, destination, Move::MoveType::QUEEN_PROMOTION));
      } else {
        candidate_moves.push_back(Move(m_coordinate, destination));
      }
    }
  }

  return candidate_moves;
}