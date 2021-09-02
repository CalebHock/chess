#include "util.h"

#include <cmath>

Coordinate uci_to_coordinate(const std::string &coordinate) {
  std::unordered_map<char, int> file_dict;
  file_dict['a'] = 0;
  file_dict['b'] = 1;
  file_dict['c'] = 2;
  file_dict['d'] = 3;
  file_dict['e'] = 4;
  file_dict['f'] = 5;
  file_dict['g'] = 6;
  file_dict['h'] = 7;

  return Coordinate(int(coordinate[1]) - '0' - 1, file_dict[coordinate[0]]);
}

Move uci_to_move(const std::string &move, const Piece::Type piece_type,
                 const Coordinate &empassant_target) {
  std::unordered_map<char, int> file_dict;
  file_dict['a'] = 0;
  file_dict['b'] = 1;
  file_dict['c'] = 2;
  file_dict['d'] = 3;
  file_dict['e'] = 4;
  file_dict['f'] = 5;
  file_dict['g'] = 6;
  file_dict['h'] = 7;

  Move base_move(Coordinate(int(move[1]) - '0' - 1, file_dict[move[0]]),
                 Coordinate(int(move[3]) - '0' - 1, file_dict[move[2]]));

  // Determine move type from board and move details.
  Move::MoveType move_type;
  if (piece_type == Piece::Type::PAWN &&
      abs(base_move.get_from().get_rank() - base_move.get_dest().get_rank()) ==
          2) {
    move_type = Move::MoveType::DOUBLE;
  } else if (piece_type == Piece::Type::PAWN &&
             base_move.get_dest() == empassant_target) {
    move_type = Move::MoveType::EM_PASSANT;
  } else if (piece_type == Piece::Type::KING &&
             abs(base_move.get_from().get_file() -
                 base_move.get_dest().get_file()) == 2 &&
             base_move.get_from().get_file() >
                 base_move.get_dest().get_file()) {
    move_type = Move::MoveType::QUEEN_CASTLE;
  } else if (piece_type == Piece::Type::KING &&
             abs(base_move.get_from().get_file() -
                 base_move.get_dest().get_file()) == 2 &&
             base_move.get_from().get_file() <
                 base_move.get_dest().get_file()) {
    move_type = Move::MoveType::KING_CASTLE;
  } else if (move.length() > 4) {
    switch (move[4]) {
    case 'n':
      move_type = Move::MoveType::KNIGHT_PROMOTION;
      break;
    case 'b':
      move_type = Move::MoveType::BISHOP_PROMOTION;
      break;
    case 'r':
      move_type = Move::MoveType::ROOK_PROMOTION;
      break;
    case 'q':
      move_type = Move::MoveType::QUEEN_PROMOTION;
      break;
    default:
      break;
    }
  } else {
    move_type = Move::MoveType::DEFAULT;
  }

  return Move(base_move.get_from(), base_move.get_dest(), move_type);
}

std::string move_to_uci(const Move &move) {
  std::unordered_map<int, char> file_dict;
  file_dict[0] = 'a';
  file_dict[1] = 'b';
  file_dict[2] = 'c';
  file_dict[3] = 'd';
  file_dict[4] = 'e';
  file_dict[5] = 'f';
  file_dict[6] = 'g';
  file_dict[7] = 'h';

  std::string move_string = file_dict[move.get_from().get_file()] +
                            std::to_string(move.get_from().get_rank() + 1) +
                            file_dict[move.get_dest().get_file()] +
                            std::to_string(move.get_dest().get_rank() + 1);

  switch (move.get_type()) {
  case Move::MoveType::KNIGHT_PROMOTION:
    move_string += 'n';
    break;
  case Move::MoveType::BISHOP_PROMOTION:
    move_string += 'b';
    break;
  case Move::MoveType::ROOK_PROMOTION:
    move_string += 'r';
    break;
  case Move::MoveType::QUEEN_PROMOTION:
    move_string += 'q';
    break;
  default:
    break;
  }

  return move_string;
}

std::string coordinate_to_uci(const Coordinate &coordinate) {
  std::unordered_map<int, char> file_dict;
  file_dict[0] = 'a';
  file_dict[1] = 'b';
  file_dict[2] = 'c';
  file_dict[3] = 'd';
  file_dict[4] = 'e';
  file_dict[5] = 'f';
  file_dict[6] = 'g';
  file_dict[7] = 'h';

  return file_dict[coordinate.get_file()] +
         std::to_string(coordinate.get_rank() + 1);
}