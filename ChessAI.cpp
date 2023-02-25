#include "ChessAI.h"

/******************************************************************************
 *
 * Method: ChessAI::ChessAI()
 *
 *****************************************************************************/
Chess::ChessAI::ChessAI(Color to_control, Difficulty d)
  : _controlling(to_control),
    _difficulty(d)
{}

/******************************************************************************
 *
 * Method: ChessAI::move()
 *
 *****************************************************************************/
bool Chess::ChessAI::move(Game* game, std::vector<Move> possible)
{
  switch (_difficulty) {
    case MEDIUM:
    case HARD:
    case IMPOSSIBLE:
      return weak_move(game, possible);
    default:
      return weak_move(game, possible);
  }
}

/******************************************************************************
 *
 * Method: ChessAI::weak_move()
 * - this is a random move from the list of possible moves 
 *****************************************************************************/
bool Chess::ChessAI::weak_move(Game* game, std::vector<Move> possible)
{
  bool success = false;
   
  if (possible.size() == 1) {

    success = game->move(possible.at(0), possible);
  } else {
    auto move_idx = rand() % possible.size(); 
    int tries = 0;
    if (game->colorMatchesTurn(_controlling)) {
      while (!success || tries > possible.size() * 2) {
        success = game->move(possible.at(move_idx), possible);
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

  // auto p = game->pieceAt(1,1); 
  // Piece local[8][8];
  // std::copy(&game->_board[0][0], &game->_board[0][0]+8*8, &local[0][0]);
  // auto moves = game->genPossibleMvPiece(p, local);
  return 0;
}

/******************************************************************************
 *
 * Method: ChessAI::weak_move()
 * - this is a random move from the list of possible moves 
 *****************************************************************************/
bool Chess::ChessAI::decent_move(Game* game, std::vector<Move> possible)
{
  bool success = false;
   
  if (possible.size() == 1) {

    success = game->move(possible.at(0), possible);
  } else {
    auto move_idx = rand() % possible.size(); 
    int tries = 0;
    if (game->colorMatchesTurn(_controlling)) {
      while (!success || tries > possible.size() * 2) {
        success = game->move(possible.at(move_idx), possible);
        move_idx = rand() % possible.size();
        tries++;
      }
    }
  }

  return success;
}