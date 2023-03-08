#pragma once

#include <vector>
#include "Piece.h"
#include "Board.h"
#include "common_enums.h"

class ChessUtils {
  public:
    std::vector<Move> GPM_Piece(Piece p, Piece (&b)[8][8]);
    std::vector<Move> GAPM_Opposing(Color c, Piece (&b)[8][8]);
    std::vector<Move> rookPossible(Piece p, Piece (&b)[8][8]);
    std::vector<Move> bishopPossible(Piece p, Piece (&b)[8][8]);
    const bool isColorInCheck(Color c, Piece (&b)[8][8]);
    Point getKing(Color c, Piece (&b)[8][8]);
    void move(Move m, Piece (&b)[8][8]);
    bool containsPoint(int x, int y, std::vector<Move> possible);
    std::string board_to_fen(Board b);
    Board fen_to_board(std::string fen);
    PieceType fen_to_type(char c);

    constexpr bool validPoint(int x, int y) const {
      return (x >= 0 && x < 8) && (y >=0 && y < 8);
    }
};