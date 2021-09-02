#ifndef PAWN_H
#define PAWN_H

#include "../piece.h"

/**
 * Pawn piece class. Inherits utility from piece parent class.
 */
class Pawn : public Piece {
public:
  /**
   * Constructor for pawn piece object.
   *
   * @param board Pointer to board object which the pawn resides on.
   * @param color Color of pawn.
   * @param coordinate Location of pawn on board.
   * @param moved Boolean if pawn has moved on board.
   */
  Pawn(Board *board, Color color, Coordinate coordinate, bool moved = false)
      : Piece(board, color, coordinate, moved) {}

  /**
   * Getter for pawn type.
   *
   * @return Type::PAWN
   */
  Type get_type() const override { return Type::PAWN; }

  /**
   * Obtains a list of all possible moves of a pawn.
   *
   * For pawn, will add all single move forward, double move from start,
   * diagonal capture, em passant, and promotion moves.
   *
   * @param in_check_moves Boolean to test only capturing moves if in check.
   * @return Vector of possible moves of pawn.
   */
  std::vector<Move>
  get_candidate_moves(const bool in_check_moves = false) const override;
};

#endif