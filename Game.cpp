#include "Game.h"
#include <iostream>

/******************************************************************************
 *
 * Method: Game::Game()
 *
 *****************************************************************************/
Game::Game()
{
  _board = std::vector<std::vector<Piece>>(8, std::vector<Piece>(8, Piece()));
  initBoard();
}

/******************************************************************************
 * PUBLIC
 * Method: Game::reset()
 *
 *****************************************************************************/
void Game::reset()
{
  initBoard();
}

/******************************************************************************
 * PUBLIC
 * Method: Game::genThisPossible()
 * - returns the possible moves for this piece
 *****************************************************************************/
std::vector<Move> Game::genThisPossible(Piece p)
{
  return GPM_Piece(p, _board);
}

/******************************************************************************
 * PUBLIC
 * Method: Game::move(Move m)
 *****************************************************************************/
MoveResult Game::move(Move m)
{
  auto possible_moves = genThisPossible(pieceAt(m.from.x, m.from.y));

  if (containsPoint(m.to.x, m.to.y, possible_moves) &&
      !resultsInCheck(m))
  {
    auto move_type = do_move(m, _board);

    switch(move_type) {
      case MoveType::NORMAL:
        // do nothing
        break;
      case MoveType::K_SIDE_CASTLE:
        // update the FEN castling rights
        break;
      case MoveType::Q_SIDE_CASTLE:
        // update the FEN castling rights
        break;
      case MoveType::EN_PASSANT:
        // update the fen en passant target square 
        _en_passant_enabled = true;
        break;
    }

    _isWhiteTurn = !_isWhiteTurn;

    // update the FEN turn here

    _move_count++;
    
    _history.push_back(ChessUtils::board_to_fen(_board));

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
 * Method: Game::do_move(Move, Board)
 * 
 * - performs the move and returns the type of move
 *****************************************************************************/
MoveType Game::do_move(Move m, Board &board)
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
        return MoveType::EN_PASSANT;
      }

    } else if (validPoint(to_x, to_y + 1)) {

      if (board[to_x][to_y + 1].type == PAWN) {
        return MoveType::EN_PASSANT;
      }

    }
  }


  return MoveType::NORMAL;
}

/******************************************************************************
 * PUBLIC
 * Method: Game::isCheckmate()
 *
 *****************************************************************************/
bool Game::isCheckmate()
{
  const auto cur_player_color = _isWhiteTurn ? WHITE : BLACK;
  
  // check if all the possible moves for the current player result in check
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      auto piece = _board[i][j];
      if (piece && piece.Color() == cur_player_color) {
        auto this_piece_possible = GPM_Piece(piece, _board);
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
 * Method: Game::pieceAt(x, y)
 *****************************************************************************/
Piece Game::pieceAt(int x, int y)
{
  return validPoint(x, y) ? _board[x][y] : Piece();
}

/******************************************************************************
 * PUBLIC
 * Method: Game::getBoard()
 *****************************************************************************/
Game::Board Game::getBoard()
{
  return _board;
}

/******************************************************************************
 * PUBLIC
 * Method: Game::historyAt(int index)
 *****************************************************************************/
const std::string Game::historyAt(int index)
{
  return _history.at(index);
}

/******************************************************************************
 * PUBLIC
 * Method: Game::colorMatchesTurn(Color)
 *****************************************************************************/
const bool Game::colorMatchesTurn(Color c)
{
  return (c == WHITE && _isWhiteTurn) ||
         (c == BLACK && !_isWhiteTurn);
}

/******************************************************************************
 * PUBLIC
 * Method: Game::colorMatchesTurn(Color)
 *****************************************************************************/
std::vector<Move> Game::genAllPossibleOpposing(Color c)
{
  return GAPM_Opposing(c, _board);
}

/******************************************************************************
 *
 * Method: Game::resultsInCheck(Move m)
 *
 * -- given a proposed move, see if that move results in check
 *    for the same color piece
 *****************************************************************************/
bool Game::resultsInCheck(Move m)
{
  auto pieceColor = _board[m.from.x][m.from.y].Color();

  std::vector<Move> possible;
  auto dy_pos = abs(m.from.y - m.to.y);

  // castling move, cant castle out of, through, or into check
  if (_board[m.from.x][m.from.y].type == KING && dy_pos >= 2) {
  
    auto y_dir = m.to.y - m.from.y < 0 ? -1 : 1;
    possible = GAPM_Opposing(pieceColor, _board);

    return (containsPoint(m.from.x, m.from.y, possible) ||
            containsPoint(m.from.x, m.from.y + (1 * y_dir), possible) ||
            containsPoint(m.from.x, m.from.y + (2 * y_dir), possible));
  } else {

    Board local(_board);
    auto res = do_move(m, local);

    possible = GAPM_Opposing(pieceColor, local);

    auto king = getKing(pieceColor, local);

    return containsPoint(king.x, king.y, possible);
  }
}

/******************************************************************************
 *
 * Method: Game::initBoard()
 *
 *****************************************************************************/
void Game::initBoard()
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
  _history.push_back(ChessUtils::board_to_fen(_board));
}
