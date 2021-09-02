#ifndef BOARD_H
#define BOARD_H

#include "coordinate.h"
#include "move.h"
#include "piece.h"

#include <iostream>
#include <string>
#include <vector>

/**
 * Chess board class. Contains board storage of pieces pointers and utility
 * functions to operator on and obtain data from board.
 */
class Board {
public:
  /// Constant dimensions of chess board.
  static const int SIZE = 8;

  friend class Piece;
  friend class Pawn;

  /**
   * Constructs board object with default board layout.
   */
  Board();

  /**
   * Constructs board object with provided fen layout.
   *
   * @param fen Layout of current chess board state, (e.g.
   * rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1).
   */
  Board(const std::string fen);

  /**
   * Constructs board object from provided board.
   *
   * @param b Board object to create copy of.
   */
  Board(const Board &b) { copy(b); }

  /**
   * Overload of the assignment operator for board object.
   *
   * @param b Board object to assign from.
   * @return Reference to board object from result of copy.
   */
  Board &operator=(const Board &b);

  /**
   * Destructor for board object.
   */
  ~Board() { destroy(); }

  /**
   * Determines if a coordinate is within the board.
   *
   * @param coordinate Coordinate to check if within board.
   * @return Boolean value if `coordinate` is within bounds.
   */
  bool contains(const Coordinate &coordinate) const {
    return coordinate.get_rank() >= 0 && coordinate.get_file() >= 0 &&
           coordinate.get_rank() < SIZE && coordinate.get_file() < SIZE;
  }

  /**
   * Determines if the provided color is in check in current board state.
   *
   * @param color Color to check if in check.
   * @return Boolean value if `color` is in check.
   */
  bool in_check(const Piece::Color color) const;

  /**
   * Determines if the provided color is checkmated in current board state.
   *
   * @param color Color to check if checkmated.
   * @return Boolean value if `color` is checkmated.
   */
  bool checkmated(const Piece::Color color) const;

  /**
   * Determines if the provided color is stalemated in current board state.
   *
   * @param color Color to check if stalemated.
   * @return Boolean value if `color` is stalemated.
   */
  bool stalemated(const Piece::Color color) const;

  /**
   * Determines if there is a draw on the current board state.
   *
   * @return Boolean value if board state is in draw.
   */
  bool draw() const;

  /**
   * Find piece at coordinate on board.
   * If no piece is at coordinate, a nullptr is returned.
   *
   * @param coordinate Coordinate to find piece on board.
   * @return Pointer to piece at `coordinate`.
   */
  Piece *piece_at(const Coordinate &coordinate);

  /**
   * Find piece at coordinate on board.
   * If no piece is at coordinate, a nullptr is returned.
   *
   * @param coordinate Coordinate to find piece on board.
   * @return Pointer to constant piece at `coordinate`.
   */
  const Piece *piece_at(const Coordinate &coordinate) const;

  /**
   * Construct vector of piece pointers of a given color
   *
   * @param color Color to get pieces of.
   * @return Vector of pointers to pieces of `color`.
   */
  std::vector<Piece *> get_pieces(const Piece::Color color);

  /**
   * Construct vector of piece pointers of a given color
   *
   * @param color Color to get pieces of.
   * @return Vector of pointers to constant pieces of `color`.
   */
  std::vector<const Piece *> get_pieces(const Piece::Color color) const;

  /**
   * Construct vector of valid moves of a given color
   *
   * @param color Color to get moves of.
   * @return Vector of moves of `color`.
   */
  std::vector<Move> get_moves(const Piece::Color color) const;

  /**
   * Appends move to `m_move_history`.
   *
   * @param move Move to add to move history.
   */
  void add_move(const Move &move) { m_move_history.push_back(move); }

  /**
   * Getter for `m_empassant_target`.
   *
   * @return Coordinate of the current em passant target.
   */
  Coordinate get_empassant_target() { return m_empassant_target; }

  /**
   * Overload insertion operator for board object.
   *
   * @param os Stream to output to.
   * @param b Board to output.
   * @return Reference to stream outputted to.
   */
  friend std::ostream &operator<<(std::ostream &os, const Board &b);

  ////////////////////////////////////////////////////////////////////////////////
  ///                           ASSIGNMENT FUNCTIONS ///
  ////////////////////////////////////////////////////////////////////////////////

  /**
   * Calculates score of `color` using standard piece valutations.
   * Used as the heuristic function for the move selection algorithm.
   *
   * (pawn : 1, knight : 3, bishop : 3, rook : 5, queen : 9, king : 4).
   *
   * @param color Color to get score of.
   * @return Total piece value of `color`.
   */
  int get_score(const Piece::Color color) const;

  /**
   * Finds best move using the assignment specified algorithm.
   * This assignment is using Iterative-Deepening Depth-Limited MiniMax.
   *
   * @param b Board to find move on.
   * @param color Color to find move of.
   * @return Move found using algorithm.
   */
  friend Move find_move(const Board &b, Piece::Color color);

  /**
   * Find the best move at the current depth limit.
   *
   * @param b Board to find move on.
   * @param color Color to find move of.
   * @param depth Current depth limit.
   * @return Pair of move and int score of best move.
   */
  friend std::pair<Move, int> max_choice(const Board &b, Piece::Color color,
                                         int depth);

  /**
   * Find the best move for other color at the current depth limit.
   *
   * @param b Board to find move on.
   * @param color Color to find move of.
   * @param depth Current depth limit.
   * @return Int score of best move.
   */
  friend int min_value(const Board &b, Piece::Color color, int depth);

  /**
   * Find the best move at the current depth limit.
   *
   * @param b Board to find move on.
   * @param color Color to find move of.
   * @param depth Current depth limit.
   * @return Int score of best move.
   */
  friend int max_value(const Board &b, Piece::Color color, int depth);

private:
  /// 2-D array of piece pointers represeting the physical board.
  Piece ***m_board;

  /// Move history of board.
  std::vector<Move> m_move_history;

  /// Coordinate of current em passant target. {-1, -1} if no target.
  Coordinate m_empassant_target;

  /// Counter until draw if eligible.
  int m_draw_counter;

  /// Current color to move.
  Piece::Color m_current_move;

  /**
   * Helper function for default and parameter constructors.
   *
   * @param fen String to construct board from.
   */
  void constructor(const std::string fen);

  /**
   * Helper function for copy constructor and operator=.
   *
   * @param b Board to copy.
   */
  void copy(const Board &b);

  /**
   * Helper function for destructor and operator=.
   */
  void destroy();
};

#endif