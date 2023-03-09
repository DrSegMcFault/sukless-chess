#pragma once

#include <vector>
#include "Piece.h"
#include "common_enums.h"

class ChessUtils {
  public:
    using Board = std::vector<std::vector<Piece>>;
    std::vector<Move> GPM_Piece(Piece p, const Board& b);
    std::vector<Move> GAPM_Opposing(Color c, const Board& b);
    std::vector<Move> rookPossible(Piece p, const Board& b);
    std::vector<Move> bishopPossible(Piece p, const Board& b);
    const bool isColorInCheck(Color c, const Board& b);
    Point getKing(Color c, const Board& b);
    void move(Move m, Board& b);
    bool containsPoint(int x, int y, std::vector<Move> possible);
    std::string board_to_fen(const Board& b);
    Board fen_to_board(std::string fen);
    PieceType fen_to_type(char c);

    constexpr bool validPoint(int x, int y) const {
      return (x >= 0 && x < 8) && (y >=0 && y < 8);
    }
};