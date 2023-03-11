#pragma once

#include <vector>
#include "common_enums.h"
#include "Piece.h"

class BoardManager {
  public: 
    BoardManager();

    BoardManager(std::string fen);

    using Board = std::vector<std::vector<Piece>>;

    bool isCheckmate();
    bool resultsInCheck(Move m);

    void reset();
    std::vector<Move> genThisPossible(Piece p);

    // try and move if true, the move took place
    MoveResult move(Move m);
    MoveType do_move(Move m, Board& b);

    Piece pieceAt(int x, int y);

    Board getBoard();

    const std::string historyAt(int index);

    const bool colorMatchesTurn(Color c);
    std::vector<Move> genAllPossibleOpposing(Color c);

    const uint32_t MoveCount() const {return _move_count;};
    const uint32_t HalfMoveCount() const {return _half_move_count;};

    Board fen_to_board(std::string fen);
    PieceType fen_to_type(char c);

  private:

    Board _board;
    uint32_t _move_count = 0;
    uint32_t _half_move_count = 0;

    std::vector<std::string> _history;

    std::string board_to_fen();
    std::string fen_to_state(std::string fen);

    void initBoard();
    bool _en_passant_enabled = false;
    Point _passant_target;
    bool _isWhiteTurn = true;

    // move generation and helpers
    std::vector<Move> GPM_Piece(Piece p);
    std::vector<Move> GAPM_Opposing(Color c);
    std::vector<Move> rookPossible(Piece p);
    std::vector<Move> bishopPossible(Piece p);

    bool containsPoint(int x, int y, std::vector<Move> possible);
    const bool isColorInCheck(Color c);
    Point getKing(Color c);

    bool validPoint(int x, int y) const {
      return (x >= 0 && x < 8) && (y >=0 && y < 8);
    }
};
