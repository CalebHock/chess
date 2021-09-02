#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"

/**
 * Queen piece class. Inherits utility from piece parent class.
 */
class Queen : public Piece {
public:
  /**
   * Constructor for queen piece object.
   *
   * @param board Pointer to board object which the queen resides on.
   * @param color Color of queen.
   * @param coordinate Location of queen on board.
   * @param moved Boolean if queen has moved on board.
   */
  Queen(Board *board, Color color, Coordinate coordinate, bool moved = false)
      : Piece(board, color, coordinate, moved) {}

  /**
   * Getter for queen type.
   *
   * @return Type::QUEEN
   */
  Type get_type() const override { return Type::QUEEN; }

  /**
   * Obtains a list of all possible moves of a queen.
   *
   * For queen, will add all straight and diagonal moves from queen.
   *
   * @param in_check_moves Boolean to test only capturing moves if in check.
   * @return Vector of possible moves of queen.
   */
  std::vector<Move>
  get_candidate_moves(const bool in_check_moves = false) const override;
};

#endif