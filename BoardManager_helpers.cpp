#include "BoardManager.h"

/******************************************************************************
 *
 * Method: BoarManager::GPM_Piece(Piece p, const _board&)
 * - generate the possible moves for a given piece and _board 
 *****************************************************************************/
std::vector<Move> BoardManager::GPM_Piece(Piece p)
{
  Point start = {p.x, p.y};

  std::vector<Move> possible;
  auto mod = p.color == Color::WHITE ? 1 : -1;

  // determine if move is possible for knight and king
  auto knightKing = [&] (auto x, auto y) {
    if (validPoint(x,y)) {
      if (!_board[x][y] || (_board[x][y] && _board[x][y].Color() != p.Color())) {
        possible.push_back(Move{start, Point{x, y}});
      }
    }
  };

  switch (p.type) {
    case PAWN:
    {
      // if there isnt a piece in front of the pawn, its possible
      if (validPoint(p.x -mod, p.y) && !_board[p.x - mod][p.y]) {
        possible.push_back(Move{start, Point{p.x - mod, p.y}});
      }

      if (validPoint(p.x-mod, p.y-mod) &&
          _board[p.x - mod][p.y - mod] && p.color != _board[p.x-mod][p.y-mod].color) {
        possible.push_back(Move {start, Point{p.x - mod, p.y - mod} });
      }

      if (validPoint(p.x-mod, p.y + mod) && 
          _board[p.x - mod][p.y + mod] && p.Color() != _board[p.x-mod][p.y+mod].Color()) {
        possible.push_back(Move {start,Point{p.x - mod, p.y + mod}});
      }


     if (!p.has_moved &&
       validPoint(p.x - mod, p.y) &&
       !_board[p.x - mod][p.y] &&
       validPoint(p.x - mod*2, p.y) &&
       !_board[p.x - mod*2][p.y])
     {
       possible.push_back(Move {start,Point{p.x - mod*2, p.y} });
     }

     // en passant
     if (_en_passant_enabled) {
      auto m = p.color == Color::WHITE ? -1 : 1;
      if (validPoint(p.x, p.y -1) &&
          _board[p.x][p.y - 1].type == PAWN &&
          _board[p.x][p.y - 1].color != p.color) 
      {
        possible.push_back(Move {start, Point {p.x - m, p.y - 1} });

      } else if (validPoint(p.x, p.y + 1) &&
                 _board[p.x][p.y + 1].type == PAWN &&
                 _board[p.x][p.y + 1].color != p.color) 
      {
        possible.push_back(Move {start, Point {p.x - m, p.y + 1} });
      }
     }

     break;
   }

   case ROOK:
   { 
      auto rook = rookPossible(p);
      possible.insert(std::end(possible), std::begin(rook), std::end(rook));
      break;
   }

   case KING:
   {
     {
       auto x = p.x - 1;
       auto y = p.y - 1;
       knightKing(x,y);
     }

     {
       auto x = p.x;
       auto y = p.y - 1;
       knightKing(x,y);
     }

     {
       auto x = p.x + 1;
       auto y = p.y - 1;
       knightKing(x,y);
     }

     {
       auto x = p.x + 1;
       auto y = p.y;
       knightKing(x,y);
     }

     {
       auto x = p.x - 1;
       auto y = p.y;
       knightKing(x,y);
     }

     {
       auto x = p.x - 1;
       auto y = p.y + 1;
       knightKing(x,y);
     }

     {
       auto x = p.x;
       auto y = p.y + 1;
       knightKing(x,y);
     }

     {
       auto x = p.x + 1;
       auto y = p.y + 1;
       knightKing(x,y);
     }

     {
       // castling
       // if the king hasnt moved
       if (!p.has_moved) {
         // if im not already in check
         if (validPoint(p.x, p.y + 3) &&
             _board[p.x][p.y + 3] &&
             _board[p.x][p.y + 3].type == ROOK &&
             !_board[p.x][p.y + 3].has_moved)
         {
           // if traveling to either of the next to squares dont result in check
           if (!_board[p.x][p.y + 1]) {
             if (!_board[p.x][p.y + 2]) {
               possible.push_back(Move{start, Point {p.x, p.y + 2} });
             }
           }
         }

         // queen side castle
         // if im not already in check
         if (validPoint(p.x, p.y - 4) &&
             _board[p.x][p.y - 4] &&
             _board[p.x][p.y - 4].type == ROOK &&
             !_board[p.x][p.y - 4].has_moved)
         {
           // if traveling to either of the next to squares dont result in check
           if (!_board[p.x][p.y - 1]) {
             if (!_board[p.x][p.y - 2]) {
               if (!_board[p.x][p.y - 3]) {
                 possible.push_back(Move{start, Point {p.x, p.y - 2}});
               }
             }
           }
         }
       }
     }
     break;
   }   
   case KNIGHT:
   {
     {
       auto x = p.x - 2;
       auto y = p.y + 1;
       knightKing(x,y);
     }
 
     {
       auto x = p.x - 2;
       auto y = p.y - 1;
       knightKing(x,y);
     }

     {
       auto x = p.x - 1;
       auto y = p.y - 2;
       knightKing(x,y);
     }

     {
       auto x = p.x + 1;
       auto y = p.y - 2;
       knightKing(x,y);
     }

     {
       auto x = p.x + 2;
       auto y = p.y - 1;
       knightKing(x,y);
     }

     {
       auto x = p.x + 2;
       auto y = p.y + 1;
       knightKing(x,y);
     }

     {
       auto x = p.x + 1;
       auto y = p.y + 2;
       knightKing(x,y);
     }

     {
       auto x = p.x - 1;
       auto y = p.y + 2;
       knightKing(x,y);
     }
     break;
   }

   case BISHOP:
   {
     auto b = bishopPossible(p);
     possible.insert(std::end(possible), std::begin(b), std::end(b)); 
     break;
   }

   case QUEEN:
   {
     auto b = bishopPossible(p);
     possible.insert(std::end(possible), std::begin(b), std::end(b)); 
     auto c = rookPossible(p);
     possible.insert(std::end(possible), std::begin(c), std::end(c)); 
     break;
   }

   default:
     break;
  }
  return possible;
}

/******************************************************************************
 *
 * Method: BoarManager::GAPM_Opposing(Color, const _board&)
 * 
 * - generate all possible moves for the opposing color
 *****************************************************************************/
std::vector<Move> BoardManager::GAPM_Opposing(Color c)
{
  std::vector<Move> possible;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j ++) {
      if (_board[i][j] && _board[i][j].Color() != c) {
        auto piecePossible = GPM_Piece(_board[i][j]);
        possible.insert(std::end(possible),
                        std::begin(piecePossible),
                        std::end(piecePossible));
      }
    }
  }
  return possible; 
}

/******************************************************************************
 *
 * Method: BoarManager::rookPossible(Piece, const _board&)
 *
 *****************************************************************************/
std::vector<Move> BoardManager::rookPossible(Piece p)
{
  std::vector<Move> possible;
  Point start = {p.x, p.y};
  {
    auto x = p.x + 1;
    auto y = p.y;
    while (validPoint(x, y)) {
      if (!_board[x][y]) {
        possible.push_back(Move {start,Point{x, y}});
        x++;
      } else if (_board[x][y].Color() != p.Color()) {
        possible.push_back(Move {start,Point{x, y}});
        break;
      } else {
        break;
      }
    }
  }

  {
    auto x = p.x - 1;
    auto y = p.y;
    while (validPoint(x, y)) {
      if (!_board[x][y]) {
        possible.push_back(Move {start,Point{x, y}});
        x--;
      } else if (_board[x][y].Color() != p.Color()) {
        possible.push_back(Move {start,Point{x, y}});
        break;
      } else {
        break;
      }
    }
  }

  {
    auto x = p.x;
    auto y = p.y + 1;
    while (validPoint(x, y)) {
      if (!_board[x][y]) {
        possible.push_back(Move {start,Point{x, y}});
        y++;
      } else if (_board[x][y].Color() != p.Color()) {
        possible.push_back(Move {start,Point{x, y}});
        break;
      } else {
        break;
      }
    }
  }

  {
    auto x = p.x;
    auto y = p.y - 1;
    while (validPoint(x, y)) {
      if (!_board[x][y]) {
        possible.push_back(Move {start,Point{x, y}});
        y--;
      } else if (_board[x][y].Color() != p.Color()) {
        possible.push_back(Move {start,Point{x, y}});
        break; 
      } else {
        break; 
      }
    }
  }
  return possible;
}

/******************************************************************************
 *
 * Method: BoarManager::bishopPossible(Piece, const _board&)
 *
 *****************************************************************************/
std::vector<Move> BoardManager::bishopPossible(Piece p)
{
  std::vector<Move> possible;
  Point start = {p.x, p.y};
  {
    auto x = p.x + 1;
    auto y = p.y + 1;
    while (validPoint(x, y)) {
      if (!_board[x][y]) {
        possible.push_back(Move {start,Point{x, y}});
        x++;
        y++;
      } else if (_board[x][y].Color() != p.Color()) {
        possible.push_back(Move {start,Point{x, y}});
        break;
      } else {
        break;
      }
    }
  }

  {
    auto x = p.x - 1;
    auto y = p.y - 1;
    while (validPoint(x, y)) {
      if (!_board[x][y]) {
        possible.push_back(Move {start,Point{x, y}});
        x--;
        y--;
      } else if (_board[x][y].Color() != p.Color()) {
        possible.push_back(Move {start,Point{x, y}});
        break;
      } else {
        break;
      }
    }
  }

  {
    auto x = p.x - 1;
    auto y = p.y + 1;
    while (validPoint(x, y)) {
      if (!_board[x][y]) {
        possible.push_back(Move {start,Point{x, y}});
        y++;
        x--;
      } else if (_board[x][y].Color() != p.Color()) {
        possible.push_back(Move {start,Point{x, y}});
        break;
      } else {
        break; 
      }
    }
  }

  {
    auto x = p.x + 1;
    auto y = p.y - 1;
    while (validPoint(x, y)) {
      if (!_board[x][y]) {
        possible.push_back(Move {start,Point{x, y}});
        y--;
        x++;
      } else if (_board[x][y].Color() != p.Color()) {
        possible.push_back(Move {start,Point{x, y}});
        break;
      } else {
        break;
      }
    }
  }
  return possible;
}

/******************************************************************************
 *
 * Method: BoarManager::getKing(Color, const _board&)
 *
 *****************************************************************************/
Point BoardManager::getKing(Color c)
{
    for (int i = 0; i < 8; i ++) {
      for (int j = 0; j < 8; j++) {
        if (_board[i][j].type == KING && _board[i][j].Color() == c) {
          return Point { _board[i][j].x, _board[i][j].y };
        }
      }
    }

    return Point {-1, -1};
}

/******************************************************************************
 *
 * Method: BoarManager::isColorInCheck(Color, const _board&)
 * 
 *****************************************************************************/
const bool BoardManager::isColorInCheck(Color c)
{
  auto otherColor = c == WHITE ? BLACK : WHITE;
  auto possible = GAPM_Opposing(c);

  auto king = getKing(c);

  return containsPoint(king.x, king.y, possible);
}

/******************************************************************************
 *
 * Method: BoarManager::containsPoint(x, y, possible)
 *
 *****************************************************************************/
bool BoardManager::containsPoint(int x, int y, std::vector<Move> possible)
{
  if (!validPoint(x,y)) {
    return false;
  }

  for (auto move : possible) {
    if (move.to.x == x && move.to.y == y) {
      return true;
    }
  }
  return false;
}
