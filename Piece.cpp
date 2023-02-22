#include "Piece.h"
#include <iostream>

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
