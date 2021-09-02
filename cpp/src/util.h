#ifndef UTIL_H
#define UTIL_H

#include "coordinate.h"
#include "move.h"
#include "piece.h"

#include <string>
#include <unordered_map>

/**
 * Converts a uci coordinate string into a coordinate object.
 *
 * @param coordinate String.
 * @return Coordinate object converted from string.
 */
Coordinate uci_to_coordinate(const std::string &coordinate);

/**
 * Converts a uci move string into a move object.
 *
 * @param move String.
 * @param piece_type Type of piece at origin of move.
 * @param empassant_target Em passant target of board.
 * @return Move object converted from string.
 */
Move uci_to_move(const std::string &move, const Piece::Type piece_type,
                 const Coordinate &empassant_target);

/**
 * Converts a move object into a uci string.
 *
 * @param move Move object.
 * @return String converted from move object.
 */
std::string move_to_uci(const Move &move);

/**
 * Converts a coordinate object into a uci string.
 *
 * @param coordinate Cove object.
 * @return String converted from coordinate object.
 */
std::string coordinate_to_uci(const Coordinate &coordinate);

#endif