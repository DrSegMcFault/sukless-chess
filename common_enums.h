#pragma once

namespace Chess {

  // turn
  enum Move {
    MOVE_WHITE = 0,
    MOVE_BLACK = 1
  };
  
  // Piece color
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
  struct Point {
    int x = 0;
    int y = 0;
  };
} // namespace chess
