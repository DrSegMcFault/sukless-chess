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
    bool resultsInCheck(Move m);

    void reset();
    std::vector<Move> genThisPossible(Piece p);
    // try and move if true, the move took place
    bool move(Move m, std::vector<Move> possible_moves);
    bool isValidMove(int from_x,
                     int from_y,
                     int to_x, 
                     int to_y, 
                     std::vector<Point> possible_moves);

    Piece pieceAt(int x, int y);
    struct Board
    {
      Piece board[8][8];
    };

    Board getBoard();

    const bool colorMatchesTurn(Color c);
    std::vector<Move> genAllPossibleOpposing(Color c);

  private:

    void move(Move m, Piece (&b)[8][8]);
    std::vector<Move> genPossibleMvPiece(Piece p, Piece (&b)[8][8]);
    std::vector<Move> genAllPossibleOpposing(Color c, Piece (&b)[8][8]);
    std::vector<Move> rookPossible(Piece p, Piece (&b)[8][8]);
    std::vector<Move> bishopPossible(Piece p, Piece (&b)[8][8]);
    Point getKing(Color c, Piece (&b)[8][8]);
    void initBoard();
    bool containsPoint(int x, int y, std::vector<Move> possible);
    bool _isWhiteTurn = true; 

    constexpr bool validPoint(int x, int y) const {
      return (x >= 0 && x < 8) && (y >=0 && y < 8);
    }

    friend class ChessAI;
};

} // namespace chess
