#include "board.h"

#include "piece/bishop.h"
#include "piece/king.h"
#include "piece/knight.h"
#include "piece/pawn.h"
#include "piece/queen.h"
#include "piece/rook.h"

#include "util.h"
#include <bits/stdc++.h>

Board::Board() {
  constructor("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

Board::Board(const std::string fen_string) { constructor(fen_string); }

void Board::constructor(const std::string fen_string) {
  // Build 2-D array of piece pointers.
  m_board = new Piece **[SIZE];
  for (int rank = 0; rank < SIZE; ++rank) {
    m_board[rank] = new Piece *[SIZE];
    for (int file = 0; file < SIZE; ++file) {
      m_board[rank][file] = nullptr;
    }
  }

  std::string piece_locations = fen_string.substr(0, fen_string.find(' '));
  std::string info = fen_string.substr(piece_locations.length() + 1);

  m_current_move = (info[0] == 'w') ? Piece::Color::WHITE : Piece::Color::BLACK;
  m_draw_counter = 0;

  info = info.substr(info.find(' ') + 1);
  std::string rook_info = info.substr(0, info.find(' '));

  // Create every piece object that exists on board according to the fen_string.
  std::string board_row;
  for (int rank = SIZE - 1; rank >= 0; --rank) {
    board_row = piece_locations.substr(0, piece_locations.find('/'));
    piece_locations = piece_locations.substr(piece_locations.find('/') + 1);
    for (int file = 0, str_file = 0; file < SIZE; ++file, ++str_file) {
      switch (board_row[str_file]) {
      // Black pawns
      case 'p':
        m_board[rank][file] =
            new Pawn(this, Piece::Color::BLACK, Coordinate(rank, file),
                     !(rank == SIZE - 2));
        break;

      // Black knights
      case 'n':
        m_board[rank][file] =
            new Knight(this, Piece::Color::BLACK, Coordinate(rank, file));
        break;

      // Black bishops
      case 'b':
        m_board[rank][file] =
            new Bishop(this, Piece::Color::BLACK, Coordinate(rank, file));
        break;

      // Black rooks
      case 'r':
        m_board[rank][file] =
            new Rook(this, Piece::Color::BLACK, Coordinate(rank, file),
                     !((rook_info.find('q') != std::string::npos && rank == 7 &&
                        file == 0) ||
                       (rook_info.find('k') != std::string::npos && rank == 7 &&
                        file == 7)));
        break;

      // Black queen
      case 'q':
        m_board[rank][file] =
            new Queen(this, Piece::Color::BLACK, Coordinate(rank, file));
        break;

      // Black king
      case 'k':
        m_board[rank][file] =
            new King(this, Piece::Color::BLACK, Coordinate(rank, file),
                     !(rook_info.find('q') != std::string::npos ||
                       rook_info.find('k') != std::string::npos));
        break;

      // White pawns
      case 'P':
        m_board[rank][file] = new Pawn(this, Piece::Color::WHITE,
                                       Coordinate(rank, file), !(rank == 1));
        break;

      // White knights
      case 'N':
        m_board[rank][file] =
            new Knight(this, Piece::Color::WHITE, Coordinate(rank, file));
        break;

      // White bishops
      case 'B':
        m_board[rank][file] =
            new Bishop(this, Piece::Color::WHITE, Coordinate(rank, file));
        break;

      // White rooks
      case 'R':
        m_board[rank][file] =
            new Rook(this, Piece::Color::WHITE, Coordinate(rank, file),
                     !((rook_info.find('Q') != std::string::npos && rank == 0 &&
                        file == 0) ||
                       (rook_info.find('K') != std::string::npos && rank == 0 &&
                        file == 7)));
        break;

      // White queen
      case 'Q':
        m_board[rank][file] =
            new Queen(this, Piece::Color::WHITE, Coordinate(rank, file));
        break;

      // White king
      case 'K':
        m_board[rank][file] =
            new King(this, Piece::Color::WHITE, Coordinate(rank, file),
                     !(rook_info.find('Q') != std::string::npos ||
                       rook_info.find('K') != std::string::npos));
        break;

      // Increment board_file to skip files if number in fen_string row.
      default:
        file += board_row[str_file] - '0' - 1;
        continue;
      }
    }
  }
}

Board &Board::operator=(const Board &b) {
  if (this != &b) {
    destroy();
    copy(b);
  }
  return *this;
}

bool Board::in_check(const Piece::Color color) const {
  // If any piece of opposite color is attacking color king, board is in check.
  for (const Piece *piece :
       get_pieces(color == Piece::Color::WHITE ? Piece::Color::BLACK
                                               : Piece::Color::WHITE)) {
    for (const Move &move : piece->get_moves(true)) {
      if (piece_at(Coordinate(move.get_dest())) &&
          piece_at(Coordinate(move.get_dest()))->get_type() ==
              Piece::Type::KING &&
          piece_at(Coordinate(move.get_dest()))->get_color() == color) {
        return true;
      }
    }
  }
  return false;
}

bool Board::checkmated(const Piece::Color color) const {
  // If color is not in check, color is not checkmated.
  if (!in_check(color)) {
    return false;
  }

  // If color has no valid moves, color is checkmated.
  for (const Piece *piece : get_pieces(color)) {
    if (piece->get_color() == color && !(piece->get_moves().empty())) {
      return false;
    }
  }
  return true;
}

bool Board::stalemated(const Piece::Color color) const {
  // If color is in check, color is not stalemated.
  if (in_check(color)) {
    return false;
  }

  // If color has no valid moves, color is stalemated.
  for (const Piece *piece : get_pieces(color)) {
    if (piece->get_color() == color && !(piece->get_moves().empty())) {
      return false;
    }
  }
  return true;
}

bool Board::draw() const {
  // If draw counter is not > 8, there exists no draw.
  if (m_draw_counter < 8) {
    return false;
  }
  if (m_draw_counter >= 50) {
    return true;
  }

  // Check move history if in last 8 moves, if moves 0, 1, 2, 3 are the same as
  // 4, 5, 6, 7; board is in draw.
  for (int i = m_move_history.size() - 1; i >= int(m_move_history.size()) - 4;
       --i) {
    if (!(m_move_history[i] == m_move_history[i - 4])) {
      return false;
    }
  }
  return true;
}

Piece *Board::piece_at(const Coordinate &coordinate) {
  // If coordinate is not in board, return nullptr.
  if (!contains(coordinate)) {
    return nullptr;
  }
  return m_board[coordinate.get_rank()][coordinate.get_file()];
}

const Piece *Board::piece_at(const Coordinate &coordinate) const {
  // If coordinate is not in board, return nullptr.
  if (!contains(coordinate)) {
    return nullptr;
  }
  return m_board[coordinate.get_rank()][coordinate.get_file()];
}

std::vector<Piece *> Board::get_pieces(const Piece::Color color) {
  // Iterate through board, add any piece of color to vector.
  std::vector<Piece *> pieces;
  for (int rank = 0; rank < SIZE; ++rank) {
    for (int file = 0; file < SIZE; ++file) {
      Piece *piece = piece_at(Coordinate(rank, file));
      if (piece != nullptr && piece->get_color() == color) {
        pieces.push_back(piece);
      }
    }
  }
  return pieces;
}

std::vector<const Piece *> Board::get_pieces(const Piece::Color color) const {
  // Iterate through board, add any piece of color to vector.
  std::vector<const Piece *> pieces;
  for (int rank = 0; rank < SIZE; ++rank) {
    for (int file = 0; file < SIZE; ++file) {
      const Piece *piece = piece_at(Coordinate(rank, file));
      if (piece != nullptr && piece->get_color() == color) {
        pieces.push_back(piece);
      }
    }
  }
  return pieces;
}

std::vector<Move> Board::get_moves(const Piece::Color color) const {
  // Iterate through pieces, add all valid moves to vector.
  std::vector<Move> moves;
  std::vector<const Piece *> pieces = get_pieces(color);
  for (const Piece *piece : pieces) {
    std::vector<Move> piece_moves = piece->get_moves();
    moves.insert(moves.end(), piece_moves.begin(), piece_moves.end());
  }
  return moves;
}

void Board::copy(const Board &b) {
  // Build 2-D array of piece pointers.
  m_board = new Piece **[SIZE];
  for (int rank = 0; rank < SIZE; ++rank) {
    m_board[rank] = new Piece *[SIZE];
    for (int file = 0; file < SIZE; ++file) {
      m_board[rank][file] = nullptr;
    }
  }

  // Copy board data
  m_current_move = b.m_current_move;
  m_draw_counter = b.m_draw_counter;
  m_empassant_target = Coordinate(b.m_empassant_target);

  for (Move move : b.m_move_history) {
    m_move_history.push_back(move);
  }

  // Create every piece object that exists on copied board.
  for (int rank = 0; rank < SIZE; ++rank) {
    for (int file = 0; file < SIZE; ++file) {
      if (b.piece_at(Coordinate(rank, file))) {
        switch (b.piece_at(Coordinate(rank, file))->get_type()) {
        case Piece::Type::PAWN:
          m_board[rank][file] =
              new Pawn(this, b.piece_at(Coordinate(rank, file))->get_color(),
                       Coordinate(rank, file),
                       b.piece_at(Coordinate(rank, file))->get_moved());
          break;
        case Piece::Type::KNIGHT:
          m_board[rank][file] =
              new Knight(this, b.piece_at(Coordinate(rank, file))->get_color(),
                         Coordinate(rank, file));
          break;
        case Piece::Type::BISHOP:
          m_board[rank][file] =
              new Bishop(this, b.piece_at(Coordinate(rank, file))->get_color(),
                         Coordinate(rank, file));
          break;
        case Piece::Type::ROOK:
          m_board[rank][file] =
              new Rook(this, b.piece_at(Coordinate(rank, file))->get_color(),
                       Coordinate(rank, file),
                       b.piece_at(Coordinate(rank, file))->get_moved());
          break;
        case Piece::Type::QUEEN:
          m_board[rank][file] =
              new Queen(this, b.piece_at(Coordinate(rank, file))->get_color(),
                        Coordinate(rank, file));
          break;
        case Piece::Type::KING:
          m_board[rank][file] =
              new King(this, b.piece_at(Coordinate(rank, file))->get_color(),
                       Coordinate(rank, file),
                       b.piece_at(Coordinate(rank, file))->get_moved());
          break;
        }
      }
    }
  }

  return;
}

void Board::destroy() {
  for (int rank = 0; rank < SIZE; ++rank) {
    for (int file = 0; file < SIZE; ++file) {
      delete m_board[rank][file];
    }
    delete[] m_board[rank];
  }
  delete[] m_board;
  return;
}

std::ostream &operator<<(std::ostream &os, const Board &b) {
  for (int rank = b.SIZE - 1; rank >= 0; --rank) {
    for (int file = 0; file < b.SIZE; ++file) {
      os << '[';
      char piece_char = ' ';
      if (b.piece_at(Coordinate(rank, file))) {
        switch (b.piece_at(Coordinate(rank, file))->get_type()) {
        case Piece::Type::PAWN:
          piece_char = 'p';
          break;
        case Piece::Type::KNIGHT:
          piece_char = 'n';
          break;
        case Piece::Type::BISHOP:
          piece_char = 'b';
          break;
        case Piece::Type::ROOK:
          piece_char = 'r';
          break;
        case Piece::Type::QUEEN:
          piece_char = 'q';
          break;
        case Piece::Type::KING:
          piece_char = 'k';
          break;
        }
        // If piece color is white, upper case.
        if (b.piece_at(Coordinate(rank, file))->get_color() ==
            Piece::Color::WHITE) {
          piece_char = toupper(piece_char);
        }
      }
      os << piece_char;
      os << ']';
    }
    os << std::endl;
  }
  return os;
}

////////////////////////////////////////////////////////////////////////////////
///                           ASSIGNMENT FUNCTIONS                           ///
////////////////////////////////////////////////////////////////////////////////

int Board::get_score(const Piece::Color color) const {
  int score = 0;
  Piece::Color opposite_color = (color == Piece::Color::WHITE)
                                    ? Piece::Color::BLACK
                                    : Piece::Color::WHITE;
  for (const Piece *piece : get_pieces(color)) {
    switch (piece->get_type()) {
    case Piece::Type::PAWN:
      score += 1;
      break;
    case Piece::Type::KNIGHT:
      score += 3;
      break;
    case Piece::Type::BISHOP:
      score += 3;
      break;
    case Piece::Type::ROOK:
      score += 5;
      break;
    case Piece::Type::QUEEN:
      score += 9;
      break;
    case Piece::Type::KING:
      score += 4;
      break;
    }
  }
  for (const Piece *piece : get_pieces(opposite_color)) {
    switch (piece->get_type()) {
    case Piece::Type::PAWN:
      score -= 1;
      break;
    case Piece::Type::KNIGHT:
      score -= 3;
      break;
    case Piece::Type::BISHOP:
      score -= 3;
      break;
    case Piece::Type::ROOK:
      score -= 5;
      break;
    case Piece::Type::QUEEN:
      score -= 9;
      break;
    case Piece::Type::KING:
      score -= 4;
      break;
    }
  }
  return score;
}

Move find_move(const Board &b, Piece::Color color) {
  const int MAX_DEPTH = 3;
  std::pair<Move, int> choice_action(Move(), INT_MIN),
      max_action(Move(), INT_MIN);

  // Use iterative deepening to find best move using depth limited minimax
  for (int i = 1; i < MAX_DEPTH; ++i) {
    Board choice_board(b);
    choice_action = max_choice(choice_board, color, i);
    if (choice_action.second > max_action.second) {
      max_action = choice_action;
    }
  }
  return max_action.first;
}

std::pair<Move, int> max_choice(const Board &b, Piece::Color color, int depth) {
  int action_val = 0;
  std::pair<Move, int> max_action(Move(), INT_MIN);

  // Depth limited minimax
  for (const Move move : b.get_moves(color)) {
    Board choice_board(b);
    choice_board.piece_at(move.get_from())->move(move);
    action_val = min_value(choice_board, color, depth - 1);
    if (action_val > max_action.second) {
      max_action = std::pair<Move, int>(move, action_val);
    }
  }
  return max_action;
}

int min_value(const Board &b, Piece::Color color, int depth) {
  int action_val = 0, min_action = INT_MAX - 1;
  Piece::Color opposite_color = (color == Piece::Color::WHITE)
                                    ? Piece::Color::BLACK
                                    : Piece::Color::WHITE;

  // Avoid draw/stalemate other color
  if (b.draw() || b.stalemated(opposite_color)) {
    return INT_MIN + 1;
  }
  // Prefer check/checkmate other color
  if (b.in_check(opposite_color) || b.checkmated(opposite_color)) {
    return min_action;
  }
  // Base case
  if (depth == 0) {
    return b.get_score(color);
  }

  // Iterate through every valid move
  for (const Move move : b.get_moves(opposite_color)) {
    Board choice_board(b);
    choice_board.piece_at(move.get_from())->move(move);
    action_val = max_value(choice_board, color, depth - 1);
    if (action_val < min_action) {
      min_action = action_val;
    }
  }
  return min_action;
}

int max_value(const Board &b, Piece::Color color, int depth) {
  int action_val = 0, max_action = INT_MIN + 1;

  // Avoid draw/stalemate other color
  if (b.draw() || b.stalemated(color)) {
    return INT_MAX - 1;
  }
  // Prefer check/checkmate other color
  if (b.in_check(color) || b.checkmated(color)) {
    return max_action;
  }
  // Base case
  if (depth == 0) {
    return b.get_score(color);
  }

  // Iterate through every valid move
  for (const Move move : b.get_moves(color)) {
    Board choice_board(b);
    choice_board.piece_at(move.get_from())->move(move);
    action_val = min_value(choice_board, color, depth - 1);
    if (action_val > max_action) {
      max_action = action_val;
    }
  }
  return max_action;
}
