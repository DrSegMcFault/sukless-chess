#pragma once

#include <vector>
#include <map>
#include "Piece.h"

namespace Chess {

class Game {
  public: 
    Game();

    Piece _board[8][8];
    uint8_t _move_count = 0;

    bool isCheckmate();
    bool resultsInCheck(int from_x, int from_y, int to_x, int to_y);

    void reset();
    std::vector<Point> genThisPossible(Piece p);
    // try and move if true, the move took place
    bool move(int from_x, int from_y, int to_x, int to_y, std::vector<Point> possible_moves);
    bool isValidMove(int from_x,
                     int from_y,
                     int to_x, 
                     int to_y, 
                     std::vector<Point> possible_moves);

    Piece pieceAt(int x, int y);

    const bool colorMatchesTurn(Color c);

  private:

    void move(int from_x, int from_y, int to_x, int to_y, Piece (&b)[8][8]);
    std::vector<Point> genPossibleMvPiece(Piece p, Piece (&b)[8][8]);
    std::vector<Point> genAllPossibleOpposing(Color c, Piece (&b)[8][8]);
    std::vector<Point> rookPossible(Piece p, Piece (&b)[8][8]);
    std::vector<Point> bishopPossible(Piece p, Piece (&b)[8][8]);
    Point getKing(Color c, Piece (&b)[8][8]);
    void initBoard();
    bool containsPoint(int x, int y, std::vector<Point> possible);
    bool _isWhiteTurn = true; 

    constexpr bool validPoint(int x, int y) const {
      return (x >= 0 && x < 8) && (y >=0 && y < 8);
    }
};

} // namespace chess
