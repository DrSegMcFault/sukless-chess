#include "common_enums.h"
#include "Game.h"

namespace Chess {
  class ChessAI {
    public:
      enum Difficulty {
        EASY = 0,
        MEDIUM = 1,
        HARD = 2,
        IMPOSSIBLE = 3
      };

      int my_eval;
      int other_eval;

      ChessAI(Color c, Difficulty d, Game* game);
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
} // namespace chess
