#include "common_enums.h"
#include "Game.h"

class AI {
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

    int my_eval;
    int other_eval;

    AI(Color c, Difficulty d, Game* game);
    Difficulty _difficulty;
    Game* _game;

    bool move(std::vector<Move> possible);
    bool weak_move(std::vector<Move> possible);
    bool decent_move(std::vector<Move> possible);
    bool isCapture(Move m);
    bool isPieceImmune(int x, int y, Piece (&b)[8][8]);
    int evaluate(Move m);
    int getPieceValue(Piece p);
    Move getRandMove(std::vector<Pair> pairs);

  private:
    Color _controlling;
};
