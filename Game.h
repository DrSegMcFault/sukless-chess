#pragma once

#include <vector>
#include "common_enums.h"
#include "Piece.h"
#include "ChessUtils.h"

class Game : public ChessUtils {
  public: 
    Game();

    Piece _board[8][8];
    uint32_t _move_count = 0;
    bool isCheckmate();
    bool resultsInCheckmate(Move m);
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

    std::vector<Board> history;
    Board getBoard();

    const bool colorMatchesTurn(Color c);
    std::vector<Move> genAllPossibleOpposing(Color c);

  private:

    void initBoard();
    bool _isWhiteTurn = true;

    friend class ChessAI;
};
