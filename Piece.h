#pragma once

#include <string>
#include "common_enums.h"

class Piece {
  private:
    std::string prefixPath = "resources/";

  public:
    std::string icon = "";
    Color color = C_NONE;
    PieceType type = NONE;
    bool has_moved = false;

    int x = 0;
    int y = 0;
    int prev_x = 0;
    int prev_y = 0;

    Piece() = default;

    Piece(int x, int y)
      : x(x),
        y(y)
    {}

    Piece(int new_x, int new_y, PieceType type)
      : x(new_x),
        y(new_y),
        prev_x(x),
        prev_y(y),
        type(type)
    {}

    Piece(int new_x, int new_y, PieceType type, Color color)
      : x(new_x),
        y(new_y),
        prev_x(x),
        prev_y(y),
        type(type),
        color(color)
    {
      icon = prefixPath + typeToString() + "_" + colorToString() + ".png";
    }
      
    explicit operator bool() const  {
      return type != NONE;
    }

    const auto Icon() { return icon.c_str(); };

    const Color Color() { return color; };
      
    void Clear() { type = NONE; color = C_NONE; x = -1; y = -1;}

    std::string typeToString();

    std::string colorToString();
};
