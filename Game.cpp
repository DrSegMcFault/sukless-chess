#include <iostream>
#include <optional>
#include "common_enums.h"
#include "Game.h"

/******************************************************************************
 *
 * Method: Game::Game()
 *
 *****************************************************************************/
Chess::Game::Game()
{
  initBoard();
}

/******************************************************************************
 * PUBLIC
 * Method: Game::reset()
 *
 *****************************************************************************/
void Chess::Game::reset()
{
  _isWhiteTurn = true;
  initBoard();
}

/******************************************************************************
 * PUBLIC
 * Method: Game::genThisPossible()
 * - returns the possible moves for this piece
 *****************************************************************************/
std::vector<Chess::Point> Chess::Game::genThisPossible(Chess::Piece p)
{
  return genPossibleMvPiece(p, _board);
}

/******************************************************************************
 * PUBLIC
 * Method: Game::move(x, y, x, y, possible moves)
 *
 *****************************************************************************/
bool Chess::Game::move(int from_x, int from_y, int to_x, int to_y,
                       std::vector<Point> possible_moves)
{
  if (containsPoint(to_x, to_y, possible_moves) &&
      !resultsInCheck(from_x, from_y, to_x, to_y))
  {
    move(from_x, from_y, to_x, to_y, _board);
    _isWhiteTurn = !_isWhiteTurn;
    return true;
  }

  return false;
}

/******************************************************************************
 * PUBLIC
 * Method: Game::isCheckmate()
 *
 *****************************************************************************/
bool Chess::Game::isCheckmate()
{
  const auto cur_player_color = _isWhiteTurn ? WHITE : BLACK;
  
  // check if all the possible moves for the current player result in check
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      auto piece = _board[i][j];
      if (piece && piece.Color() == cur_player_color) {
        auto this_piece_possible = genPossibleMvPiece(piece, _board);
        for (auto point : this_piece_possible) {
          if (!resultsInCheck(piece.x, piece.y, point.x, point.y)) {
            return false; 
          }
        }
      }
    }
  }

  return true;
}

/******************************************************************************
 * PUBLIC
 * Method: Game::pieceAt(x, y)
 *****************************************************************************/
Chess::Piece Chess::Game::pieceAt(int x, int y)
{
  return validPoint(x, y) ? _board[x][y] : Piece();
}

/******************************************************************************
 * PUBLIC
 * Method: Game::colorMatchesTurn(Color)
 *****************************************************************************/
const bool Chess::Game::colorMatchesTurn(Color c)
{
  return (c == WHITE && _isWhiteTurn) ||
         (c == BLACK && !_isWhiteTurn);
}

/******************************************************************************
 *
 * Method: Game::genPossibleMvPiece()
 * 
 * - generate the possible moves for a given piece and board 
 *****************************************************************************/
std::vector<Chess::Point> Chess::Game::genPossibleMvPiece(Chess::Piece p, Piece (&board)[8][8] )
{
  std::vector<Point> possible;
  auto mod = p.color == Chess::WHITE ? 1 : -1;

  // determine if move is possible for knight and king
  auto knightKing = [&] (auto x, auto y) {
    if (validPoint(x,y)) {
      if (!board[x][y] || (board[x][y] && board[x][y].Color() != p.Color())) {
        possible.push_back(Point{x, y});
      }
    }
  };

  switch (p.type) {
    case PAWN:
    {
      // if there isnt a piece in front of the pawn, its possible
      if (!board[p.x - mod][p.y]) {
        possible.push_back(Point{p.x - mod, p.y});
      }

      if (board[p.x - mod][p.y - mod] && p.color != board[p.x-mod][p.y-mod].color) {
        possible.push_back(Point{p.x - mod, p.y - mod});
      }

      if (board[p.x - mod][p.y + mod] && p.Color() != board[p.x-mod][p.y+mod].Color()) {
        possible.push_back(Point{p.x - mod, p.y + mod});
      }


     if (!p.has_moved &&
       !board[p.x - mod][p.y] &&
       !board[p.x - mod*2][p.y])
     {
       possible.push_back(Point{p.x - mod*2, p.y});
     }
     break;
   }

   case ROOK:
   { 
      auto rook = rookPossible(p, board);
      possible.insert(std::end(possible), std::begin(rook), std::end(rook));
      rookPossible(p, board);
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
               possible.push_back(Point {p.x, p.y + 2});
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
                 possible.push_back(Point {p.x, p.y - 2});
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
 * Method: Game::genAllPossibleOpposing()
 * 
 * - generate all possible moves for the opposing color
 *****************************************************************************/
std::vector<Chess::Point> Chess::Game::genAllPossibleOpposing(Color c, Piece (&b)[8][8])
{
  std::vector<Point> possible;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j ++) {
      if (b[i][j] && b[i][j].Color() != c) {
        auto piecePossible = genPossibleMvPiece(b[i][j], b);
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
 * Method: Game::move()
 * 
 * - move a piece to a new location within a given board
 *****************************************************************************/
void Chess::Game::move(int from_x, int from_y, int to_x, int to_y, Piece (&board)[8][8])
{
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
      (to_x == 0 || to_x == 8))
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
      move(king_x, king_y + 1, king_x, king_y - 1, board);
    } else {
      // queen side castle
      move(king_x, king_y - 2, king_x, king_y + 1, board);
    } 
  }
}

/******************************************************************************
 *
 * Method: Game::resultsInCheck()
 *
 * -- given a proposed move, see if that move results in check
 *    for the same color piece
 *****************************************************************************/
bool Chess::Game::resultsInCheck(int from_x, int from_y, int to_x, int to_y)
{
  auto pieceColor = _board[from_x][from_y].Color();

  Piece local[8][8];
  std::vector<Point> possible;
  auto dy_pos = abs(from_y - to_y);

  // castling move, cant castle out of, through, or into check
  if (_board[from_x][from_y].type == KING && dy_pos >= 2) {
  
    auto y_dir = to_y - from_y < 0 ? -1 : 1;
    possible = genAllPossibleOpposing(pieceColor, _board);

    return (containsPoint(from_x, from_y, possible) ||
            containsPoint(from_x, from_y + (1 * y_dir), possible) ||
            containsPoint(from_x, from_y + (2 * y_dir), possible));
  } else {

    std::copy(&_board[0][0], &_board[0][0]+8*8, &local[0][0]);
    move(from_x, from_y, to_x, to_y, local);

    possible = genAllPossibleOpposing(pieceColor, local);

    auto king = getKing(pieceColor, local);

    return containsPoint(king.x, king.y, possible);
  }
}

/******************************************************************************
 *
 * Method: Game::getKing()
 *
 *****************************************************************************/
Chess::Point Chess::Game::getKing(Color c, Piece (&b)[8][8] )
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
 * Method: Game::containsPoint()
 *
 *****************************************************************************/
bool Chess::Game::containsPoint(int x, int y, std::vector<Point> possible)
{
  if (!validPoint(x,y)) {
    return false;
  }

  for (auto point : possible) {
    if (point.x == x && point.y == y) {
      return true;
    }
  }
  return false;
}

/******************************************************************************
 *
 * Method: Game::rookPossible()
 *
 *****************************************************************************/
std::vector<Chess::Point> Chess::Game::rookPossible(Piece p, Piece (&board)[8][8] )
{
  std::vector<Point> possible;
  {
    auto x = p.x + 1;
    auto y = p.y;
    while (validPoint(x, y)) {
      if (!board[x][y]) {
        possible.push_back(Point{x, y});
        x++;
      } else if (board[x][y].Color() != p.Color()) {
        possible.push_back(Point{x, y});
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
        possible.push_back(Point{x, y});
        x--;
      } else if (board[x][y].Color() != p.Color()) {
        possible.push_back(Point{x, y});
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
        possible.push_back(Point{x, y});
        y++;
      } else if (board[x][y].Color() != p.Color()) {
        possible.push_back(Point{x, y});
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
        possible.push_back(Point{x, y});
        y--;
      } else if (board[x][y].Color() != p.Color()) {
        possible.push_back(Point{x, y});
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
 * Method: Game::bishopPossible()
 *
 *****************************************************************************/
std::vector<Chess::Point> Chess::Game::bishopPossible(Piece p, Piece (&board)[8][8] )
{
  std::vector<Point> possible;
  {
    auto x = p.x + 1;
    auto y = p.y + 1;
    while (validPoint(x, y)) {
      if (!board[x][y]) {
        possible.push_back(Point{x, y});
        x++;
        y++;
      } else if (board[x][y].Color() != p.Color()) {
        possible.push_back(Point{x, y});
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
        possible.push_back(Point{x, y});
        x--;
        y--;
      } else if (board[x][y].Color() != p.Color()) {
        possible.push_back(Point{x, y});
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
        possible.push_back(Point{x, y});
        y++;
        x--;
      } else if (board[x][y].Color() != p.Color()) {
        possible.push_back(Point{x, y});
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
        possible.push_back(Point{x, y});
        y--;
        x++;
      } else if (board[x][y].Color() != p.Color()) {
        possible.push_back(Point{x, y});
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
 * Method: Game::initBoard()
 *
 *****************************************************************************/
void Chess::Game::initBoard()
{
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (i == 0) {
        if (j == 0) {
          _board[i][j] = Piece(i, j, PieceType::ROOK, Color::BLACK);
        } else if (j == 1) {
          _board[i][j] = Piece(i, j, PieceType::KNIGHT, Color::BLACK);
        } else if (j == 2) {
          _board[i][j] = Piece(i, j, PieceType::BISHOP, Color::BLACK);
        } else if (j == 3) {
          _board[i][j] = Piece(i, j, PieceType::QUEEN, Color::BLACK);
        } else if (j == 4) {
          _board[i][j] = Piece(i, j, PieceType::KING, Color::BLACK);
        } else if (j == 5) {
          _board[i][j] = Piece(i, j, PieceType::BISHOP, Color::BLACK);
        } else if (j == 6) {
          _board[i][j] = Piece(i, j, PieceType::KNIGHT, Color::BLACK);
        } else if (j == 7) {
          _board[i][j] = Piece(i, j, PieceType::ROOK, Color::BLACK);
        }
        
      } else if (i == 1) {
        _board[i][j] = Piece(i, j, PieceType::PAWN, Color::BLACK);
      } else if (i == 6) {
        _board[i][j] = Piece(i, j, PieceType::PAWN, Color::WHITE);
      } else if (i == 7) {
        if (j == 0) {
          _board[i][j] = Piece(i, j, PieceType::ROOK, Color::WHITE);
        } else if (j == 1) {
          _board[i][j] = Piece(i, j, PieceType::KNIGHT, Color::WHITE);
        } else if (j == 2) {
          _board[i][j] = Piece(i, j, PieceType::BISHOP, Color::WHITE);
        } else if (j == 3) {
          _board[i][j] = Piece(i, j, PieceType::QUEEN, Color::WHITE);
        } else if (j == 4) {
          _board[i][j] = Piece(i, j, PieceType::KING, Color::WHITE);
        } else if (j == 5) {
          _board[i][j] = Piece(i, j, PieceType::BISHOP, Color::WHITE);
        } else if (j == 6) {
          _board[i][j] = Piece(i, j, PieceType::KNIGHT, Color::WHITE);
        } else if (j == 7) {
          _board[i][j] = Piece(i, j, PieceType::ROOK, Color::WHITE);
        }
      } else {
        // no piece
        _board[i][j] = Piece(i, j);
      }
    }
  }
}
