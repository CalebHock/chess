#include "piece.h"
#include "board.h"

#include "bishop.h"
#include "knight.h"
#include "queen.h"
#include "rook.h"

std::vector<Move> Piece::get_moves(const bool in_check_moves) const {
  // All moves of piece, whether valid or not.
  std::vector<Move> candidate_moves = get_candidate_moves(in_check_moves);
  // All valid moves of piece.
  std::vector<Move> moves;

  for (const Move &move : candidate_moves) {
    // If move destination is not in board, do not add to valid moves.
    if (!m_board->contains(move.get_dest())) {
      continue;
    }

    // If piece captures own color piece, do not add to valid moves.
    if (m_board->piece_at(move.get_dest()) != nullptr &&
        m_board->piece_at(move.get_dest())->get_color() == m_color) {
      continue;
    }

    // If move puts board in check for color, do not add to valid moves.
    if (!in_check_moves) {
      Board candidate_board(*m_board);
      candidate_board.piece_at(m_coordinate)->move(move);
      if (candidate_board.in_check(m_color)) {
        continue;
      }
    }

    moves.push_back(move);
  }

  return moves;
}

void Piece::move(const Move &move) {
  // Get rank and file of destination.
  int rank = move.get_dest().get_rank(), file = move.get_dest().get_file();

  // Update draw counter on board.
  if (m_board->piece_at(move.get_dest()) != nullptr ||
      get_type() == Piece::Type::PAWN) {
    m_board->m_draw_counter = 0;
  } else {
    m_board->m_draw_counter += 1;
  }

  // Phyiscally move piece on board object. Update piece data.
  m_board->m_board[m_coordinate.get_rank()][m_coordinate.get_file()] = nullptr;
  delete m_board->m_board[rank][file];
  m_board->m_board[rank][file] = this;
  m_moved = true;
  m_coordinate = move.get_dest();

  // Update `m_empassant_target` of board if pawn double move.
  if (move.get_type() == Move::MoveType::DOUBLE) {
    m_board->m_empassant_target =
        Coordinate(rank + ((m_color == Color::WHITE) ? -1 : 1), file);
  } else {
    m_board->m_empassant_target = Coordinate(-1, -1);
  }

  // Add move to board move history.
  m_board->add_move(move);

  /*
    Handle special case moves.

    1. Change piece type if pawn promotion.
    2. Move rook if castling move.
    3. Remove enemy pawn if em passant move.
  */
  switch (move.get_type()) {
  case Move::MoveType::KNIGHT_PROMOTION:
    m_board->m_board[rank][file] =
        new Knight(m_board, get_color(), move.get_dest(), true);
    delete this;
    break;

  case Move::MoveType::BISHOP_PROMOTION:
    m_board->m_board[rank][file] =
        new Bishop(m_board, get_color(), move.get_dest(), true);
    delete this;
    break;

  case Move::MoveType::ROOK_PROMOTION:
    m_board->m_board[rank][file] =
        new Rook(m_board, get_color(), move.get_dest(), true);
    delete this;
    break;

  case Move::MoveType::QUEEN_PROMOTION:
    m_board->m_board[rank][file] =
        new Queen(m_board, get_color(), move.get_dest(), true);
    delete this;
    break;

  case Move::MoveType::QUEEN_CASTLE:
    m_board->m_board[rank][file + 1] = m_board->m_board[rank][0];
    m_board->m_board[rank][0] = nullptr;
    m_board->m_board[rank][file + 1]->m_moved = true;
    break;

  case Move::MoveType::KING_CASTLE:
    m_board->m_board[rank][file - 1] = m_board->m_board[rank][7];
    m_board->m_board[rank][7] = nullptr;
    m_board->m_board[rank][file - 1]->m_moved = true;
    break;

  case Move::MoveType::EM_PASSANT:
    delete m_board
        ->m_board[rank + (m_color == Piece::Color::WHITE ? -1 : 1)][file];
    m_board->m_board[rank + (m_color == Piece::Color::WHITE ? -1 : 1)][file] =
        nullptr;
    break;

  default:
    break;
  }
}

std::vector<Move> Piece::get_straight_moves() const {
  std::vector<Move> straight_moves;

  std::vector<std::vector<int>> dirs{{1, 0}, {0, -1}, {-1, 0}, {0, 1}};
  for (const std::vector<int> &dir : dirs) {
    for (Coordinate dest(m_coordinate.get_rank() + dir[0],
                         m_coordinate.get_file() + dir[1]);
         m_board->contains(dest); dest = Coordinate(dest.get_rank() + dir[0],
                                                    dest.get_file() + dir[1])) {
      straight_moves.push_back(Move(m_coordinate, dest));
      if (m_board->piece_at(dest) != nullptr) {
        break;
      }
    }
  }

  return straight_moves;
}

std::vector<Move> Piece::get_diagonal_moves() const {
  std::vector<Move> diagonal_moves;

  std::vector<std::vector<int>> dirs{{1, 1}, {1, -1}, {-1, -1}, {-1, 1}};
  for (const std::vector<int> &dir : dirs) {
    for (Coordinate dest(m_coordinate.get_rank() + dir[0],
                         m_coordinate.get_file() + dir[1]);
         m_board->contains(dest); dest = Coordinate(dest.get_rank() + dir[0],
                                                    dest.get_file() + dir[1])) {
      diagonal_moves.push_back(Move(m_coordinate, dest));
      if (m_board->piece_at(dest) != nullptr) {
        break;
      }
    }
  }

  return diagonal_moves;
}