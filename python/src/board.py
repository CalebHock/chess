from typing import Union, Tuple, List, Dict, Set
from collections import defaultdict
from copy import deepcopy

from src.piece import Piece, Color, Type
from src.coordinate import Coordinate
from src.move import Move, MoveType
from util.chess_syntax import chess_to_coordinate


class Board:
    HEIGHT: int = 8
    WIDTH: int = 8

    board: List[List[Union[Piece, None]]]

    current_move: Color
    en_passant_target: Union[Coordinate, None]

    def __init__(
        self,
        fen_string: Union[
            str, None
        ] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
    ) -> None:
        """
        initialize the board object

        arguments:
        fen_string -- game specifications using Forsyth–Edwards Notation https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
        """

        # Build empty board
        self.board = [[]] * self.HEIGHT
        for row in range(self.HEIGHT):
            self.board[row] = [None] * self.WIDTH

        # Parse fen string
        board_specs: List = fen_string.split(" ")
        board_placement: List = board_specs[0].split("/")

        self.current_move = Color.WHITE if board_specs[1] == "w" else Color.BLACK
        self.en_passant_target = (
            chess_to_coordinate(board_specs[3]) if board_specs[3] != "-" else None
        )

        # Populate board from fen specification
        for board_rank in range(self.HEIGHT):
            board_file: int = 0
            i: int = 0

            while board_file < self.WIDTH:
                if board_placement[self.HEIGHT - 1 - board_rank][i].isnumeric():
                    board_file += int(board_placement[self.HEIGHT - 1 - board_rank][i])
                    i += 1
                    if board_file >= self.WIDTH:
                        break

                piece_char: str = board_placement[self.HEIGHT - 1 - board_rank][i]

                if piece_char == "P":
                    self.board[board_rank][board_file] = Piece(
                        Color.WHITE, Coordinate(board_rank, board_file), Type.PAWN
                    )
                elif piece_char == "N":
                    self.board[board_rank][board_file] = Piece(
                        Color.WHITE, Coordinate(board_rank, board_file), Type.KNIGHT
                    )
                elif piece_char == "B":
                    self.board[board_rank][board_file] = Piece(
                        Color.WHITE, Coordinate(board_rank, board_file), Type.BISHOP
                    )
                elif piece_char == "R":
                    self.board[board_rank][board_file] = Piece(
                        Color.WHITE, Coordinate(board_rank, board_file), Type.ROOK
                    )
                elif piece_char == "Q":
                    self.board[board_rank][board_file] = Piece(
                        Color.WHITE, Coordinate(board_rank, board_file), Type.QUEEN
                    )
                elif piece_char == "K":
                    self.board[board_rank][board_file] = Piece(
                        Color.WHITE, Coordinate(board_rank, board_file), Type.KING
                    )
                elif piece_char == "p":
                    self.board[board_rank][board_file] = Piece(
                        Color.BLACK, Coordinate(board_rank, board_file), Type.PAWN
                    )
                elif piece_char == "n":
                    self.board[board_rank][board_file] = Piece(
                        Color.BLACK, Coordinate(board_rank, board_file), Type.KNIGHT
                    )
                elif piece_char == "b":
                    self.board[board_rank][board_file] = Piece(
                        Color.BLACK, Coordinate(board_rank, board_file), Type.BISHOP
                    )
                elif piece_char == "r":
                    self.board[board_rank][board_file] = Piece(
                        Color.BLACK, Coordinate(board_rank, board_file), Type.ROOK
                    )
                elif piece_char == "q":
                    self.board[board_rank][board_file] = Piece(
                        Color.BLACK, Coordinate(board_rank, board_file), Type.QUEEN
                    )
                elif piece_char == "k":
                    self.board[board_rank][board_file] = Piece(
                        Color.BLACK, Coordinate(board_rank, board_file), Type.KING
                    )

                i += 1
                board_file += 1

        # Set castling eligibility
        rook: Piece
        rook = self.piece_at(Coordinate(0, 0), self.board)
        if rook and rook.piece_type == Type.ROOK and not "Q" in board_specs[2]:
            rook.moved = True
        rook = self.piece_at(Coordinate(0, self.WIDTH - 1), self.board)
        if rook and rook.piece_type == Type.ROOK and not "K" in board_specs[2]:
            rook.moved = True
        rook = self.piece_at(Coordinate(self.HEIGHT - 1, 0), self.board)
        if rook and rook.piece_type == Type.ROOK and not "q" in board_specs[2]:
            rook.moved = True
        rook = self.piece_at(Coordinate(self.HEIGHT - 1, self.WIDTH - 1), self.board)
        if rook and rook.piece_type == Type.ROOK and not "k" in board_specs[2]:
            rook.moved = True

    def __str__(self) -> None:
        """
        converts the board object into string format
        """

        board_str: str = "-------------------------\n"
        for board_rank in range(self.HEIGHT - 1, -1, -1):
            board_str += "|"
            for board_file in range(self.WIDTH):
                tile: Union[Piece, None] = self.board[board_rank][board_file]

                if tile == None:
                    board_str += "  "
                else:
                    if tile.color == Color.WHITE:
                        board_str += "w"
                    else:
                        board_str += "b"
                    if tile.piece_type == Type.PAWN:
                        board_str += "p"
                    elif tile.piece_type == Type.KNIGHT:
                        board_str += "n"
                    elif tile.piece_type == Type.BISHOP:
                        board_str += "b"
                    elif tile.piece_type == Type.ROOK:
                        board_str += "r"
                    elif tile.piece_type == Type.QUEEN:
                        board_str += "Q"
                    elif tile.piece_type == Type.KING:
                        board_str += "K"
                board_str += "|"
            board_str += "\n-------------------------\n"
        return board_str

    def in_board(self, coordinate: Coordinate) -> bool:
        """
        returns whether a coordinate is within the bounds of the board

        arguments:
        coordinate -- coordinate to check
        """

        return (
            coordinate.get_rank() >= 0
            and coordinate.get_file() >= 0
            and coordinate.get_rank() < self.HEIGHT
            and coordinate.get_file() < self.WIDTH
        )

    def piece_at(
        self, coordinate: Coordinate, board: List[List[Piece]]
    ) -> Union[Piece, None]:
        """
        returns a reference to the piece at a specified coordinate, if no piece is present returns None

        arguments:
        coordinate -- coordinate to check
        board -- board to check for piece in
        """

        if not self.in_board(coordinate):
            return None
        else:
            return board[coordinate.get_rank()][coordinate.get_file()]

    def get_pawn_moves(self, piece: Piece, board: List[List[Piece]]) -> List[Move]:
        """
        returns a list of all pawn moves

        arguments:
        piece -- piece to generate moves
        board -- board to validate moves
        """

        if piece.piece_type != Type.PAWN:
            return []

        direction: int = 1 if piece.color == Color.WHITE else -1
        moves: List[Move] = []

        # Add one square move
        destination: Coordinate = Coordinate(
            piece.coordinate.get_rank() + 1 * direction, piece.coordinate.get_file()
        )
        if self.in_board(destination) and not self.piece_at(destination, board):
            if (
                piece.color == Color.WHITE and destination.get_rank() == self.HEIGHT - 1
            ) or (piece.color == Color.BLACK and destination.get_rank() == 0):
                moves.append(
                    Move(piece.coordinate, destination, False, MoveType.N_PROMO)
                )
                moves.append(
                    Move(piece.coordinate, destination, False, MoveType.B_PROMO)
                )
                moves.append(
                    Move(piece.coordinate, destination, False, MoveType.R_PROMO)
                )
                moves.append(
                    Move(piece.coordinate, destination, False, MoveType.Q_PROMO)
                )
            else:
                moves.append(Move(piece.coordinate, destination))

            # Add two square move
            destination = Coordinate(
                piece.coordinate.get_rank() + 2 * direction, piece.coordinate.get_file()
            )
            if (
                not piece.moved
                and self.in_board(destination)
                and not self.piece_at(destination, board)
                and (
                    (piece.coordinate.get_rank() == 1 and piece.color == Color.WHITE)
                    or (
                        piece.coordinate.get_rank() == self.HEIGHT - 2
                        and piece.color == Color.BLACK
                    )
                )
            ):
                moves.append(
                    Move(piece.coordinate, destination, False, MoveType.DOUBLE)
                )

        # Add capturing moves (default and en passant)
        for offset in [-1, 1]:
            destination = Coordinate(
                piece.coordinate.get_rank() + 1 * direction,
                piece.coordinate.get_file() + offset,
            )
            # Add en passant moves
            if (
                self.in_board(destination)
                and self.en_passant_target
                and destination == self.en_passant_target
            ):
                moves.append(
                    Move(piece.coordinate, destination, False, MoveType.EN_PASSANT)
                )
            # Add default capturing moves
            elif self.in_board(destination) and self.piece_at(destination, board):
                if (
                    piece.color == Color.WHITE
                    and destination.get_rank() == self.HEIGHT - 1
                ) or (piece.color == Color.BLACK and destination.get_rank() == 0):
                    moves.append(
                        Move(piece.coordinate, destination, True, MoveType.N_PROMO)
                    )
                    moves.append(
                        Move(piece.coordinate, destination, True, MoveType.B_PROMO)
                    )
                    moves.append(
                        Move(piece.coordinate, destination, True, MoveType.R_PROMO)
                    )
                    moves.append(
                        Move(piece.coordinate, destination, True, MoveType.Q_PROMO)
                    )
                else:
                    moves.append(Move(piece.coordinate, destination, True))

        return moves

    def get_knight_moves(self, piece: Piece, board: List[List[Piece]]) -> List[Move]:
        """
        returns a list of all knight moves

        arguments:
        piece -- piece to generate moves
        board -- board to validate moves
        """

        if piece.piece_type != Type.KNIGHT:
            return []

        offsets: List[Tuple[int, int]] = [
            (2, 1),
            (1, 2),
            (-1, 2),
            (-2, 1),
            (-2, -1),
            (-1, -2),
            (1, -2),
            (2, -1),
        ]
        moves: List[Move] = []

        for offset in offsets:
            destination: Coordinate = Coordinate(
                piece.coordinate.get_rank() + offset[0],
                piece.coordinate.get_file() + offset[1],
            )
            if self.in_board(destination):
                moves.append(
                    Move(
                        piece.coordinate, destination, self.piece_at(destination, board)
                    )
                )

        return moves

    def get_straight_moves(self, piece: Piece, board: List[List[Piece]]) -> List[Move]:
        """
        returns a list of all straight moves

        arguments:
        piece -- piece to generate moves
        board -- board to validate moves
        """

        if piece.piece_type != Type.ROOK and piece.piece_type != Type.QUEEN:
            return []

        directions: List[Tuple[int, int]] = [(0, 1), (-1, 0), (0, -1), (1, 0)]
        moves: List[Move] = []

        for direction in directions:
            destination: Coordinate = Coordinate(
                piece.coordinate.get_rank() + direction[0],
                piece.coordinate.get_file() + direction[1],
            )
            while self.in_board(destination):
                moves.append(
                    Move(
                        piece.coordinate, destination, self.piece_at(destination, board)
                    )
                )
                if self.piece_at(destination, board):
                    break
                destination = Coordinate(
                    destination.get_rank() + direction[0],
                    destination.get_file() + direction[1],
                )

        return moves

    def get_diagonal_moves(self, piece: Piece, board: List[List[Piece]]) -> List[Move]:
        """
        returns a list of all diagonal moves

        arguments:
        piece -- piece to generate moves
        board -- board to validate moves
        """

        if piece.piece_type != Type.BISHOP and piece.piece_type != Type.QUEEN:
            return []

        directions: List[Tuple[int, int]] = [(1, 1), (-1, 1), (-1, -1), (1, -1)]
        moves: List[Move] = []

        for direction in directions:
            destination: Coordinate = Coordinate(
                piece.coordinate.get_rank() + direction[0],
                piece.coordinate.get_file() + direction[1],
            )
            while self.in_board(destination):
                moves.append(
                    Move(
                        piece.coordinate, destination, self.piece_at(destination, board)
                    )
                )
                if self.piece_at(destination, board):
                    break
                destination = Coordinate(
                    destination.get_rank() + direction[0],
                    destination.get_file() + direction[1],
                )

        return moves

    def get_king_moves(
        self, piece: Piece, board: List[List[Piece]], check_moves: bool = False
    ) -> List[Move]:
        """
        returns a list of all king moves

        arguments:
        piece -- piece to generate moves
        board -- board to validate moves
        """

        if piece.piece_type != Type.KING:
            return []

        moves: List[Move] = []

        # Get default king moves
        for rank_offset in [-1, 0, 1]:
            for file_offset in [-1, 0, 1]:
                if rank_offset == 0 and file_offset == 0:
                    continue
                destination: Coordinate = Coordinate(
                    piece.coordinate.get_rank() + rank_offset,
                    piece.coordinate.get_file() + file_offset,
                )
                if self.in_board(destination):
                    moves.append(
                        Move(
                            piece.coordinate,
                            destination,
                            self.piece_at(destination, board),
                        )
                    )

        # Get castling moves
        if (
            not piece.moved
            and not check_moves
            and not self.in_check(piece.color, board)
        ):
            rook: Piece
            rook_rank = 0 if piece.color == Color.WHITE else self.HEIGHT - 1

            # Queen side rook
            rook = self.piece_at(Coordinate(rook_rank, 0), board)
            if rook and rook.piece_type == Type.ROOK and not rook.moved:
                if (
                    not self.piece_at(Coordinate(rook_rank, 1), board)
                    and not self.piece_at(Coordinate(rook_rank, 2), board)
                    and not self.piece_at(Coordinate(rook_rank, 3), board)
                ):
                    candidate_board: List[List[Piece]] = deepcopy(board)
                    candidate_board[rook_rank][2] = candidate_board[
                        piece.coordinate.get_rank()
                    ][piece.coordinate.get_file()]
                    candidate_board[piece.coordinate.get_rank()][
                        piece.coordinate.get_file()
                    ] = None
                    if not self.in_check(piece.color, candidate_board):
                        moves.append(
                            Move(
                                piece.coordinate,
                                Coordinate(rook_rank, 2),
                                False,
                                MoveType.CASTLE,
                            )
                        )

            # King side rook
            rook = self.piece_at(Coordinate(rook_rank, self.WIDTH - 1), board)
            if rook and rook.piece_type == Type.ROOK and not rook.moved:
                if not self.piece_at(
                    Coordinate(rook_rank, self.WIDTH - 2), board
                ) and not self.piece_at(Coordinate(rook_rank, self.WIDTH - 3), board):
                    candidate_board: List[List[Piece]] = deepcopy(board)
                    candidate_board[rook_rank][self.WIDTH - 2] = candidate_board[
                        piece.coordinate.get_rank()
                    ][piece.coordinate.get_file()]
                    candidate_board[piece.coordinate.get_rank()][
                        piece.coordinate.get_file()
                    ] = None
                    if not self.in_check(piece.color, candidate_board):
                        moves.append(
                            Move(
                                piece.coordinate,
                                Coordinate(rook_rank, self.WIDTH - 2),
                                False,
                                MoveType.CASTLE,
                            )
                        )

        return moves

    def get_candidate_moves(
        self,
        piece: Union[Piece, None],
        board: List[List[Piece]],
        check_moves: bool = False,
    ) -> List[Move]:
        """
        returns a list of all candidate moves of a piece

        arguments:
        piece -- piece to generate moves
        board -- board to validate moves
        """

        # If the piece does not exist, return an empty list
        if not piece:
            return []

        # Generate pawn moves using the get_pawn_moves function
        if piece.piece_type == Type.PAWN:
            return self.get_pawn_moves(piece, board)

        # Generate pawn moves using the get_knight_moves function
        elif piece.piece_type == Type.KNIGHT:
            return self.get_knight_moves(piece, board)

        # Generate pawn moves using the get_diagonal_moves function
        elif piece.piece_type == Type.BISHOP:
            return self.get_diagonal_moves(piece, board)

        # Generate pawn moves using the get_straight_moves function
        elif piece.piece_type == Type.ROOK:
            return self.get_straight_moves(piece, board)

        # Generate pawn moves using the get_straight_moves and get_diagonal_moves functions
        elif piece.piece_type == Type.QUEEN:
            return self.get_diagonal_moves(piece, board) + self.get_straight_moves(
                piece, board
            )

        # Generate pawn moves using the get_king_moves function
        elif piece.piece_type == Type.KING:
            return self.get_king_moves(piece, board, check_moves)

    def get_moves(self, piece: Piece, board: List[List[Piece]]) -> List[Move]:
        """
        returns a list of all valid moves of a piece

        arguments:
        piece -- piece to generate moves
        board -- board to validate moves
        """

        candidate_moves: List[Move] = self.get_candidate_moves(piece, board)
        moves: List[Move] = []

        for move in candidate_moves:
            # Remove all moves that go outside the board
            if not self.in_board(move.dest):
                continue

            # Remove all moves that capture own piece
            if (
                self.piece_at(move.dest, board)
                and self.piece_at(move.dest, board).color == piece.color
            ):
                continue

            # Remove all moves that put king in check
            candidate_board: List[List[Piece]] = deepcopy(board)
            self.move(move, candidate_board, deepcopy(self.en_passant_target))
            if self.in_check(piece.color, candidate_board):
                del candidate_board
                continue
            del candidate_board

            moves.append(move)

        return moves

    def get_check_moves(self, piece: Piece, board: List[List[Piece]]) -> List[Move]:
        """
        returns a list of all valid moves of a piece

        arguments:
        piece -- piece to generate moves
        board -- board to validate moves
        """

        candidate_moves: List[Move] = self.get_candidate_moves(piece, board, True)
        moves: List[Move] = []

        for move in candidate_moves:
            # Remove all moves that go outside the board
            if not self.in_board(move.dest):
                continue

            # Remove all moves that capture own piece
            if (
                self.piece_at(move.dest, board)
                and self.piece_at(move.dest, board).color == piece.color
            ):
                continue

            moves.append(move)

        return moves

    def in_check(self, color: Color, board: List[List[Piece]]) -> bool:
        """
        checks whether a side is currently in check on a board

        arguments:
        color -- color to check if in check
        board -- board to check on
        """

        pieces = (
            self.get_pieces(Color.WHITE, board)
            if color == Color.BLACK
            else self.get_pieces(Color.BLACK, board)
        )
        for piece in list(pieces):
            for move in self.get_check_moves(piece, board):
                piece_dest = self.piece_at(move.dest, board)
                if (
                    piece_dest
                    and piece_dest.piece_type == Type.KING
                    and piece_dest.color != piece.color
                ):
                    return True

        return False

    def checkmated(self, color: Color, board: List[List[Piece]]) -> bool:
        """
        checks whether a side is currently in checkmate on a board

        arguments:
        color -- color to check if in check
        board -- board to check on
        """

        if not self.in_check(color, board):
            return False

        for r in range(self.HEIGHT):
            for f in range(self.WIDTH):
                piece: Union[Piece, None] = self.piece_at(Coordinate(r, f), board)
                if piece and piece.color == color and len(self.get_moves(piece, board)):
                    return False

        return True

    def move(
        self, move: Move, board: List[List[Piece]], en_passant_target: Coordinate
    ) -> None:
        """
        makes specified move on given board

        arguments:
        move -- move to make
        board -- board to make move on
        """

        piece = self.piece_at(move.origin, board)

        # Remove captured pieces from piece sets
        if move.capture:
            board[move.dest.get_rank()][move.dest.get_file()] = None
        if move.move_type == MoveType.EN_PASSANT:
            if piece.color == Color.WHITE:
                board[move.origin.get_rank() - 1][move.origin.get_file()] = None
            elif piece.color == Color.BLACK:
                board[move.origin.get_rank() + 1][move.origin.get_file()] = None

        # Move piece on board
        board[move.dest.get_rank()][move.dest.get_file()] = piece
        board[move.origin.get_rank()][move.origin.get_file()] = None
        piece.coordinate = move.dest
        piece.moved = True

        # Castling moves
        if move.move_type == MoveType.CASTLE:
            rook: Piece
            direction = int(
                (move.dest.get_file() - move.origin.get_file())
                / abs((move.dest.get_file() - move.origin.get_file()))
            )
            if direction < 0 and piece.color == Color.WHITE:
                rook = self.piece_at(Coordinate(0, 0), board)
                board[0][3] = rook
                board[0][0] = None
                rook.coordinate = Coordinate(0, 3)
            elif direction > 0 and piece.color == Color.WHITE:
                rook = self.piece_at(Coordinate(0, self.WIDTH - 1), board)
                board[0][self.WIDTH - 3] = rook
                board[0][self.WIDTH - 1] = None
                rook.coordinate = Coordinate(0, self.WIDTH - 3)
            elif direction < 0 and piece.color == Color.BLACK:
                rook = self.piece_at(Coordinate(self.HEIGHT - 1, 0), board)
                board[self.HEIGHT - 1][3] = rook
                board[self.HEIGHT - 1][0] = None
                rook.coordinate = Coordinate(self.HEIGHT - 1, 3)
            elif direction > 0 and piece.color == Color.BLACK:
                rook = self.piece_at(Coordinate(self.HEIGHT - 1, self.WIDTH - 1), board)
                board[self.HEIGHT - 1][self.WIDTH - 3] = rook
                board[self.HEIGHT - 1][self.WIDTH - 1] = None
                rook.coordinate = Coordinate(self.HEIGHT - 1, self.WIDTH - 3)
            rook.moved = True

        # Pawn promotion moves
        if move.move_type == MoveType.N_PROMO:
            board[move.dest.get_rank()][move.dest.get_file()] = Piece(
                piece.color, move.dest, Type.KNIGHT, True
            )
        elif move.move_type == MoveType.B_PROMO:
            board[move.dest.get_rank()][move.dest.get_file()] = Piece(
                piece.color, move.dest, Type.BISHOP, True
            )
        elif move.move_type == MoveType.R_PROMO:
            board[move.dest.get_rank()][move.dest.get_file()] = Piece(
                piece.color, move.dest, Type.ROOK, True
            )
        elif move.move_type == MoveType.Q_PROMO:
            board[move.dest.get_rank()][move.dest.get_file()] = Piece(
                piece.color, move.dest, Type.QUEEN, True
            )

        # Update en-passant target
        if move.move_type == MoveType.DOUBLE:
            en_passant_target = (
                Coordinate(move.dest.get_rank() - 1, move.dest.get_file())
                if piece.color == Color.WHITE
                else Coordinate(move.dest.get_rank() + 1, move.dest.get_file())
            )
        else:
            en_passant_target = None

    def get_pieces(self, color: Color, board: List[List[str]]) -> Set[Piece]:
        """
        build set of pieces of specified color from board
        
        arguments:
        color -- color of pieces to choose
        board -- board to get pieces from
        """

        pieces: Set[Piece] = set()
        for board_rank in range(self.HEIGHT):
            for board_file in range(self.WIDTH):
                piece: Union[Piece, None] = self.board[board_rank][board_file]
                if piece and piece.color == color:
                    pieces.add(piece)
        return pieces
