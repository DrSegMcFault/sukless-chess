#include "ChessUtils.h"
#include <iostream>
/******************************************************************************
 *
 * Method: ChessUtils::genPossibleMvPiece()
 * 
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
 * Method: ChessUtils::genAllPossibleOpposing()
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
 * Method: ChessUtils::rookPossible()
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
 * Method: ChessUtils::bishopPossible()
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
 * Method: ChessUtils::getKing()
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
 * Method: ChessUtils::genPossibleMvPiece()
 * 
 * - generate the possible moves for a given piece and board 
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
 * Method: ChessUtils::move()
 * 
 * - move a piece to a new location within a given board
 *****************************************************************************/
void ChessUtils::move(Move m, Board& board)
{
  auto from_x = m.from.x;
  auto from_y = m.from.y;
  auto to_x = m.to.x;
  auto to_y = m.to.y;

  if (from_x == to_x && from_y == to_y) {
    return;
  }
  
  auto dx = abs(to_x - from_x);
  auto dy = abs(to_y - from_y);
  bool is_castle = dy == 2 && board[from_x][from_y].type == KING;
  
  // copy constructor desperately needed
  board[to_x][to_y].prev_x = from_x; 
  board[to_x][to_y].prev_y = from_y; 
  board[to_x][to_y].x = to_x;
  board[to_x][to_y].y = to_y;
  board[to_x][to_y].type = board[from_x][from_y].type;
  board[to_x][to_y].color = board[from_x][from_y].color;
  board[to_x][to_y].icon = board[from_x][from_y].icon;
  board[to_x][to_y].has_moved = true; 
  board[from_x][from_y].Clear();

  // auto promote to queen
  if (board[to_x][to_y].type == PAWN &&
      (to_x == 0 || to_x == 7))
  {
    board[to_x][to_y].type = QUEEN;
    board[to_x][to_y].icon = "resources/queen_" +
                             board[to_x][to_y].colorToString() +
                             ".png";
  }

  if (is_castle) {
    auto dx = to_x - from_x;
    auto dy = to_y - from_y;
    auto king_x = to_x;
    auto king_y = to_y;

    // if the difference in y is positive, its a king side castle
    // move the rook to the correct position
    if (dy > 0) {
      move(Move {Point {king_x, king_y + 1} , Point {king_x, king_y -1}},
           board);
    } else {
      // queen side castle
      move(Move {Point {king_x, king_y - 2} , Point {king_x, king_y + 1} },
           board);
    } 
  }
}

/******************************************************************************
 *
 * Method: ChessUtils::containsPoint()
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

// returns the standard FEN representation of the board
// https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
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
// returns a board from a FEN string
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
