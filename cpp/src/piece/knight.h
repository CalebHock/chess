#ifndef KNIGHT_H
#define KNIGHT_H

#include "../piece.h"

/**
 * Knight piece class. Inherits utility from piece parent class.
 */
class Knight : public Piece {
public:
  /**
   * Constructor for knight piece object.
   *
   * @param board Pointer to board object which the knight resides on.
   * @param color Color of knight.
   * @param coordinate Location of knight on board.
   * @param moved Boolean if knight has moved on board.
   */
  Knight(Board *board, Color color, Coordinate coordinate, bool moved = false)
      : Piece(board, color, coordinate, moved) {}

  /**
   * Getter for knight type.
   *
   * @return Type::KNIGHT
   */
  Type get_type() const override { return Type::KNIGHT; }

  /**
   * Obtains a list of all possible moves of a knight.
   *
   * For knight, will add all L-moves from knight.
   *
   * @param in_check_moves Boolean to test only capturing moves if in check.
   * @return Vector of possible moves of knight.
   */
  std::vector<Move>
  get_candidate_moves(const bool in_check_moves = false) const override;
};

#endif