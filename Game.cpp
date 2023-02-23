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

  SDL_Init(SDL_INIT_VIDEO);

  _window = SDL_CreateWindow("Chess",
              SDL_WINDOWPOS_CENTERED,
              SDL_WINDOWPOS_CENTERED,
              _screenW,
              _screenH,
              SDL_WINDOW_SHOWN);


  _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

  auto* surface = IMG_Load("../resources/circle.png");

  if (surface) {
    _circleTexture = SDL_CreateTextureFromSurface(_renderer, surface);
  }

  if (!_window || !_renderer ) {
    std::cout << "ERROR IN INIT \n";
  }

  // doing this here saves me a couple MB of ram
  p_textures.insert({"../resources/pawn_white.png", loadTexture("../resources/pawn_white.png") });
  p_textures.insert({"../resources/pawn_black.png", loadTexture("../resources/pawn_black.png") });

  // zero the _board
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (i == 0) {
        if (j == 0) {
          _board[i][j] = Piece(i, j, PieceType::ROOK, Color::BLACK);
          p_textures.insert({_board[i][j].icon, loadTexture(_board[i][j].Icon()) });
        } else if (j == 1) {
          _board[i][j] = Piece(i, j, PieceType::KNIGHT, Color::BLACK);
          p_textures.insert({_board[i][j].icon, loadTexture(_board[i][j].Icon()) });
        } else if (j == 2) {
          _board[i][j] = Piece(i, j, PieceType::BISHOP, Color::BLACK);
          p_textures.insert({_board[i][j].icon, loadTexture(_board[i][j].Icon()) });
        } else if (j == 3) {
          _board[i][j] = Piece(i, j, PieceType::QUEEN, Color::BLACK);
          p_textures.insert({_board[i][j].icon, loadTexture(_board[i][j].Icon()) });
        } else if (j == 4) {
          _board[i][j] = Piece(i, j, PieceType::KING, Color::BLACK);
          p_textures.insert({_board[i][j].icon, loadTexture(_board[i][j].Icon()) });
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
          p_textures.insert({_board[i][j].icon, loadTexture(_board[i][j].Icon()) });
        } else if (j == 1) {
          _board[i][j] = Piece(i, j, PieceType::KNIGHT, Color::WHITE);
          p_textures.insert({_board[i][j].icon, loadTexture(_board[i][j].Icon()) });
        } else if (j == 2) {
          _board[i][j] = Piece(i, j, PieceType::BISHOP, Color::WHITE);
          p_textures.insert({_board[i][j].icon, loadTexture(_board[i][j].Icon()) });
        } else if (j == 3) {
          _board[i][j] = Piece(i, j, PieceType::QUEEN, Color::WHITE);
          p_textures.insert({_board[i][j].icon, loadTexture(_board[i][j].Icon()) });
        } else if (j == 4) {
          _board[i][j] = Piece(i, j, PieceType::KING, Color::WHITE);
          p_textures.insert({_board[i][j].icon, loadTexture(_board[i][j].Icon()) });
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

/******************************************************************************
 *
 * Method: Game::~Game()
 *
 *****************************************************************************/
Chess::Game::~Game()
{
  SDL_DestroyWindow(_window);
  SDL_DestroyRenderer(_renderer);
  SDL_Quit();
}

/******************************************************************************
 *
 * Method: Game::Game()
 *
 *****************************************************************************/
SDL_Texture* Chess::Game::loadTexture(const char* filepath)
{
  auto* img = IMG_Load(filepath);

  if (!img) {
    return nullptr;
  }
  SDL_Texture* txt = SDL_CreateTextureFromSurface(_renderer, img);
  
  return txt;
}

/******************************************************************************
 *
 * Method: Game::renderPiece()
 *
 *****************************************************************************/
void Chess::Game::renderPiece(SDL_Texture* texture, Piece p)
{
  SDL_Rect src = {0, 0, 80, 80};
	SDL_Rect dest = { 
                    _screenW / ROWS * p.y + 5,
					          _screenH / COLS * p.x + 5,
					          _screenW / ROWS - 10,
					          _screenH / COLS - 10};
  SDL_RenderCopy(_renderer, texture, &src, &dest);
}

/******************************************************************************
 *
 * Method: Game::run()
 *
 *****************************************************************************/
void Chess::Game::run()
{
  //cached piece that has been previously clicked
  std::optional<Piece> clicked;
  bool first = true;

  // main loop
  // we only render if there is a click on the screen for performance

  while (_state.game != GameState::EXIT) {

    SDL_RenderClear(_renderer);
    if (first) {
      display();
      first = false;
    }

    SDL_Event ev;
    
    SDL_WaitEvent(&ev);

    switch (ev.type) {
      case SDL_QUIT:
        _state.game = GameState::EXIT;
        break;

      case SDL_MOUSEBUTTONDOWN:
      {
        SDL_RenderClear(_renderer);
        display();
        auto x = ev.button.x;
        auto y = ev.button.y;
        int grid_x = floor(x / (_screenH / COLS));
        int grid_y = floor(y / (_screenW / ROWS));

        // investigate if this can be removed
        if (x <= _screenW && y <= _screenH) {
          // get the piece that was clicked on
          if (!clicked.has_value()) {
            if (auto this_piece = _board[grid_y][grid_x];
                this_piece)
            { 
              if (this_piece.Color() == WHITE && _state.isWhiteTurn ||
                  this_piece.Color() == BLACK && !_state.isWhiteTurn)
              {
                clicked = this_piece;
                // stop rendering here and render the board, pieces,
                // and possible moves with renderPossible
                //_has_render_auth = false;

                _possible_moves = generatePossible(_board[grid_y][grid_x], _board);
                renderPossible();
              }
            }

          } else {
            //second click, get params and try to move
            if (clicked.value().color == Color::WHITE && _state.isWhiteTurn ||
                clicked.value().color == Color::BLACK && !_state.isWhiteTurn)
            {
              // if the coordindates are in the list of possible moves
              // and doesnt result in check for the SAME color,
              // then move.
              if (containsPoint(grid_y, grid_x, _possible_moves) &&
                  !resultsInCheck(clicked.value().x,
                                  clicked.value().y,
                                  grid_y, grid_x)) 
              {
                move(clicked.value().x, clicked.value().y, grid_y, grid_x, _board);
                _state.isWhiteTurn = !_state.isWhiteTurn;
                //_has_render_auth = true;
                _possible_moves.clear();
              }

              SDL_RenderClear(_renderer);
              display();
            }

            clicked.reset(); 
          } 
        }
        break;
      }
      default:
        break;
    }
  }
}

/******************************************************************************
 *
 * Method: Game::display()
 * 
 * - render and display the background
 *****************************************************************************/
void Chess::Game::renderBackground()
{
	bool white = true;
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {

			if (white) {
				SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
			} else {
				SDL_SetRenderDrawColor(_renderer, 83, 132, 172, 255);
			}
			white = !white;
			SDL_Rect rectangle = { i * _screenW / ROWS,
								  j * _screenH / COLS,
								  _screenW / ROWS,
								  _screenH / COLS };
			SDL_RenderFillRect(_renderer, &rectangle);
		}
		white = !white;
	}
}

/******************************************************************************
 *
 * Method: Game::renderPossible()
 * 
 * - display the possible moves
 *****************************************************************************/
void Chess::Game::renderPossible()
{
 SDL_RenderClear(_renderer);

 renderBackground();

  // render all peices
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (_board[i][j]) {
        renderPiece(p_textures.at(_board[i][j].Icon()), _board[i][j]);
      }
    }
   }

 SDL_Rect src = {0,0, 180, 180};
 
 for (auto point : _possible_moves) {
    auto x = point.x;
    auto y = point.y;

    SDL_Rect dest = {
              _screenW / ROWS * y + 30,
		          _screenH / COLS * x + 30,
			        20,
			        20};
    SDL_RenderCopy(_renderer, _circleTexture, &src, &dest);
 }

 SDL_RenderPresent(_renderer);
}

/******************************************************************************
 *
 * Method: Game::generatePossible()
 * 
 * - display the possible moves
 *****************************************************************************/
std::vector<Chess::Game::Point> Chess::Game::generatePossible(Chess::Piece p, Piece (&board)[8][8] )
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

     if (!p.has_moved && !board[p.x - mod*2][p.y]) {
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
       knightKing(x, y);
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
 * Method: Game::display()
 * 
 * - render and display the background and pieces 
 *****************************************************************************/
void Chess::Game::display()
{
   SDL_RenderClear(_renderer);

   renderBackground();

    // render all peices
    for (int i = 0; i < ROWS; i++) {
      for (int j = 0; j < COLS; j++) {
        if (_board[i][j]) {
          renderPiece(p_textures.at(_board[i][j].Icon()), _board[i][j]);
        }
      }
    }

    // Display what we copied into the renderer
    SDL_RenderPresent(_renderer);
}

/******************************************************************************
 *
 * Method: Game::move()
 * 
 * - move a piece to a new location within the actual _board
 *****************************************************************************/
void Chess::Game::move(int from_x, int from_y, int to_x, int to_y, Piece (&board)[8][8])
{
  std::cout << "Moving { " << from_x << " , " << from_y << "}\n"
            << "to { " << to_x << " , " << to_y << "}\n";
  
  // copy constructor desperately needed
  board[to_x][to_y].x = to_x;
  board[to_x][to_y].y = to_y;
  board[to_x][to_y].type = board[from_x][from_y].type;
  board[to_x][to_y].color = board[from_x][from_y].color;
  board[to_x][to_y].icon = board[from_x][from_y].icon;
  board[to_x][to_y].has_moved = true; 

  board[from_x][from_y].Clear();
}

/******************************************************************************
 *
 * Method: Game::resultsInCheck()
 * -- given a proposed move, see if that move results in check
 *    for the same color piece
 *
 *****************************************************************************/
bool Chess::Game::resultsInCheck(int from_x, int from_y, int to_x, int to_y)
{
  
  auto pieceColor = _board[from_x][from_y].Color();

  Piece local[8][8];
  std::copy(&_board[0][0], &_board[0][0]+8*8, &local[0][0]);
  std::vector<Point> possible;

  // state of the board as if the move took place
  move(from_x, from_y, to_x, to_y, local);

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j ++) {
      if (local[i][j].Color() != pieceColor) {
        auto piecePossible = generatePossible(local[i][j], local);
        possible.insert(std::end(possible),std::begin(piecePossible), std::end(piecePossible));
      }
    }
  }

  auto getKing = [&] (auto color) {
    for (int i = 0; i < 8; i ++) {
      for (int j = 0; j < 8; j++) {
        if (local[i][j].type == KING && local[i][j].Color() == color) {
          return Point { local[i][j].x, local[i][j].y };
        }
      }
    }
    return Point {-1, -1};
  };

  auto king = getKing(pieceColor);

  return containsPoint(king.x, king.y, possible) ? true : false;
}

/******************************************************************************
 *
 * Method: Game::print_board()
 *
 *****************************************************************************/
void Chess::Game::printBoard()
{
 for (int i = 0; i < ROWS; i ++) {
   for (int j = 0; j < COLS; j++) {
     _board[i][j].printPiece();
   }
  std::cout << "\n";
 }
}

/******************************************************************************
 *
 * Method: Game::print_board()
 *
 *****************************************************************************/
bool Chess::Game::containsPoint(int x, int y, std::vector<Point> possible)
{
  if (validPoint(x,y)) {
    for (auto point : possible) {
      if (point.x == x && point.y == y) {
        return true;
      }
    }
    return false;
  }
  return false;
}

/******************************************************************************
 *
 * Method: Game::rookPossible()
 *
 *****************************************************************************/
std::vector<Chess::Game::Point> Chess::Game::rookPossible(Piece p, Piece (&board)[8][8] )
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
        x = -1;
      } else {
        x = -1;
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
        x = -1;
      } else {
        x = -1;
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
        x = -1;
      } else {
        x = -1;
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
        x = -1;
      } else {
        x = -1;
      }
    }
  }
  return possible;
}

std::vector<Chess::Game::Point> Chess::Game::bishopPossible(Piece p, Piece (&board)[8][8] )
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
        x = -1;
      } else {
        x = -1;
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
        x = -1;
      } else {
        x = -1;
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
        x = -1;
      } else {
        x = -1;
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
        x = -1;
      } else {
        x = -1;
      }
    }
  }
  return possible;
}
