#ifndef MOVE_H
#define MOVE_H

#include "coordinate.h"

#include <iostream>

/**
 * Move class.
 */
class Move {
public:
  /// Move type enum
  enum class MoveType {
    DEFAULT,
    QUEEN_CASTLE,
    KING_CASTLE,
    EM_PASSANT,
    DOUBLE,
    KNIGHT_PROMOTION,
    BISHOP_PROMOTION,
    ROOK_PROMOTION,
    QUEEN_PROMOTION
  };

  /**
   * Constructs move object with move {-1, -1} -> {-1, -1}, default type.
   */
  Move()
      : m_from(Coordinate()), m_dest(Coordinate()),
        m_move_type(MoveType::DEFAULT){};

  /**
   * Constructs move object with given from/dest and type.
   *
   * @param from Coordinate for origin of move.
   * @param dest Coordinate for destination of move.
   * @param move_type Type of move.
   */
  Move(const Coordinate &from, const Coordinate &dest,
       const MoveType move_type = MoveType::DEFAULT)
      : m_from(from), m_dest(dest), m_move_type(move_type){};

  /**
   * Getter for `m_from`.
   *
   * @return Origin of move.
   */
  Coordinate get_from() const { return m_from; }

  /**
   * Getter for `m_dest`.
   *
   * @return Destination of move.
   */
  Coordinate get_dest() const { return m_dest; }

  /**
   * Getter for `m_move_type`.
   *
   * @return Type of move.
   */
  MoveType get_type() const { return m_move_type; }

  /**
   * Overload equality operator for move object.
   *
   * @param m1 Move 1.
   * @param m2 Move 2.
   * @return Boolean if origin and destination of both moves are both equal.
   */
  friend bool operator==(const Move &m1, const Move &m2);

  /**
   * Overload insertion operator for move object.
   *
   * @param os Stream to output to.
   * @param m Move to output.
   * @return Reference to stream outputted to.
   */
  friend std::ostream &operator<<(std::ostream &os, const Move &m);

private:
  /// Origin of move
  Coordinate m_from;

  // Destination of move
  Coordinate m_dest;

  // Type of move (e.g. DEFAULT, QUEEN_CASTLE, KING_CASTLE, EM_PASSANT, DOUBLE,
  // KNIGHT_PROMOTION, BISHOP_PROMOTION, ROOK_PROMOTION, QUEEN_PROMOTION)
  MoveType m_move_type;
};

#endif