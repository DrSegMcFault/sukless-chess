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

    void reset();
    std::vector<Move> genPossible(Piece p);

    bool resultsInCheck(Move m);

    // try and move if true, the move took place
    MoveResult move(Move m);

    Piece pieceAt(int x, int y);

    Board getBoard();

    const std::string historyAt(int index);

    const bool colorMatchesTurn(Color c);
    std::vector<Move> genPossibleOpposing(Color c);

    const uint32_t MoveCount() const {return _move_count;};
    const uint32_t HalfMoveCount() const {return _half_move_count;};

    Board fen_to_board(std::string fen);
    std::string board_to_fen();

    PieceType fen_to_type(char c);

    const bool isColorInCheck(Color c);
    bool containsPoint(int x, int y, std::vector<Move> possible);

  private:

    
    // dont need this anymore, leaving for now
    enum MoveType {
      NORMAL = 0,
      Q_SIDE_CASTLE = 1,
      K_SIDE_CASTLE = 2,

      // next move can be en passant
      ENABLE_PASSANT = 3,

      // a passant move was just made
      PERFORM_PASSANT = 4
    };

    Board _board;
    uint32_t _move_count = 0;
    uint32_t _half_move_count = 0;

    std::vector<std::string> _history;


    void fen_to_state(std::string fen);

    void initBoard();
    bool _en_passant_enabled = false;
    Point _passant_target = {-1, -1};
    bool _isWhiteTurn = true;

    // move generation and helpers
    std::vector<Move> GPM_Piece(Piece p);
    std::vector<Move> GAPM_Opposing(Color c);
    std::vector<Move> rookPossible(Piece p);
    std::vector<Move> bishopPossible(Piece p);

    MoveType do_move(Move m);

    Point getKing(Color c);
    Point fen_to_point(std::string fen);
    std::string point_to_fen(Point p);

    bool validPoint(int x, int y) const {
      return (x >= 0 && x < 8) && (y >=0 && y < 8);
    }
};
