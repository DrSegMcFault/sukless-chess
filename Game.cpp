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
 * Method: Game::move(Move, possible moves)
 * this should really return a enum called Result { SUCCESS, FAILURE, CHECKMATE }
 *****************************************************************************/
MoveResult Game::move(Move m)
{
  auto possible_moves = genThisPossible(pieceAt(m.from.x, m.from.y));

  if (containsPoint(m.to.x, m.to.y, possible_moves) &&
      !resultsInCheck(m))
  {
    ChessUtils::move(m, _board);

    _isWhiteTurn = !_isWhiteTurn;

    _move_count++;
    
    history.push_back(ChessUtils::board_to_fen(_board));

    if (history.size() >= 3) {
      auto count = 0;
      auto last = history[history.size() - 1]; 
      for (int i = 0; i < history.size() - 2; i++) {
        if (history[i].compare(last) == 0) {
          count++;
        }
      }
      // 3 move repetition draw
      if (count >= 3) {
        std::cout << "3 move repetition draw" << std::endl;
        return MoveResult::DRAW;
      }
    }
    
    return isCheckmate() ? MoveResult::CHECKMATE : MoveResult::VALID;
  }
  // something other than 3 move repetition, or a valid move
  std::cout << "invalid move" << std::endl;
  return MoveResult::INVALID;
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
 * Method: Game::resultsInCheck()
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
    ChessUtils::move(m, local);

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

  history.clear();
  _isWhiteTurn = true;
  _move_count = 0;
  history.push_back(ChessUtils::board_to_fen(_board));
}
