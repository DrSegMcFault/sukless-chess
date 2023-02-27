#include "Piece.h"

/******************************************************************************
 *
 * Method: Piece::typeToString()
 *
 *****************************************************************************/
std::string Piece::typeToString()
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
    default:
      return " ";
  }
}

/******************************************************************************
 *
 * Method: Piece::colorToString()
 *
 *****************************************************************************/
std::string Piece::colorToString()
{
  switch (color) {
    case WHITE:
      return "white";
    case BLACK:
      return "black";
    default: return "none";
  }
}
