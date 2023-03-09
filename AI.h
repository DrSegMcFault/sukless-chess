#include "common_enums.h"
#include "Game.h"
#include "ChessUtils.h"

class AI : public ChessUtils {
  public:
  
    enum Difficulty {
      EASY = 0,
      MEDIUM = 1,
      HARD = 2,
      IMPOSSIBLE = 3
    };

    struct Pair {
      int score;
      Move move;
    };

    AI(Color c, Difficulty d, Game* game);
    Difficulty _difficulty;
    Game* _game;

    Move move();
    bool weak_move(std::vector<Move> possible);
    Move decent_move(std::vector<Move> possible);
    bool isCapture(Move m);
    bool isPieceImmune(int x, int y, const Board& b);
    bool isAttacking(Move m);
    bool isCheckmate(Board b);
    int evaluate(Move m);
    int getPieceValue(Piece p);
    bool isMoveCheck(Move m, Board b);
    Move getRandMove(const std::vector<Pair>& pairs);

  private:
    Color _controlling;

    const int pawn_white_p[8][8] = {
      {900, 900, 900, 900, 900, 900, 900, 900},
      {50, 50, 50, 50, 50, 50, 50, 50},
      {10, 10, 20, 30, 30, 20, 10, 10},
      {5,  5, 10, 25, 25, 10,  5,  5},
      {0,  0,  0, 20, 20,  0,  0,  0},
      {5, -5,-10,  0,  0,-10, -5,  5},
      {5, 10, 10,-20,-20, 10, 10,  5},
      {0,  0,  0,  0,  0,  0,  0,  0}
    };

    const int pawn_black_p[8][8] = {
      {0,  0,  0,  0,  0,  0,  0,  0},
      {5, 10, 10,-20,-20, 10, 10,  5},
      {5, -5,-10,  0,  0,-10, -5,  5},
      {0,  0,  0, 20, 20,  0,  0,  0},
      {5,  5, 10, 25, 25, 10,  5,  5},
      {10, 10, 20, 30, 30, 20, 10, 10},
      {50, 50, 50, 50, 50, 50, 50, 50},
      {900, 900, 900, 900, 900, 900, 900, 900}
    };

    const int knight_p[8][8] = {
      {-50,-40,-30,-30,-30,-30,-40,-50},
      {-40,-20,  0,  0,  0,  0,-20,-40},
      {-30,  0, 10, 15, 15, 10,  0,-30},
      {-30,  5, 15, 20, 20, 15,  5,-30},
      {-30,  0, 15, 20, 20, 15,  0,-30},
      {-30,  5, 10, 15, 15, 10,  5,-30},
      {-40,-20,  0,  5,  5,  0,-20,-40},
      {-50,-40,-30,-30,-30,-30,-40,-50}
    };

    const int bishop_white_p[8][8] = {
      {-20,-10,-10,-10,-10,-10,-10,-20},
      {-10,  0,  0,  0,  0,  0,  0,-10},
      {-10,  0,  5, 10, 10,  5,  0,-10},
      {-10,  5,  5, 10, 10,  5,  5,-10},
      {-10,  0, 10, 10, 10, 10,  0,-10},
      {-10, 10, 10, 10, 10, 10, 10,-10},
      {-10,  5,  0,  0,  0,  0,  5,-10},
      {-20,-10,-10,-10,-10,-10,-10,-20}
    };
};
