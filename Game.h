#pragma once

#include <vector>
#include "common_enums.h"
#include "Piece.h"

class Game {
  public: 
    Game();

    uint32_t _move_count = 0;

    bool isCheckmate();
    bool isResultsInCheckmate(Move m);
    bool resultsInCheck(Move m);

    void reset();
    std::vector<Move> genThisPossible(Piece p);
    // try and move if true, the move took place
    bool move(Move m, std::vector<Move> possible_moves);

    Piece pieceAt(int x, int y);
    struct Board
    {
      Piece board[8][8];
    };

    Board getBoard();
    std::vector<Board> history;

    const bool colorMatchesTurn(Color c);
    std::vector<Move> genAllPossibleOpposing(Color c);

  private:
    bool _isWhiteTurn = true; 
    Piece _board[8][8];

    void move(Move m, Piece (&b)[8][8]);

    const bool isColorInCheck(Color c, Piece (&b)[8][8]);
    std::vector<Move> genPossibleMvPiece(Piece p, Piece (&b)[8][8]);
    std::vector<Move> genAllPossibleOpposing(Color c, Piece (&b)[8][8]);
    std::vector<Move> rookPossible(Piece p, Piece (&b)[8][8]);
    std::vector<Move> bishopPossible(Piece p, Piece (&b)[8][8]);
    Point getKing(Color c, Piece (&b)[8][8]);
    void initBoard();
    bool containsPoint(int x, int y, std::vector<Move> possible);

    constexpr bool validPoint(int x, int y) const {
      return (x >= 0 && x < 8) && (y >=0 && y < 8);
    }

    friend class AI;
};
