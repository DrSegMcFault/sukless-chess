#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "common_enums.h"

namespace Chess {

  class Piece {
    private:
      std::string prefixPath = "../resources/";

    public:
      std::string icon = "";
      Chess::Color color = C_NONE;
      PieceType type = NONE;
      bool has_moved = false;

      int x = 0;
      int y = 0;

      Piece() = default;

      Piece(int x, int y)
        : x(x),
          y(y)
      {}

      Piece(int new_x, int new_y, PieceType type)
        : x(new_x),
          y(new_y),
          type(type)
      {}

      Piece(int new_x, int new_y, PieceType type, Color color)
        : x(new_x),
          y(new_y),
          type(type),
          color(color)
      {
        icon = prefixPath + typeToString() + "_" + colorToString() + ".png";
      }
      
      explicit operator bool() const  {
        return type != NONE;
      }

      constexpr auto Icon() { return icon.c_str(); };

      constexpr Chess::Color Color() { return color; };
      
      constexpr void Clear() { type = NONE; color = C_NONE; x = -1; y = -1;}

      void printPiece();

      std::string typeToString();

      std::string colorToString();
  };

} //namespace chess
