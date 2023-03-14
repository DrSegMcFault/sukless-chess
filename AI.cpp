#include "AI.h"
#include <iostream>
#include "time.h"

/******************************************************************************
 *
 * Method: AI::AI()
 *
 *****************************************************************************/
AI::AI(Color to_control, Difficulty d, BoardManager* game)
  : _controlling(to_control),
    _difficulty(d),
    _game(game)
{}

/******************************************************************************
 *
 * Method: AI::move()
 *
 *****************************************************************************/
Move AI::move()
{
  auto possible =
    _game->genPossibleOpposing(_controlling == WHITE ? BLACK : WHITE);

  switch (_difficulty) {
    case EASY:
    case MEDIUM:
    case HARD:
    case IMPOSSIBLE:
      return decent_move(possible);
  }
}

/******************************************************************************
 * Method: AI::evaluate(Move m)
 *
 * returns a score for the given move 
 *****************************************************************************/
int AI::evaluate(Move m)
{
  switch (_difficulty) {
    case EASY:
    {
      break;
    }
    case MEDIUM:
    {
      // the 'best' move is to take a piece, put the other player
      // in check, and the piece cannot be taken after, OR Checkmate
      int score = 0;

      bool was_attacked = _game->containsPoint(m.from.x, m.from.y,
                                               _game->genPossibleOpposing(_controlling));
      auto piece_from = _game->pieceAt(m.from.x, m.from.y);
      
      // a copy of the game state, board included
      BoardManager game_cpy(_game->board_to_fen());

      // below this is the result of 1 move
      switch ( game_cpy.move(m) ) {
        case CHECKMATE:
          return 10000;
        case INVALID:
          return -10000;
        case DRAW:
          return 0;
        default:
          break;
      }

      auto other_color = _controlling == WHITE ? BLACK : WHITE;
      auto opponent_moves = game_cpy.genPossibleOpposing(_controlling);

      auto isPieceImmune = [&,&opponent_moves=opponent_moves](int x, int y) {
        return !game_cpy.containsPoint(x, y, opponent_moves);
      };

      if (isCapture(m)) {
        int val_capture = getPieceValue(_game->pieceAt(m.to.x, m.to.y));

        if (isPieceImmune(m.to.x, m.to.y)) {
          score += 300;
        }
         
        score += 5 + val_capture;
      }

      if (game_cpy.isColorInCheck(other_color))
      {
        // if check and immune, very valuable
        if (isPieceImmune(m.to.x, m.to.y)) {
          score += 50;
        }
        score += 50;
      }

      if (!isPieceImmune(m.to.x, m.to.y)) {
        score -= getPieceValue(_game->pieceAt(m.from.x, m.from.y));;
      }

      // If the piece was under attack and a retreating
      // square is available, the move is equal to the
      // pieces value
      if (was_attacked && isPieceImmune(m.to.x, m.to.y)){
        score += getPieceValue(piece_from);
      }

      // add positional value, if applicable
      if (piece_from.type == PAWN) {
        switch (piece_from.Color()) {
          case WHITE:
            score += pawn_white_p[m.to.x][m.to.y];
            break;
          case BLACK:
            score += pawn_black_p[m.to.x][m.to.y];
            break;
          default:
            break;
        }
      } else if (piece_from.type == KNIGHT) {
        score += knight_p[m.to.x][m.to.y];
      }

      return score;
      break;
    }
    case HARD:
    {
      break;
    }
    case IMPOSSIBLE:
    {
      break;
    }
    default:
      break;
  }

  return 0;
}

/******************************************************************************
 *
 * Method: AI::decent_move(vector<Move> possible)
 * - this is a random move from the list of possible moves 
 *****************************************************************************/
Move AI::decent_move(std::vector<Move> possible)
{
  Move move {0,0,0,0};
  std::vector<Pair> scores = {};
  if (_game->colorMatchesTurn(_controlling)) {
    for (auto move : possible) {
      if (!_game->resultsInCheck(move)) {
        scores.push_back({evaluate(move) , move});
      }
    }
  }

  if (scores.empty()) {
    std::cout << "ai has no moves\n";
    return move;
  }

  int best_score = 0;
  int best_idx = 0;
  int i = 0;
  for (auto [score, move] : scores) {
    if (score > best_score) {
      best_score = score;
      best_idx = i;
    }
    i++;
  }

  move = scores[best_idx].move;
  
  std::cout << "best score is " << best_score << "\n";

  return move;
}

/******************************************************************************
 *
 * Method: AI::getPieceValue(Piece p)
 * return the associated integer value for the peice
 *****************************************************************************/
int AI::getPieceValue(Piece p)
{
  switch (p.type) {
    case PAWN:
      return 100;
    case KNIGHT:
    case BISHOP:
      return 300;
    case ROOK:
      return 500;
    case QUEEN:
      return 900;
    default:
      return 0;
  }
}

/******************************************************************************
 *
 * Method: AI::isCapture(Move m)
 * was the move a capture
 *****************************************************************************/
bool AI::isCapture(Move m) {
  auto piece_moving = _game->pieceAt(m.from.x, m.from.y);
  auto piece_dest = _game->pieceAt(m.to.x, m.to.y);

  return piece_dest && piece_moving.Color() != piece_dest.Color();
}

/******************************************************************************
 * Method: AI::getRandMove(vector<Pair>)
 * 
 * Pair - Struct with a move and a score
 *****************************************************************************/
Move AI::getRandMove(const std::vector<Pair>& scores)
{
  if (scores.size() == 1) {
    return scores[0].move;
  } 
  else {
    srand(time(NULL));
    auto move_idx = rand() % scores.size(); 
    return scores[move_idx].move;
  }
}
