from typing import Union

from src.coordinate import Coordinate
from src.move import Move, MoveType

file_dict = {
    'a': 0,
    'b': 1,
    'c': 2,
    'd': 3,
    'e': 4,
    'f': 5,
    'g': 6,
    'h': 7
}

file_inv_dict = {
    0: 'a',
    1: 'b',
    2: 'c',
    3: 'd',
    4: 'e',
    5: 'f',
    6: 'g',
    7: 'h'
}

def chess_to_coordinate(pos: str) -> Union[Coordinate, Move]:
    """

    Arguments:
    """
    
    if len(pos) == 2:
        return Coordinate(int(pos[1]) - 1, file_dict[pos[0]])
    
    else:
        if len(pos) == 5:
            if pos[4] == 'n':
                return Move(Coordinate(int(pos[1]) - 1, file_dict[pos[0]]), Coordinate(int(pos[3]) - 1, file_dict[pos[2]]), False, MoveType.N_PROMO)
            elif pos[4] == 'b':
                return Move(Coordinate(int(pos[1]) - 1, file_dict[pos[0]]), Coordinate(int(pos[3]) - 1, file_dict[pos[2]]), False, MoveType.B_PROMO)
            elif pos[4] == 'r':
                return Move(Coordinate(int(pos[1]) - 1, file_dict[pos[0]]), Coordinate(int(pos[3]) - 1, file_dict[pos[2]]), False, MoveType.R_PROMO)
            elif pos[4] == 'q':
                return Move(Coordinate(int(pos[1]) - 1, file_dict[pos[0]]), Coordinate(int(pos[3]) - 1, file_dict[pos[2]]), False, MoveType.Q_PROMO)
            
        else:
            return Move(Coordinate(int(pos[1]) - 1, file_dict[pos[0]]), Coordinate(int(pos[3]) - 1, file_dict[pos[2]]))

def move_to_chess(move: Move) -> str:
    """

    Arguments:
    """

    move_str: str = file_inv_dict[move.origin.get_file()] + str(move.origin.get_rank() + 1) + file_inv_dict[move.dest.get_file()] + str(move.dest.get_rank() + 1)

    if move.move_type == MoveType.N_PROMO:
        move_str += "n"
    elif move.move_type == MoveType.B_PROMO:
        move_str += "b"
    elif move.move_type == MoveType.R_PROMO:
        move_str += "r"
    elif move.move_type == MoveType.Q_PROMO:
        move_str += "q"

    return move_str 

def chess_to_move(move_str: str) -> Move:
    """
    
    Arguments:
    """
    move = Move()

    return move