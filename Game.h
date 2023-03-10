#pragma once

#include <vector>
#include "common_enums.h"

#include "Piece.h"
#include "ChessUtils.h"

class Game : public ChessUtils {
  public: 
    Game();

    using Board = std::vector<std::vector<Piece>>;


    bool isCheckmate();
    bool resultsInCheck(Move m);

    void reset();
    std::vector<Move> genThisPossible(Piece p);

    // try and move if true, the move took place
    MoveResult move(Move m);

    Piece pieceAt(int x, int y);

    Board getBoard();

    const std::string historyAt(int index);

    const bool colorMatchesTurn(Color c);
    std::vector<Move> genAllPossibleOpposing(Color c);

    const uint32_t MoveCount() const {return _move_count;};

  private:

    Board _board;
    uint32_t _move_count = 0;
    std::vector<std::string> _history;

    void initBoard();

    bool _isWhiteTurn = true;
};
