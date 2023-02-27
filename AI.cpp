#include "AI.h"

/******************************************************************************
 *
 * Method: AI::AI()
 *
 *****************************************************************************/
AI::AI(Color to_control, Difficulty d, Game* game)
  : _controlling(to_control),
    _difficulty(d),
    _game(game)
{}

/******************************************************************************
 *
 * Method: AI::move()
 *
 *****************************************************************************/
bool AI::move( std::vector<Move> possible)
{
  switch (_difficulty) {
    case MEDIUM:
      return decent_move(possible);
    case HARD:
    case IMPOSSIBLE:
      return weak_move(possible);
    default:
      return weak_move(possible);
  }
}

/******************************************************************************
 *
 * Method: AI::weak_move()
 * - this is a random move from the list of possible moves 
 *****************************************************************************/
bool AI::weak_move(std::vector<Move> possible)
{
  bool success = false;
  if (possible.size() == 0) {
    return false;
  }
  if (possible.size() == 1) {
    success = _game->move(possible.at(0), possible);
  } 
  else {
    auto move_idx = rand() % possible.size(); 
    int tries = 0;
      while (!success || tries < possible.size() * 2) {
        success = _game->move(possible.at(move_idx), possible);
        move_idx = rand() % possible.size();
        tries++;
      }
  }

  return success;
}

/******************************************************************************
 *
 * Method: AI::evaluate()
 * - returns a score for the given move 
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

      Piece local[8][8];
      std::copy(&_game->_board[0][0], &_game->_board[0][0]+8*8, &local[0][0]);
      _game->move(m, local);

      auto other_color = _controlling == WHITE ? BLACK : WHITE;

      if (isCapture(m)) {
        int val_capture = getPieceValue(_game->pieceAt(m.to.x, m.to.y));

        if (isPieceImmune(m.to.x, m.to.y, local)) {
          score += 300;
        }
         
        score += 5 + val_capture;
      }
      if (_game->isColorInCheck(other_color, local) )
      {
        // if check and immune, very valuable
        if (isPieceImmune(m.to.x, m.to.y, local)) {
          score += 1000;
        }
        score += 50;
      }

      if (!isPieceImmune(m.to.x, m.to.y, local)) {
        score -= getPieceValue(_game->pieceAt(m.from.x, m.from.y));;
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
 * Method: AI::weak_move()
 * - this is a random move from the list of possible moves 
 *****************************************************************************/
bool AI::decent_move(std::vector<Move> possible)
{
  bool success = false;

  std::vector<Pair> scores = {};
  if (_game->colorMatchesTurn(_controlling)) {
    for (auto move : possible) {
      if (!_game->resultsInCheck(move)) {
        scores.push_back({evaluate(move) , move});
      }
    }
  }

  if (scores.empty()) {
    return false;
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
    int tries = 0;
    while (!success || tries > scores.size() *2) {
      success = _game->move(getRandMove(scores), possible);
      tries++;
    }
  } else {
    success = _game->move(scores[best_idx].move, possible);
  }
  //std::cout << "best score is " << best_score << "\n";

  return success;
}

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

bool AI::isCapture(Move m) {
  auto piece_moving = _game->pieceAt(m.from.x, m.from.y);
  auto piece_dest = _game->pieceAt(m.to.x, m.to.y);

  return piece_dest && piece_moving.Color() != piece_dest.Color();
}

bool AI::isPieceImmune(int x, int y, Piece (&b)[8][8])
{
  auto p = b[x][y];
  auto possible = _game->genAllPossibleOpposing(_controlling, b);
  if (_game->containsPoint(p.x, p.y, possible)) {
    return false;
  }
  return true;
}

Move AI::getRandMove(std::vector<Pair> scores)
{
  if (scores.size() == 1) {
    return scores[0].move;
  } 
  else {
    auto move_idx = rand() % scores.size(); 
    return scores[move_idx].move;
  }
}