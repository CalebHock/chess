class Coordinate:
    _rank: int
    _file: int

    def __init__(self, r: int, f: int) -> None:
        self._rank = r
        self._file = f

    def __str__(self) -> None:
        return "(" + str(self._file) + ", " + str(self._rank) + ")"

    def __eq__(self, coord):
        return self._rank == coord._rank and self._file == coord._file

    def get_rank(self) -> int:
        return self._rank

    def get_file(self) -> int:
        return self._file

    def to_syntax(self) -> str:
        pass
