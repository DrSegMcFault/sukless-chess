#pragma once

#include <vector>
#include "common_enums.h"

#include "Piece.h"
#include "ChessUtils.h"

class Game : public ChessUtils {
  public: 
    Game();

    using Field = std::vector<std::vector<Piece>>;

    Field _board;
    uint32_t _move_count = 0;
    std::vector<Move> possible_moves;
    std::vector<std::string> history;

    bool isCheckmate();
    bool resultsInCheckmate(Move m);
    bool resultsInCheck(Move m);

    void reset();
    std::vector<Move> genThisPossible(Piece p);

    // try and move if true, the move took place
    bool move(Move m);

    Piece pieceAt(int x, int y);

    Field getBoard();

    const bool colorMatchesTurn(Color c);
    std::vector<Move> genAllPossibleOpposing(Color c);

  private:

    void initBoard();
    bool _isWhiteTurn = true;

    friend class ChessAI;
};
