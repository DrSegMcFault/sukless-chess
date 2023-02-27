#include "ChessAI.h"

/******************************************************************************
 *
 * Method: ChessAI::ChessAI()
 *
 *****************************************************************************/
Chess::ChessAI::ChessAI(Color to_control, Difficulty d, Game* game)
  : _controlling(to_control),
    _difficulty(d),
    _game(game)
{}

/******************************************************************************
 *
 * Method: ChessAI::move()
 *
 *****************************************************************************/
bool Chess::ChessAI::move( std::vector<Move> possible)
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
 * Method: ChessAI::weak_move()
 * - this is a random move from the list of possible moves 
 *****************************************************************************/
bool Chess::ChessAI::weak_move(std::vector<Move> possible)
{
  bool success = false;
   
  if (possible.size() == 1) {
    success = _game->move(possible.at(0), possible);
  } 
  else {
    auto move_idx = rand() % possible.size(); 
    int tries = 0;
    if (_game->colorMatchesTurn(_controlling)) {
      while (!success || tries > possible.size() * 2) {
        success = _game->move(possible.at(move_idx), possible);
        move_idx = rand() % possible.size();
        tries++;
      }
    }
  }

  return success;
}

/******************************************************************************
 *
 * Method: ChessAI::evaluate()
 * - returns a score for the given move 
 *****************************************************************************/
int Chess::ChessAI::evaluate(Move m)
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

        my_eval += val_capture;

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
        score += 20;
      }

      if (!isPieceImmune(m.to.x, m.to.y, local)) {
        score -= 300;
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
 * Method: ChessAI::weak_move()
 * - this is a random move from the list of possible moves 
 *****************************************************************************/
bool Chess::ChessAI::decent_move(std::vector<Move> possible)
{
  bool success = false;
  // one score per move, if same score,
  // the move doesnt matter 

  std::vector<Pair> scores;
  if (_game->colorMatchesTurn(_controlling)) {
    for (auto move : possible) {
      if (!_game->resultsInCheck(move)) {
        scores.push_back({evaluate(move) , move});
      }
    }
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

  if (best_score == 0) {
    success = _game->move(getRandMove(scores), possible);
  } else {
    success = _game->move(scores[best_idx].move, possible);
  }
  std::cout << "best score is " << best_score << "\n";

  return success;
}

int Chess::ChessAI::getPieceValue(Piece p)
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

bool Chess::ChessAI::isCapture(Move m) {
  auto piece_moving = _game->pieceAt(m.from.x, m.from.y);
  auto piece_dest = _game->pieceAt(m.to.x, m.to.y);

  return piece_dest && piece_moving.Color() != piece_dest.Color();
}

bool Chess::ChessAI::isPieceImmune(int x, int y, Piece (&b)[8][8])
{
  auto p = b[x][y];
  auto possible = _game->genAllPossibleOpposing(_controlling, b);
  if (_game->containsPoint(p.x, p.y, possible)) {
    return false;
  }
  return true;
}

Chess::Move Chess::ChessAI::getRandMove(std::vector<Chess::Pair> scores)
{
  if (scores.size() == 1) {
    return scores[0].move;
  } 
  else {
    auto move_idx = rand() % scores.size(); 
    return scores[move_idx].move;
  }
}