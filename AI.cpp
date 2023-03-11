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
    _game->genAllPossibleOpposing(_controlling == WHITE ? BLACK : WHITE);

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
      // in check, and the piece cannot be taken after
      int score = 0;

      Board local(_game->getBoard());

      bool was_attacked = !isPieceImmune(m.from.x, m.from.y, local);
      auto piece_from = _game->pieceAt(m.from.x, m.from.y);

      // below this is the result of 1 move
      _game->do_move(m);

      auto other_color = _controlling == WHITE ? BLACK : WHITE;

      if (isCapture(m)) {
        int val_capture = getPieceValue(_game->pieceAt(m.to.x, m.to.y));

        if (isPieceImmune(m.to.x, m.to.y, local)) {
          score += 300;
        }
         
        score += 5 + val_capture;
      }

      if (_game->isColorInCheck(other_color))
      {
        if (_game->isCheckmate()) {
          return 10000;
        }
        // if check and immune, very valuable
        if (isPieceImmune(m.to.x, m.to.y, local)) {
          score += 1000;
        }
        score += 50;
      }

      if (!isPieceImmune(m.to.x, m.to.y, local)) {
        score -= getPieceValue(_game->pieceAt(m.from.x, m.from.y));;
      }

      // If the piece was under attack and a retreating
      // square is available, the move is equal to the
      // pieces value
      if (was_attacked && isPieceImmune(m.to.x, m.to.y, local)){
        score += getPieceValue(piece_from);
      }

      if (isPieceImmune(m.to.x, m.to.y, local)) {
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

  if (best_score <= 0) {
    move = getRandMove(scores);
  } else {
    move = scores[best_idx].move;
  }
  
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
    case QUEEN:
      return 900;
    case ROOK:
      return 500;
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
 * Method: AI::isPieceImmmune(x, y, const Board&)
 *
 * can the piece be taken
 *****************************************************************************/
bool AI::isPieceImmune(int x, int y, const Board& b)
{
  auto p = b[x][y];
  auto possible = GAPM_Opposing(_controlling, b);
  if (_game->containsPoint(p.x, p.y, possible)) {
    return false;
  }
  return true;
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
