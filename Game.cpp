#include <iostream>
#include <optional>

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


  _renderer = SDL_CreateRenderer(_window, -1, 0);
  
  if (!_window || !_renderer ) {
    std::cout << "ERROR IN INIT \n";
  }

  // zero the _board
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
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
  SDL_Rect src = {0, 0, 451, 451};
	SDL_Rect dest = { 
                    _screenW / ROWS * p.y,
					          _screenH / COLS * p.x,
					          _screenW / ROWS,
					          _screenH / COLS };
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
  std::vector<Point> possible_moves;
  // main loop
  while (_state.game != GameState::EXIT) {

    if (_has_render_auth) {
      display();
    }

    SDL_Event ev;
    
    SDL_WaitEvent(&ev);

    switch (ev.type) {
      case SDL_QUIT:
        _state.game = GameState::EXIT;
        break;

      case SDL_MOUSEBUTTONDOWN:
      {
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
                _has_render_auth = false;
                possible_moves = renderPossible(_board[grid_y][grid_x]);
              }
            }

          } else {
            //second click, get params and try to move
            if (clicked.value().color == Color::WHITE && _state.isWhiteTurn ||
                clicked.value().color == Color::BLACK && !_state.isWhiteTurn)
            {
              // if the coordindates are in the list of possible moves
              // and doesnt result in check for the SAME color
              // then move. would be grid_y, grid_x
              if (containsPoint(grid_y, grid_x, possible_moves)) {
                move(clicked.value().x, clicked.value().y, grid_y, grid_x);
                _state.isWhiteTurn = !_state.isWhiteTurn;
                _has_render_auth = true;
                possible_moves.clear();
              }
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
 * - find and display the possible moves, return the list of coordinates
 *   that are valid for the piece to move to
 *****************************************************************************/
std::vector<Chess::Game::Point> Chess::Game::renderPossible(Piece p)
{
 SDL_RenderClear(_renderer);

 renderBackground();

  // render all peices
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (_board[i][j]) {
        auto* txture = loadTexture(_board[i][j].Icon());
        if (txture) {
          renderPiece(txture, _board[i][j]);
        }
      }
    }
   }

 // Display what we copied into the renderer
 auto* texture = loadTexture("resources/circle.png");
 std::vector<Point> possible_moves;
 if (texture) {
  switch(p.type){
    case PAWN:
    {
        SDL_Rect src = {0,0, 180, 180};
        auto mod = p.color == WHITE ? 1 : -1;
        // if there isnt a piece in front of the pawn, its possible
        if (!_board[p.x - mod][p.y]) {
	        SDL_Rect dest = { 
                    _screenW / ROWS * p.y + 30 , 
					          _screenH / COLS * (p.x - mod)  + 30,
					          20,
					          20};
	        SDL_RenderCopy(_renderer, texture, &src, &dest);
          possible_moves.push_back(Point{p.x - mod, p.y});
        }

        if (_board[p.x - mod][p.y - mod] && p.color != _board[p.x-mod][p.y-mod].color) {
	        SDL_Rect dest = { 
                    _screenW / ROWS * (p.y - mod) + 30, 
					          _screenH / COLS * (p.x - mod)  + 30,
					          20,
					          20};
	        SDL_RenderCopy(_renderer, texture, &src, &dest);
          possible_moves.push_back(Point{p.x - mod, p.y - mod});
        }

        if (_board[p.x - mod][p.y + mod] && p.Color() != _board[p.x-mod][p.y+mod].Color()) {
	        SDL_Rect dest = { 
                    _screenW / ROWS * (p.y + mod) + 30, 
					          _screenH / COLS * (p.x - mod) + 30,
					          20,
					          20};
	        SDL_RenderCopy(_renderer, texture, &src, &dest);
          possible_moves.push_back(Point{p.x - mod, p.y + mod});
        }

        if (!p.has_moved && !_board[p.x - mod*2][p.y]) {
	        SDL_Rect dest2 = { 
                    _screenW / ROWS * p.y + 30 , 
					          _screenH / COLS * (p.x - mod*2)  + 30,
					          20,
					          20};
	        SDL_RenderCopy(_renderer, texture, &src, &dest2);
          possible_moves.push_back(Point{p.x - mod*2, p.y});
        }
      break;
    }
    case ROOK:
    {
      break;
    }
    case KING:
    {
      break;
    }
    case KNIGHT:
    {
      break;
    }
    case BISHOP:
    {
      break;
    }
    case QUEEN:
    {
      break;
    }
    default:
      break;
  }

  SDL_RenderPresent(_renderer);
 }

 return possible_moves;
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
          auto* txture = loadTexture(_board[i][j].Icon());
          if (txture) {
            renderPiece(txture, _board[i][j]);
          }
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
bool Chess::Game::move(int from_x, int from_y, int to_x, int to_y)
{
  std::cout << "Moving { " << from_x << " , " << from_y << "}\n"
            << "to { " << to_x << " , " << to_y << "}\n";

  // check if the piece can move in this way
  if (_board[from_x][from_y].isValidMove(to_x, to_y)) {
    if (!resultsInCheck(from_x, from_y, to_x, to_y)) {
      //copy constructor desperately needed
      _board[to_x][to_y].x = to_x;
      _board[to_x][to_y].y = to_y;
      _board[to_x][to_y].type = _board[from_x][from_y].type;
      _board[to_x][to_y].color = _board[from_x][from_y].color;
      _board[to_x][to_y].icon = _board[from_x][from_y].icon;
      _board[to_x][to_y].has_moved = true; 

      _board[from_x][from_y].Clear();
      std::cout << "VALID\n";
      return true;
    }
  }
  std::cout << "**INVALID\n";
  return false;
}

/******************************************************************************
 *
 * Method: Game::resultsInCheck()
 *
 *****************************************************************************/
bool Chess::Game::resultsInCheck(int from_x, int from_y, int to_x, int to_y)
{
  return false;
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
  for (auto point : possible) {
    if (point.x == x && point.y == y) {
      return true;
    }
  }
  return false;
}
