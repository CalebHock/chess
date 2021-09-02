from enum import Enum

from src.coordinate import Coordinate


class MoveType(Enum):
    DEFAULT = 0
    CASTLE = 1
    EN_PASSANT = 3
    N_PROMO = 4
    B_PROMO = 5
    R_PROMO = 6
    Q_PROMO = 7
    DOUBLE = 8


class Move:
    origin: Coordinate
    dest: Coordinate
    move_type: MoveType
    capture: bool

    def __init__(
        self,
        origin: Coordinate,
        dest: Coordinate,
        capture: bool = False,
        move_type: MoveType = MoveType.DEFAULT,
    ) -> None:
        """

        Arguments:
        """

        self.origin = origin
        self.dest = dest
        self.capture = capture
        self.move_type = move_type

    def __str__(self) -> None:
        """
        
        Arguments:
        """

        output = str(self.origin) + " -> " + str(self.dest)
        if self.capture:
            output += " X"
        output += " " + str(self.move_type)
        return output
