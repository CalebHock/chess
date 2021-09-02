from typing import List, Tuple
from enum import Enum
from copy import deepcopy

from src.coordinate import Coordinate


class Color(Enum):
    WHITE = 0
    BLACK = 1


class Type(Enum):
    PAWN = 0
    KNIGHT = 1
    BISHOP = 2
    ROOK = 3
    QUEEN = 4
    KING = 5


class Piece:
    coordinate: Coordinate
    color: Color
    moved: bool
    piece_type: Type

    def __init__(
        self,
        color: Color,
        coordinate: Coordinate,
        piece_type: Type,
        moved: bool = False,
    ) -> None:
        """

        Arguments:
        """

        self.color = color
        self.coordinate = coordinate
        self.piece_type = piece_type
        self.moved = moved

    def __str__(self) -> None:
        return self.color.name + " " + self.piece_type.name
