#include "board.h"
#include "coordinate.h"
#include "move.h"
#include "piece.h"
#include "util.h"

#include <cstdlib>

int main() {
  srand(time(NULL));

  Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  // Board board("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1");
  std::cout << board << std::endl;
  // std::vector<Move> moves = board.piece_at(Coordinate(6, 0))->get_moves();
  // for (const Move &move : moves) {
  //   std::cout << move << std::endl;
  // }

  std::string str = "";
  Move move;
  while (true) {
    // White move
    move = find_move(board, Piece::Color::WHITE);
    board.piece_at(move.get_from())->move(move);

    std::cout << board << move << std::endl;
    // std::getline(std::cin, str);
    // if (str.length() != 0) {
    //   break;
    // }

    // Black move
    move = find_move(board, Piece::Color::BLACK);
    board.piece_at(move.get_from())->move(move);

    std::cout << board << move << std::endl;
    // std::getline(std::cin, str);
    // if (str.length() != 0) {
    //   break;
    // }
  }

  return 0;
}