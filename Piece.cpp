#include "Piece.h"
#include <iostream>

/******************************************************************************
 *
 * Method: Piece::isValidMove()
 * - is the pathing allowed for the piece, the board state is unknown
 *****************************************************************************/      
bool Chess::Piece::isValidMove(int new_x, int new_y)
{
  if (new_x == x && new_y == y){
    return false;
  }
  
  auto dx = abs(new_x - x);
  auto dy = abs(new_y - y);

  switch (type) {
    case PAWN:
    { 
      if (color == WHITE) {
        if (has_moved) {
          return (new_x == x - 1 && (new_y == y || dy == 1) );
        } else {
          return ( (new_x == x - 1 || new_x == x - 2) && (new_y == y || dy == 1) );
        }
      } else if (color == BLACK) {
        if (has_moved) {
          return (new_x == x + 1 && (new_y == y || dy == 1) );
        } else {
          return ( (new_x == x + 1 || new_x == x + 2) && (new_y == y || dy == 1) );
        }
      }
    } 
    case ROOK:
    {
      if ((new_x != x && new_y == y) || (new_x == x && new_y != y)) {
        return true;
      } else {
        return false;
      }
    }   
    case BISHOP:
    {
      return dx == dy;
    } 
    case QUEEN:
    {
      // moves like rook OR bishop
      return dx == dy || ((new_x != x && new_y == y) || (new_x == x && new_y != y)); 
    }
    case KING:
    {
      // moves like queen but only one square 
      bool flag = dx == dy;

      return flag ? dx == 1 : (dx == 1 || dy == 1) && 
                              ((new_x != x && new_y == y) ||
                              (new_x == x && new_y != y));
    }
    case KNIGHT:
    {
      if ((dx == 2) && (dy == 1)){
        return true;
      } else if ((dy == 2) && (dx == 1)){
        return true;
      } else if ((dx == 1) && new_y == y - 2) {
        return true;
      } else {
        return false;
      }
    }
    case NONE:
      return false;
  }
 return true;
}

/******************************************************************************
 *
 * Method: Piece::typeToString()
 *
 *****************************************************************************/
std::string Chess::Piece::typeToString()
{
  switch (type) {
    case PAWN:
      return "pawn";
    case ROOK:
      return "rook";
    case BISHOP:
      return "bishop";
    case QUEEN:
      return "queen";
    case KING:
      return "king";
    case KNIGHT: 
      return "knight";
    case NONE:
      return " ";
  }
}

/******************************************************************************
 *
 * Method: Piece::colorToString()
 *
 *****************************************************************************/
std::string Chess::Piece::colorToString()
{
  switch (color) {
    case WHITE:
      return "white";
    case BLACK:
      return "black";
    default: return "none";
  }
}

/******************************************************************************
 *
 * Method: Piece::printPiece()
 *
 *****************************************************************************/
void Chess::Piece::printPiece()
{
  std::cout << typeToString() << " " << colorToString() << "   ";
}
