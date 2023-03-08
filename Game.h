#pragma once

#include <vector>
#include "common_enums.h"

#include "Piece.h"
#include "Board.h"
#include "ChessUtils.h"

class Game : public ChessUtils {
  public: 
    Game();

    Piece _board[8][8];
    uint32_t _move_count = 0;
    bool isCheckmate();
    bool resultsInCheckmate(Move m);
    bool resultsInCheck(Move m);
    std::vector<Move> possible_moves;

    void reset();
    std::vector<Move> genThisPossible(Piece p);

    // try and move if true, the move took place
    bool move(Move m);

    Piece pieceAt(int x, int y);

    std::vector<std::string> history;
    Board getBoard();

    const bool colorMatchesTurn(Color c);
    std::vector<Move> genAllPossibleOpposing(Color c);

  private:

    void initBoard();
    bool _isWhiteTurn = true;

    friend class ChessAI;
};
