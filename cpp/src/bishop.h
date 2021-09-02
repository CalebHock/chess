#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"

/**
 * Bishop piece class. Inherits utility from piece parent class.
 */
class Bishop : public Piece {
public:
  /**
   * Constructor for bishop piece object.
   *
   * @param board Pointer to board object which the bishop resides on.
   * @param color Color of bishop.
   * @param coordinate Location of bishop on board.
   * @param moved Boolean if bishop has moved on board.
   */
  Bishop(Board *board, Color color, Coordinate coordinate, bool moved = false)
      : Piece(board, color, coordinate, moved) {}

  /**
   * Getter for bishop type.
   *
   * @return Type::BISHOP
   */
  Type get_type() const override { return Type::BISHOP; }

  /**
   * Obtains a list of all possible moves of a bishop.
   *
   * For bishop, will add all diagonal moves from bishop.
   *
   * @param in_check_moves Boolean to test only capturing moves if in check.
   * @return Vector of possible moves of bishop.
   */
  std::vector<Move>
  get_candidate_moves(const bool in_check_moves = false) const override {
    return get_diagonal_moves();
  }
};

#endif