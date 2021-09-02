#ifndef PIECE_H
#define PIECE_H

#include "coordinate.h"
#include "move.h"

#include <vector>

class Board;

/**
 * Piece parent class. Contains utility functions to operator on pieces.
 */
class Piece {
public:
  /// Color enum.
  enum class Color { WHITE, BLACK };

  /// Piece type enum.
  enum class Type { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

  /**
   * Constructor for piece object.
   *
   * @param board Pointer to board object which the piece resides on.
   * @param color Color of piece.
   * @param coordinate Location of piece on board.
   * @param moved Boolean if piece has moved on board.
   */
  Piece(Board *board, Color color, Coordinate coordinate, bool moved = false)
      : m_board(board), m_color(color), m_coordinate(coordinate),
        m_moved(moved) {}

  /**
   * Destructor for piece object.
   */
  virtual ~Piece() {}

  /**
   * Getter for `m_color`.
   *
   * @return Color of piece.
   */
  Color get_color() const { return m_color; }

  /**
   * Getter for `m_coordinate`.
   *
   * @return Coordinate of piece.
   */
  Coordinate get_coordinate() const { return m_coordinate; }

  /**
   * Getter for piece type.
   *
   * @return Type of piece.
   */
  virtual Type get_type() const = 0;

  /**
   * Getter for `m_moved`.
   *
   * @return Boolean if piece has moved.
   */
  bool get_moved() const { return m_moved; }

  /**
   * Obtains a list of all possible moves of a piece depending on piece move
   * specifications.
   *
   * @param in_check_moves Boolean to test only capturing moves if in check.
   * @return Vector of possible moves of piece.
   */
  virtual std::vector<Move>
  get_candidate_moves(const bool in_check_moves = false) const = 0;

  /**
   * Obtains a list of all valid moves of a piece.
   *
   * @param in_check_moves Boolean to test only capturing moves if in check.
   * @return Vector of valid moves of piece.
   */
  std::vector<Move> get_moves(const bool in_check_moves = false) const;

  /**
   * Moves the current piece object on the board.
   *
   * @param move Move specifications to carry out.
   */
  void move(const Move &move);

protected:
  /// Pointer to board object which the piece resides on.
  Board *m_board;

  /// Color of piece object.
  Color m_color;

  /// Coordinate of piece object on board.
  Coordinate m_coordinate;

  /// Boolean if piece has moved.
  bool m_moved;

  /**
   * Helper function for rook and queen candidate moves.
   *
   * @return Vector of all possible straight moves from piece.
   */
  std::vector<Move> get_straight_moves() const;

  /**
   * Helper function for bishop and queen candidate moves.
   *
   * @return Vector of all possible diagonal moves from piece.
   */
  std::vector<Move> get_diagonal_moves() const;
};

#endif