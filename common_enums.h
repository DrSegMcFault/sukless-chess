#pragma once

enum Color {
  WHITE = 0,
  BLACK = 1,
  C_NONE = 2
};

enum PieceType {
  NONE = 0,
  PAWN = 1,
  KNIGHT = 2,
  BISHOP = 3,
  ROOK = 4,
  QUEEN = 5,
  KING = 6
};

enum MoveResult {
  VALID = 0,
  INVALID = 1,
  CHECKMATE = 2,
  STALEMATE = 3,
  DRAW = 4
};

struct Point {
  int x = 0;
  int y = 0;
};

struct Move {
  Point from;
  Point to;
};

