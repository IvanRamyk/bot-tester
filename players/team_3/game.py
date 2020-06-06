import numpy as np
import random
from typing import Tuple


FREE_CELL = 0
WALL = 1
FIRST_PLAYER = 2
SECOND_PLAYER = 3


def is_player(value: int) -> bool:
    return value == FIRST_PLAYER or value == SECOND_PLAYER


class Game:
    def __init__(self, size: int = 9, fp_pos: Tuple[int, int] = (1, 5), sp_pos: Tuple[int, int] = (9, 5)) -> None:
        self._board = np.full((2 * size - 1, 2 * size - 1), FREE_CELL, dtype=np.uint8)
        self._size = size
        self._fp_pos = fp_pos
        self._sp_pos = sp_pos
        self._fp_walls = 10
        self._sp_walls = 10
        self._fp_move_right = None
        self._sp_move_right = None
        self.set_cell(*fp_pos, FIRST_PLAYER)
        self.set_cell(*sp_pos, SECOND_PLAYER)

    @property
    def size(self) -> int:
        return self._size

    def get_cell(self, x: int, y: int) -> int:
        return self._board[2 * y - 2][2 * x - 2]

    def set_cell(self, x: int, y: int, value: int) -> None:
        self._board[2 * y - 2][2 * x - 2] = value

    def __repr__(self) -> str:
        return repr(np.flip(np.transpose(self._board), 0))

    def move_player(self, player: int, x: int, y: int) -> None:
        if player == FIRST_PLAYER:
            cur_x, cur_y = self._fp_pos

            self.set_cell(cur_x, cur_y, FREE_CELL)
            self.set_cell(x, y, FIRST_PLAYER)
            self._fp_pos = (x, y)
        else:
            cur_x, cur_y = self._sp_pos

            self.set_cell(cur_x, cur_y, FREE_CELL)
            self.set_cell(x, y, SECOND_PLAYER)
            self._sp_pos = (x, y)

    def is_move_possible(self, cur_x: int, cur_y: int, x: int, y: int) -> bool:
        if x < 1 or y < 1 or x > self.size or y > self.size:
            return False

        if not self.get_cell(x, y) == FREE_CELL:
            return False

        dif_x, dif_y = abs(x - cur_x), abs(y - cur_y)
        up, right = cur_x < x, cur_y < y

        if (dif_x == 1 and dif_y == 0) or (dif_x == 0 and dif_y == 1):
            real_avg_x, real_avg_y = x + cur_x - 2, y + cur_y - 2
            return not self._board[real_avg_y][real_avg_x] == WALL

        if dif_x == 1 and dif_y == 1:
            if up and right:
                check_wall_x1, check_wall_y1 = cur_x * 2 - 1, cur_y * 2 - 2
                check_wall_x2, check_wall_y2 = cur_x * 2, cur_y * 2 - 1
                if not (is_player(self.get_cell(cur_x + 1, y)) or is_player(self.get_cell(cur_x, y + 1))):
                    return False
                return not (self._board[check_wall_y1][check_wall_x1] == WALL or
                            self._board[check_wall_y2][check_wall_x2] == WALL)

            if up and not right:
                check_wall_x1, check_wall_y1 = cur_x * 2 - 1, cur_y * 2 - 2
                check_wall_x2, check_wall_y2 = cur_x * 2, cur_y * 2 - 3
                if not (is_player(self.get_cell(cur_x + 1, y)) or is_player(self.get_cell(cur_x, y - 1))):
                    return False
                return not (self._board[check_wall_y1][check_wall_x1] == WALL or
                            self._board[check_wall_y2][check_wall_x2] == WALL)

            if right:
                check_wall_x1, check_wall_y1 = cur_x * 2 - 3, cur_y * 2 - 2
                check_wall_x2, check_wall_y2 = cur_x * 2 - 4, cur_y * 2 - 1
                if not (is_player(self.get_cell(cur_x - 1, y)) or is_player(self.get_cell(cur_x, y + 1))):
                    return False
                return not (self._board[check_wall_y1][check_wall_x1] == WALL or
                            self._board[check_wall_y2][check_wall_x2] == WALL)

            check_wall_x1, check_wall_y1 = cur_x * 2 - 3, cur_y * 2 - 2
            check_wall_x2, check_wall_y2 = cur_x * 2 - 4, cur_y * 2 - 3
            if not (is_player(self.get_cell(cur_x - 1, y)) or is_player(self.get_cell(cur_x, y + 1))):
                return False
            return not (self._board[check_wall_y1][check_wall_x1] == WALL or
                        self._board[check_wall_y2][check_wall_x2] == WALL)

        if dif_x == 2 and dif_y == 0:
            check_wall_y = 2 * cur_y - 2
            if up:
                check_wall_x1, check_wall_x2 = 2 * cur_x - 1, 2 * cur_x + 1
                if not is_player(self.get_cell(cur_x + 1, y)):
                    return False
                return not (self._board[check_wall_y][check_wall_x1] == WALL or
                            self._board[check_wall_y][check_wall_x2] == WALL)

            check_wall_x1, check_wall_x2 = 2 * cur_x - 3, 2 * cur_x - 5
            if not is_player(self.get_cell(cur_x - 1, y)):
                return False
            return not (self._board[check_wall_y][check_wall_x1] == WALL or
                        self._board[check_wall_y][check_wall_x2] == WALL)

        if dif_y == 2 and dif_x == 0:
            check_wall_x = 2 * cur_x - 2
            if right:
                check_wall_y1, check_wall_y2 = 2 * cur_y - 1, 2 * cur_y + 1,
                if not is_player(self.get_cell(cur_x + 1, y)):
                    return False
                return not (self._board[check_wall_y1][check_wall_x] == WALL or
                            self._board[check_wall_y2][check_wall_x] == WALL)

            check_wall_y1, check_wall_y2 = 2 * cur_y - 3, 2 * cur_y - 5
            if not is_player(self.get_cell(cur_x - 1, y)):
                return False
            return not (self._board[check_wall_y1][check_wall_x] == WALL or
                        self._board[check_wall_y2][check_wall_x] == WALL)

        return False

    def build_wall(self, player: int, x1: int, y1: int, x2: int, y2: int) -> None:
        if player == FIRST_PLAYER:
            self._fp_walls -= 1
        else:
            self._sp_walls -= 1

        if x1 > x2:
            x1, x2 = x2, x1
        if y1 > y2:
            y1, y2 = y2, y1

        if x1 == x2:
            self._board[2 * y1 - 2][2 * x1 - 3] = WALL
            self._board[2 * y1 - 1][2 * x1 - 3] = WALL
            self._board[2 * y1][2 * x1 - 3] = WALL
        else:
            self._board[2 * y1 - 3][2 * x1 - 2] = WALL
            self._board[2 * y1 - 3][2 * x1 - 1] = WALL
            self._board[2 * y1 - 3][2 * x1] = WALL

    def is_wall_possible(self, player: int, x1: int, y1: int, x2: int, y2: int) -> bool:
        if (player == FIRST_PLAYER and self._fp_walls == 0) or (player == SECOND_PLAYER and self._sp_walls == 0):
            return False

        if x1 > x2:
            x1, x2 = x2, x1
        if y1 > y2:
            y1, y2 = y2, y1

        if x1 == x2 and abs(y1 - y2) == 2:
            real_x, real_y1, real_y2 = 2 * x1 - 3, 2 * y1 - 2, 2 * y2 - 2
            avg_y = y1 + y2 - 2
            if (self._board[real_y1][real_x] == WALL
                    or self._board[real_y1 + 1][real_x] == WALL
                    or self._board[real_y1 + 2][real_x] == WALL):
                return False
            return True

        if y1 == y2 and abs(x1 - x2) == 2:
            real_x1, real_x2, real_y = 2 * x1 - 2, 2 * x2 - 2, 2 * y1 - 3
            if (self._board[real_y][real_x1] == WALL
                    or self._board[real_y][real_x1 + 1] == WALL
                    or self._board[real_y][real_x1 + 2] == WALL):
                return False
            return True

        return False

    def make_move(self, player: int, x: int, y: int) -> None:
        self.move_player(player, x, y)
        print(f'move {x} {y}')

    def make_wall(self, player: int, x1: int, y1: int, x2: int, y2: int):
        self.build_wall(player, x1, y1, x2, y2)
        print(f'partition  {x1} {y1} {x2} {y2}')

    def rush_mode(self, player: int) -> None:
        if player == FIRST_PLAYER:
            self._rush_first_player()
        else:
            self._rush_second_player()

    def _rush_first_player(self) -> None:
        cur_x, cur_y = self._fp_pos
        if self.is_move_possible(cur_x, cur_y, cur_x + 2, cur_y):
            self.make_move(FIRST_PLAYER, cur_x + 2, cur_y)
        elif self.is_move_possible(cur_x, cur_y, cur_x + 1, cur_y):
            self.make_move(FIRST_PLAYER, cur_x + 1, cur_y)
        elif self.is_move_possible(cur_x, cur_y, cur_x + 1, cur_y + 1):
            self.make_move(FIRST_PLAYER, cur_x + 1, cur_y + 1)
        elif self.is_move_possible(cur_x, cur_y, cur_x + 1, cur_y - 1):
            self.make_move(FIRST_PLAYER, cur_x + 1, cur_y - 1)
        elif self.is_move_possible(cur_x, cur_y, cur_x, cur_y + 1):
            self.make_move(FIRST_PLAYER, cur_x, cur_y + 1)
        elif self.is_move_possible(cur_x, cur_y, cur_x, cur_y - 1):
            self.make_move(FIRST_PLAYER, cur_x, cur_y - 1)
        elif self.is_move_possible(cur_x, cur_y, cur_x, cur_y + 2):
            self.make_move(FIRST_PLAYER, cur_x, cur_y + 2)
        elif self.is_move_possible(cur_x, cur_y, cur_x, cur_y - 2):
            self.make_move(FIRST_PLAYER, cur_x, cur_y - 2)
        elif self.is_move_possible(cur_x, cur_y, cur_x - 1, cur_y):
            self.make_move(FIRST_PLAYER, cur_x - 1, cur_y)
        elif self.is_move_possible(cur_x, cur_y, cur_x - 1, cur_y + 1):
            self.make_move(FIRST_PLAYER, cur_x - 1, cur_y + 1)
        elif self.is_move_possible(cur_x, cur_y, cur_x - 1, cur_y - 1):
            self.make_move(FIRST_PLAYER, cur_x - 1, cur_y - 1)
        else:
            self.make_move(FIRST_PLAYER, cur_x - 2, cur_y)

    def _rush_second_player(self) -> None:
        cur_x, cur_y = self._sp_pos
        if self.is_move_possible(cur_x, cur_y, cur_x - 2, cur_y):
            self.make_move(SECOND_PLAYER, cur_x - 2, cur_y)
        elif self.is_move_possible(cur_x, cur_y, cur_x - 1, cur_y):
            self.make_move(SECOND_PLAYER, cur_x - 1, cur_y)
        elif self.is_move_possible(cur_x, cur_y, cur_x - 1, cur_y - 1):
            self.make_move(SECOND_PLAYER, cur_x - 1, cur_y - 1)
        elif self.is_move_possible(cur_x, cur_y, cur_x - 1, cur_y + 1):
            self.make_move(SECOND_PLAYER, cur_x - 1, cur_y + 1)
        elif self.is_move_possible(cur_x, cur_y, cur_x, cur_y - 1):
            self.make_move(SECOND_PLAYER, cur_x, cur_y - 1)
        elif self.is_move_possible(cur_x, cur_y, cur_x, cur_y + 1):
            self.make_move(SECOND_PLAYER, cur_x, cur_y + 1)
        elif self.is_move_possible(cur_x, cur_y, cur_x, cur_y - 2):
            self.make_move(SECOND_PLAYER, cur_x, cur_y - 2)
        elif self.is_move_possible(cur_x, cur_y, cur_x, cur_y + 2):
            self.make_move(SECOND_PLAYER, cur_x, cur_y + 2)
        elif self.is_move_possible(cur_x, cur_y, cur_x + 1, cur_y):
            self.make_move(SECOND_PLAYER, cur_x + 1, cur_y)
        elif self.is_move_possible(cur_x, cur_y, cur_x + 1, cur_y - 1):
            self.make_move(SECOND_PLAYER, cur_x + 1, cur_y - 1)
        elif self.is_move_possible(cur_x, cur_y, cur_x + 1, cur_y + 1):
            self.make_move(SECOND_PLAYER, cur_x + 1, cur_y + 1)
        else:
            self.make_move(SECOND_PLAYER, cur_x + 2, cur_y)

    def build_quoridor_mode(self, player: int) -> None:
        if player == FIRST_PLAYER:
            self._build_quoridor_first_player()
        else:
            self._build_quoridor_second_player()

    def _build_quoridor_first_player(self) -> None:
        cur_x, cur_y = self._fp_pos
        op_x, op_y = self._sp_pos

        if op_x <= 5 and self.is_move_possible(op_x, op_y, op_x - 1, op_y):
            if op_y <= 7 and self.is_wall_possible(FIRST_PLAYER, op_x, op_y, op_x, op_y + 2):
                self.make_wall(FIRST_PLAYER, op_x, op_y, op_x, op_y + 2)
            elif op_y > 1 and self.is_wall_possible(FIRST_PLAYER, op_x, op_y - 1, op_x, op_y + 1):
                self.make_wall(FIRST_PLAYER, op_x, op_y - 1, op_x, op_y + 1)
            else:
                self._rush_first_player()

            return

        if cur_y != 1 and cur_y != 9:
            dif_lt, dif_rt = abs(cur_y - 1), abs(9 - cur_y)

            if dif_lt >= dif_rt and self.is_move_possible(cur_x, cur_y, cur_x, cur_y + 1):
                self.make_move(FIRST_PLAYER, cur_x, cur_y + 1)
            else:
                self._rush_first_player()

            return

        for i in range(1, 8):
            if cur_y == 1 and self.is_wall_possible(FIRST_PLAYER, i, 2, i + 2, 2):
                self.make_wall(FIRST_PLAYER, i, 2, i + 2, 2)
                return
            elif self.is_wall_possible(FIRST_PLAYER, i, 9, i + 2, 9):
                self.make_wall(FIRST_PLAYER, i, 9, i + 2, 9)
                return

        self._rush_first_player()

    def _build_quoridor_second_player(self) -> None:
        cur_x, cur_y = self._sp_pos
        op_x, op_y = self._fp_pos

        if op_x >= 5 and self.is_move_possible(op_x, op_y, op_x + 1, op_y):
            if op_y <= 7 and self.is_wall_possible(SECOND_PLAYER, op_x + 1, op_y, op_x + 1, op_y + 2):
                self.make_wall(SECOND_PLAYER, op_x + 1, op_y, op_x + 1, op_y + 2)
            elif op_y > 1 and self.is_wall_possible(SECOND_PLAYER, op_x + 1, op_y - 1, op_x + 1, op_y + 1):
                self.make_wall(SECOND_PLAYER, op_x + 1, op_y - 1, op_x + 1, op_y + 1)
            else:
                self._rush_second_player()

            return

        if cur_y != 1 and cur_y != 9:
            dif_lt, dif_rt = abs(cur_y - 1), abs(9 - cur_y)

            if dif_lt >= dif_rt and self.is_move_possible(cur_x, cur_y, cur_x, cur_y + 1):
                self.make_move(SECOND_PLAYER, cur_x, cur_y + 1)
            else:
                self._rush_second_player()

            return

        for i in range(9, 2):
            if cur_y == 1 and self.is_wall_possible(SECOND_PLAYER, i - 2, 2, i, 2):
                self.make_wall(SECOND_PLAYER, i - 2, 2, i, 2)
                return
            elif self.is_wall_possible(SECOND_PLAYER, i - 2, 9, i, 9):
                self.make_wall(SECOND_PLAYER, i - 2, 9, i, 9)
                return

        self._rush_second_player()
