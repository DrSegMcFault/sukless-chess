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

      ChessAI(Color c, Difficulty d);
      Difficulty _difficulty;
      bool move(Game* game, std::vector<Move> possible);
      bool weak_move(Game* game, std::vector<Move> possible);
      bool decent_move(Game* game, std::vector<Move> possible);
      int evaluate(Move m);

    private:
      Color _controlling;
  };
} // namespace chess
