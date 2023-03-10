#include "ChessUtils.h"
#include <iostream>

/******************************************************************************
 *
 * Method: ChessUtils::GPM_Piece(Piece p, const Board&)
 * - generate the possible moves for a given piece and board 
 *****************************************************************************/
std::vector<Move> ChessUtils::GPM_Piece(Piece p, const Board& board)
{
  Point start = {p.x, p.y};

  std::vector<Move> possible;
  auto mod = p.color == Color::WHITE ? 1 : -1;

  // determine if move is possible for knight and king
  auto knightKing = [&] (auto x, auto y) {
    if (validPoint(x,y)) {
      if (!board[x][y] || (board[x][y] && board[x][y].Color() != p.Color())) {
        possible.push_back(Move{start, Point{x, y}});
      }
    }
  };

  switch (p.type) {
    case PAWN:
    {
      // if there isnt a piece in front of the pawn, its possible
      if (validPoint(p.x -mod, p.y) && !board[p.x - mod][p.y]) {
        possible.push_back(Move{start, Point{p.x - mod, p.y}});
      }

      if (validPoint(p.x-mod, p.y-mod) &&
          board[p.x - mod][p.y - mod] && p.color != board[p.x-mod][p.y-mod].color) {
        possible.push_back(Move {start, Point{p.x - mod, p.y - mod} });
      }

      if (validPoint(p.x-mod, p.y + mod) && 
          board[p.x - mod][p.y + mod] && p.Color() != board[p.x-mod][p.y+mod].Color()) {
        possible.push_back(Move {start,Point{p.x - mod, p.y + mod}});
      }


     if (!p.has_moved &&
       !board[p.x - mod][p.y] &&
       !board[p.x - mod*2][p.y])
     {
       possible.push_back(Move {start,Point{p.x - mod*2, p.y} });
     }

     // en passant
    //  if (en_pass_enabled) {
    //    if (board[p.x][p.y - 1] && board[p.x][p.y - 1].type == PAWN
    //        && board[p.x][p.y - 1].color != p.color) 
    //   {
    //     possible.push_back(Move {start, Point {p.x - mod, p.y - 1} });
    //   } else if (board[p.x][p.y + 1] && board[p.x][p.y + 1].type == PAWN
    //        && board[p.x][p.y + 1].color != p.color) 
    //   {
    //     possible.push_back(Move {start, Point {p.x - mod, p.y + 1} });
    //   }
    //  }
     break;
   }

   case ROOK:
   { 
      auto rook = rookPossible(p, board);
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
         if (board[p.x][p.y + 3] &&
             board[p.x][p.y + 3].type == ROOK &&
             !board[p.x][p.y + 3].has_moved)
         {
           // if traveling to either of the next to squares dont result in check
           if (!board[p.x][p.y + 1]) {
             if (!board[p.x][p.y + 2]) {
               possible.push_back(Move{start, Point {p.x, p.y + 2} });
             }
           }
         }

         // queen side castle
         // if im not already in check
         if (board[p.x][p.y - 4] &&
             board[p.x][p.y - 4].type == ROOK &&
             !board[p.x][p.y - 4].has_moved)
         {
           // if traveling to either of the next to squares dont result in check
           if (!board[p.x][p.y - 1]) {
             if (!board[p.x][p.y - 2]) {
               if (!board[p.x][p.y - 3]) {
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
     auto b = bishopPossible(p, board);
     possible.insert(std::end(possible), std::begin(b), std::end(b)); 
     break;
   }

   case QUEEN:
   {
     auto b = bishopPossible(p, board);
     possible.insert(std::end(possible), std::begin(b), std::end(b)); 
     auto c = rookPossible(p, board);
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
 * Method: ChessUtils::GAPM_Opposing(Color, const Board&)
 * 
 * - generate all possible moves for the opposing color
 *****************************************************************************/
std::vector<Move> ChessUtils::GAPM_Opposing(Color c, const Board& b)
{
  std::vector<Move> possible;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j ++) {
      if (b[i][j] && b[i][j].Color() != c) {
        auto piecePossible = GPM_Piece(b[i][j], b);
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
 * Method: ChessUtils::rookPossible(Piece, const Board&)
 *
 *****************************************************************************/
std::vector<Move> ChessUtils::rookPossible(Piece p, const Board& board )
{
  std::vector<Move> possible;
  Point start = {p.x, p.y};
  {
    auto x = p.x + 1;
    auto y = p.y;
    while (validPoint(x, y)) {
      if (!board[x][y]) {
        possible.push_back(Move {start,Point{x, y}});
        x++;
      } else if (board[x][y].Color() != p.Color()) {
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
      if (!board[x][y]) {
        possible.push_back(Move {start,Point{x, y}});
        x--;
      } else if (board[x][y].Color() != p.Color()) {
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
      if (!board[x][y]) {
        possible.push_back(Move {start,Point{x, y}});
        y++;
      } else if (board[x][y].Color() != p.Color()) {
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
      if (!board[x][y]) {
        possible.push_back(Move {start,Point{x, y}});
        y--;
      } else if (board[x][y].Color() != p.Color()) {
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
 * Method: ChessUtils::bishopPossible(Piece, const Board&)
 *
 *****************************************************************************/
std::vector<Move> ChessUtils::bishopPossible(Piece p, const Board& board )
{
  std::vector<Move> possible;
  Point start = {p.x, p.y};
  {
    auto x = p.x + 1;
    auto y = p.y + 1;
    while (validPoint(x, y)) {
      if (!board[x][y]) {
        possible.push_back(Move {start,Point{x, y}});
        x++;
        y++;
      } else if (board[x][y].Color() != p.Color()) {
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
      if (!board[x][y]) {
        possible.push_back(Move {start,Point{x, y}});
        x--;
        y--;
      } else if (board[x][y].Color() != p.Color()) {
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
      if (!board[x][y]) {
        possible.push_back(Move {start,Point{x, y}});
        y++;
        x--;
      } else if (board[x][y].Color() != p.Color()) {
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
      if (!board[x][y]) {
        possible.push_back(Move {start,Point{x, y}});
        y--;
        x++;
      } else if (board[x][y].Color() != p.Color()) {
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
 * Method: ChessUtils::getKing(Color, const Board&)
 *
 *****************************************************************************/
Point ChessUtils::getKing(Color c, const Board& b)
{
    for (int i = 0; i < 8; i ++) {
      for (int j = 0; j < 8; j++) {
        if (b[i][j].type == KING && b[i][j].Color() == c) {
          return Point { b[i][j].x, b[i][j].y };
        }
      }
    }

    return Point {-1, -1};
}

/******************************************************************************
 *
 * Method: ChessUtils::isColorInCheck(Color, const Board&)
 * 
 *****************************************************************************/
const bool ChessUtils::isColorInCheck(Color c, const Board& b)
{
  auto otherColor = c == WHITE ? BLACK : WHITE;
  auto possible = GAPM_Opposing(c, b);

  auto king = getKing(c, b);

  return containsPoint(king.x, king.y, possible);
}

/******************************************************************************
 *
 * Method: ChessUtils::containsPoint(x, y, possible)
 *
 *****************************************************************************/
bool ChessUtils::containsPoint(int x, int y, std::vector<Move> possible)
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


/******************************************************************************
 * Method: ChessUtils::board_to_fen(x, y, possible)
 * 
 * returns the standard FEN representation of the board
 * https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
 *****************************************************************************/
std::string ChessUtils::board_to_fen(const Board& b)
{
  std::string fen = "";
  int empty = 0;
  int x = 0;
  int y = 0;

  while (x < 8) {
    while (y < 8) {
      if (b[x][y]) {
        fen += b[x][y].typeToFEN();
        y++;
      } else {
        while (y < 8 && !b[x][y]) {
          empty++;
          y++;
        }
        if (empty > 0 && empty <= 8) {
          fen += std::to_string(empty);
          empty = 0;
        }
      }
    }

    y = 0;
    empty = 0;

    if (x != 7) {
      fen += "/";
    }

    x++;
  }

  return fen;
}

/******************************************************************************
 * Method: ChessUtils::fen_to_board(std::string fen)
 * 
 * returns a board from a FEN string
 *****************************************************************************/
ChessUtils::Board ChessUtils::fen_to_board(std::string fen) {

  Board b = std::vector<std::vector<Piece>>(8, std::vector<Piece>(8));
  int x = 0;
  int y = 0;
  for (auto c : fen) {
    if (c == 'x') {
      b[x][y] = Piece(x, y);
      continue;
    }
    if (c == '/') {
      x++;
      y = 0;
    } else if (c >= '1' && c <= '8') {
      y += c - '0';
    } else {
      b[x][y] = Piece(x, y, fen_to_type(c), isupper(c) ? WHITE : BLACK);
      y++;
    }
  }
  return b;
}

/******************************************************************************
 * Method: ChessUtils::fen_to_type(char c)
 * 
 * returns the correlated PieceType
 *****************************************************************************/
PieceType ChessUtils::fen_to_type(char c) {
  switch (toupper(c)) {
    case 'P':
      return PAWN;
    case 'R':
      return ROOK;
    case 'N':
      return KNIGHT;
    case 'B':
      return BISHOP;
    case 'Q':
      return QUEEN;
    case 'K':
      return KING;
    default:
      return NONE;
  }
}
