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

std::string Piece::typeToFEN() const 
{
  switch (type) {
    case PAWN:
      return color == WHITE ? "P" : "p";
    case ROOK:
      return color == WHITE ? "R" : "r";
    case BISHOP:
      return color == WHITE ? "B" : "b";
    case QUEEN: 
      return color == WHITE ? "Q" : "q";
    case KING:
      return color == WHITE ? "K" : "k";
    case KNIGHT:
      return color == WHITE ? "N" : "n"; 
    case NONE:
      return "x";
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
