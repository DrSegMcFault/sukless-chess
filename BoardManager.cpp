#include "BoardManager.h"
#include <iostream>

/******************************************************************************
 *
 * Method: BoardManager::BoardManager()
 *
 *****************************************************************************/
BoardManager::BoardManager()
{
  _board = std::vector<std::vector<Piece>>(8, std::vector<Piece>(8, Piece()));
  initBoard();
}

/******************************************************************************
 *
 * Method: BoardManager::BoardManager()
 *
 *****************************************************************************/
BoardManager::BoardManager(std::string fen)
{
  fen_to_state(fen);
}

/******************************************************************************
 * PUBLIC
 * Method: BoardManager::reset()
 *
 *****************************************************************************/
void BoardManager::reset()
{
  initBoard();
}

/******************************************************************************
 * PUBLIC
 * Method: BoardManager::genThisPossible()
 * - returns the possible moves for this piece
 *****************************************************************************/
std::vector<Move> BoardManager::genThisPossible(Piece p)
{
  return GPM_Piece(p);
}

/******************************************************************************
 * PUBLIC
 * Method: BoardManager::move(Move m)
 *****************************************************************************/
MoveResult BoardManager::move(Move m)
{
  auto possible_moves = genThisPossible(pieceAt(m.from.x, m.from.y));

  if (containsPoint(m.to.x, m.to.y, possible_moves) &&
      !resultsInCheck(m))
  {
    if (do_move(m, _board) == MoveType::ENABLE_PASSANT) {
      _en_passant_enabled = true;
      auto mod = _isWhiteTurn ? 1 : -1;
      _passant_target = Point(m.to.x - mod, m.to.y);
    }

    // black is moving, increment the full move count
    if (!_isWhiteTurn) {
      _move_count++;
    }

    _isWhiteTurn = !_isWhiteTurn;

    _half_move_count++;
    
    _history.push_back(board_to_fen());

    if (_history.size() >= 3) {
      auto count = 0;
      auto last = _history[_history.size() - 1]; 
      for (int i = 0; i < _history.size() - 2; i++) {
        if (_history[i].compare(last) == 0) {
          count++;
        }
      }
      // 3 move repetition draw
      if (count >= 3) {
        return MoveResult::DRAW;
      }
    }
    
    return isCheckmate() ? MoveResult::CHECKMATE : MoveResult::VALID;
  }
  // something other than 3 move repetition, checkmate or a valid move
  return MoveResult::INVALID;
}

/******************************************************************************
 *
 * Method: BoardManager::do_move(Move, Board)
 * 
 * - performs the move and returns the type of move
 *****************************************************************************/
MoveType BoardManager::do_move(Move m, Board &board)
{
  auto from_x = m.from.x;
  auto from_y = m.from.y;
  auto to_x = m.to.x;
  auto to_y = m.to.y;

  auto dx = abs(to_x - from_x);
  auto dy = abs(to_y - from_y);
  bool is_castle = dy == 2 && board[from_x][from_y].type == KING;
  auto mod = board[from_x][from_y].color == WHITE ? 1 : -1;

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
      do_move(Move { Point {king_x, king_y + 1} , Point {king_x, king_y - 1} },
           board);
      return MoveType::K_SIDE_CASTLE;
    } else {
      // queen side castle
      do_move(Move { Point {king_x, king_y - 2} , Point {king_x, king_y + 1} },
           board);
      return MoveType::Q_SIDE_CASTLE;
    } 
  }

  // this will let use know that we can en_passant on the next move
  if (board[to_x][to_y].type == PAWN &&
      (dx == 2))
  {
    if (validPoint(to_x, to_y - 1)) {

      if (board[to_x][to_y - 1].type == PAWN ) {
        return MoveType::ENABLE_PASSANT;
      }

    } else if (validPoint(to_x, to_y + 1)) {

      if (board[to_x][to_y + 1].type == PAWN) {
        return MoveType::ENABLE_PASSANT;
      }

    }
  }


  return MoveType::NORMAL;
}

/******************************************************************************
 * PUBLIC
 * Method: BoardManager::isCheckmate()
 *
 *****************************************************************************/
bool BoardManager::isCheckmate()
{
  const auto cur_player_color = _isWhiteTurn ? WHITE : BLACK;
  
  // check if all the possible moves for the current player result in check
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      auto piece = _board[i][j];
      if (piece && piece.Color() == cur_player_color) {
        auto this_piece_possible = GPM_Piece(piece);
        for (auto move : this_piece_possible) {
          if (!resultsInCheck(move)) {
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
 * Method: BoardManager::pieceAt(x, y)
 *****************************************************************************/
Piece BoardManager::pieceAt(int x, int y)
{
  return validPoint(x, y) ? _board[x][y] : Piece();
}

/******************************************************************************
 * PUBLIC
 * Method: BoardManager::getBoard()
 *****************************************************************************/
BoardManager::Board BoardManager::getBoard()
{
  return _board;
}

/******************************************************************************
 * PUBLIC
 * Method: BoardManager::historyAt(int index)
 *****************************************************************************/
const std::string BoardManager::historyAt(int index)
{
  return _history.at(index);
}

/******************************************************************************
 * PUBLIC
 * Method: BoardManager::colorMatchesTurn(Color)
 *****************************************************************************/
const bool BoardManager::colorMatchesTurn(Color c)
{
  return (c == WHITE && _isWhiteTurn) ||
         (c == BLACK && !_isWhiteTurn);
}

/******************************************************************************
 * PUBLIC
 * Method: BoardManager::colorMatchesTurn(Color)
 *****************************************************************************/
std::vector<Move> BoardManager::genAllPossibleOpposing(Color c)
{
  return GAPM_Opposing(c);
}

/******************************************************************************
 *
 * Method: BoardManager::resultsInCheck(Move m)
 *
 * -- given a proposed move, see if that move results in check
 *    for the same color piece
 *****************************************************************************/
bool BoardManager::resultsInCheck(Move m)
{
  auto pieceColor = _board[m.from.x][m.from.y].Color();

  std::vector<Move> possible;
  auto dy_pos = abs(m.from.y - m.to.y);

  // castling move, cant castle out of, through, or into check
  if (_board[m.from.x][m.from.y].type == KING && dy_pos >= 2) {
  
    auto y_dir = m.to.y - m.from.y < 0 ? -1 : 1;
    possible = GAPM_Opposing(pieceColor);

    return (containsPoint(m.from.x, m.from.y, possible) ||
            containsPoint(m.from.x, m.from.y + (1 * y_dir), possible) ||
            containsPoint(m.from.x, m.from.y + (2 * y_dir), possible));
  } else {

    Board local(_board);

    possible = GAPM_Opposing(pieceColor);

    auto king = getKing(pieceColor);

    return containsPoint(king.x, king.y, possible);
  }
}

/******************************************************************************
 *
 * Method: BoardManager::initBoard()
 *
 *****************************************************************************/
void BoardManager::initBoard()
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

  _history.clear();
  _isWhiteTurn = true;
  _move_count = 0;
  _half_move_count = 0;
  _history.push_back(board_to_fen());
}


/******************************************************************************
 * Method: BoardManaher::board_to_fen(x, y, possible)
 * 
 * returns the standard FEN representation of the board
 * https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
 *****************************************************************************/
std::string BoardManager::board_to_fen()
{
  std::string fen = "";
  int empty = 0;
  int x = 0;
  int y = 0;

  while (x < 8) {
    while (y < 8) {
      if (_board[x][y]) {
        fen += _board[x][y].typeToFEN();
        y++;
      } else {
        while (y < 8 && !_board[x][y]) {
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

  // add turn
  fen += _isWhiteTurn ? " w " : " b ";

  // castling rights
  auto white_king = getKing(Color::WHITE);
  auto black_king = getKing(Color::BLACK);
  
  if (pieceAt(white_king.x, white_king.y).has_moved &&
      pieceAt(black_king.x, black_king.y).has_moved) 
  {
    fen += "-";
  } 
  else { 

    if (pieceAt(0,0).type == ROOK &&
        !pieceAt(0, 0).has_moved &&
        !pieceAt(black_king.x, black_king.y).has_moved)
    {
      fen += "Q";
    }

    if (pieceAt(0,7).type == ROOK &&
        !pieceAt(0, 7).has_moved &&
        !pieceAt(black_king.x, black_king.y).has_moved)
    {
      fen += "K";
    }

    if (pieceAt(7,0).type == ROOK &&
        !pieceAt(7, 0).has_moved &&
        !pieceAt(white_king.x, white_king.y).has_moved)
    {
      fen += "q";
    }

    if (pieceAt(7,7).type == ROOK &&
        !pieceAt(7, 7).has_moved &&
        !pieceAt(white_king.x, white_king.y).has_moved)
    {
      fen += "k";
    }
  }

  fen += " ";
  
  // en passant target square

  // halfmove clock
  fen += std::to_string(_half_move_count) + " ";

  // fullmove number
  fen += std::to_string(_move_count);
  std::cout << fen << std::endl;

  return fen;
}

/******************************************************************************
 * Method: BoardManager::fen_to_board(std::string fen)
 * 
 * returns a board from a FEN string
 *****************************************************************************/
std::string BoardManager::fen_to_state(std::string fen)
{
  std::cout << "Fen to state: " << fen << "\n";

  _board = fen_to_board(fen);

  // just tokenize the fen and get by known indices
  _isWhiteTurn = fen[fen.find(" ") + 1] == 'w' ? true : false;

  auto str = fen.substr(fen.find_last_of(" ") + 1);
  _move_count = std::stoi(str);

  // half move count is a pain to parse
  std::cout << "Move count: " << _move_count << "\n" << "Half move count: " << _half_move_count << "\n";
}

/******************************************************************************
 * Method: BoardManager::fen_to_board(std::string fen)
 * 
 * returns a board from a FEN string
 *****************************************************************************/
BoardManager::Board BoardManager::fen_to_board(std::string fen) {

  Board b = std::vector<std::vector<Piece>>(8, std::vector<Piece>(8));
  int x = 0;
  int y = 0;
  for (auto c : fen) {
    if (c == ' ') {
      break;
    } else if (c == '/') {
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
 * Method: BoardManager::fen_to_type(char c)
 * 
 * returns the correlated PieceType
 *****************************************************************************/
PieceType BoardManager::fen_to_type(char c) {
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
