#ifndef ROOK_H
#define ROOK_H

#include "piece.h"

/**
 * Rook piece class. Inherits utility from piece parent class.
 */
class Rook : public Piece {
public:
  /**
   * Constructor for rook piece object.
   *
   * @param board Pointer to board object which the rook resides on.
   * @param color Color of rook.
   * @param coordinate Location of rook on board.
   * @param moved Boolean if rook has moved on board.
   */
  Rook(Board *board, Color color, Coordinate coordinate, bool moved = false)
      : Piece(board, color, coordinate, moved) {}

  /**
   * Getter for rook type.
   *
   * @return Type::ROOK
   */
  Type get_type() const override { return Type::ROOK; }

  /**
   * Obtains a list of all possible moves of a rook.
   *
   * For rook, will add all straight moves from rook.
   *
   * @param in_check_moves Boolean to test only capturing moves if in check.
   * @return Vector of possible moves of rook.
   */
  std::vector<Move>
  get_candidate_moves(const bool in_check_moves = false) const override {
    return get_straight_moves();
  }
};

#endif