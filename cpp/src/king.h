#ifndef KING_H
#define KING_H

#include "piece.h"

/**
 * King piece class. Inherits utility from piece parent class.
 */
class King : public Piece {
public:
  /**
   * Constructor for king piece object.
   *
   * @param board Pointer to board object which the king resides on.
   * @param color Color of king.
   * @param coordinate Location of king on board.
   * @param moved Boolean if king has moved on board.
   */
  King(Board *board, Color color, Coordinate coordinate, bool moved = false)
      : Piece(board, color, coordinate, moved) {}

  /**
   * Getter for king type.
   *
   * @return Type::KING
   */
  Type get_type() const override { return Type::KING; }

  /**
   * Obtains a list of all possible moves of a king.
   *
   * For king, will add all one square and castling moves from king.
   *
   * @param in_check_moves Boolean to test only capturing moves if in check.
   * @return Vector of possible moves of king.
   */
  std::vector<Move>
  get_candidate_moves(const bool in_check_moves = false) const override;
};

#endif