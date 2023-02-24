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

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );

  _window = SDL_CreateWindow("Chess",
              SDL_WINDOWPOS_CENTERED,
              SDL_WINDOWPOS_CENTERED,
              _screenW,
              _screenH,
              SDL_WINDOW_SHOWN);


  _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
  auto* surface = IMG_Load("resources/circle.png");

  if (surface) {
    _circleTexture = SDL_CreateTextureFromSurface(_renderer, surface);
  }

  if (!_window || !_renderer ) {
    std::cout << "ERROR IN INIT \n";
  }

  _move_sound = Mix_LoadWAV("resources/place_down.wav"); 

  // something is not working right with the win sound
  _win_sound = Mix_LoadWAV("resources/game_lose.wav");

  // doing this here saves me a couple MB of ram
  p_textures.insert({"resources/pawn_white.png", loadTexture("resources/pawn_white.png") });
  p_textures.insert({"resources/pawn_black.png", loadTexture("resources/pawn_black.png") });

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
  Mix_FreeChunk(_move_sound);
  Mix_FreeChunk(_win_sound);
  IMG_Quit();
  Mix_Quit();
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
bool Chess::Game::isCheckmate()
{
  const auto cur_player_color = _state.isWhiteTurn ? WHITE : BLACK;
  
  // check if all the possible moves for the current player result in check
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      auto piece = _board[i][j];
      if (piece && piece.Color() == cur_player_color) {
        auto this_piece_possible = generatePossible(piece, _board);
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
 *
 * Method: Game::renderPiece()
 *
 *****************************************************************************/
void Chess::Game::renderPiece(SDL_Texture *texture, Piece p)
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

    // render the first frame
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
                Mix_PlayChannel( 0, _move_sound, 0 );
                
                _move_count++;
                _state.isWhiteTurn = !_state.isWhiteTurn;
                display();
                // check for checkmate
                if (isCheckmate()) {
                  Mix_PlayChannel(1, _win_sound, 0);
                  SDL_Delay(3000);
                  reset();
                  Mix_PlayChannel(0, _move_sound, 0);
                }

                _possible_moves.clear();

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

     {
       // castling
       // if the king hasnt moved
       if (!p.has_moved) {
         // if im not already in check
         if (!resultsInCheck(p.x, p.y, p.x, p.y)) {
           if (board[p.x][p.y + 3] &&
               board[p.x][p.y + 3].type == ROOK &&
               !board[p.x][p.y + 3].has_moved)
           {
             // if traveling to either of the next to squares dont result in check
             if (!board[p.x][p.y + 1] && !resultsInCheck(p.x, p.y, p.x, p.y + 1)) {
               if (!board[p.x][p.y + 2] && !resultsInCheck(p.x, p.y, p.x, p.y + 2)) {
                 possible.push_back(Point {p.x, p.y + 2});
               }
             }
           }
         }

         // queen side castle
         // if im not already in check
         if (!resultsInCheck(p.x, p.y, p.x, p.y)) {
           if (board[p.x][p.y - 4] &&
               board[p.x][p.y - 4].type == ROOK &&
               !board[p.x][p.y - 4].has_moved)
           {
             // if traveling to either of the next to squares dont result in check
             if (!board[p.x][p.y - 1] && !resultsInCheck(p.x, p.y, p.x, p.y - 1)) {
               if (!board[p.x][p.y - 2] && !resultsInCheck(p.x, p.y, p.x, p.y - 2)) {
                 if (!board[p.x][p.y - 3]) {
                   possible.push_back(Point {p.x, p.y - 2});
                 }
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
  if (from_x == to_x && from_y == to_y) {
    return;
  }
  
  auto dx = abs(to_x - from_x);
  auto dy = abs(to_y - from_y);
  bool is_castle = dy == 2 && board[from_x][from_y].type == KING;

  // copy constructor desperately needed
  board[to_x][to_y].x = to_x;
  board[to_x][to_y].y = to_y;
  board[to_x][to_y].type = board[from_x][from_y].type;
  board[to_x][to_y].color = board[from_x][from_y].color;
  board[to_x][to_y].icon = board[from_x][from_y].icon;
  board[to_x][to_y].has_moved = true; 
  board[from_x][from_y].Clear();

  
  if (is_castle) {
    auto dx = to_x - from_x;
    auto dy = to_y - from_y;
    auto king_x = to_x;
    auto king_y = to_y;

    // if the difference in x is positive, its a king side castle
    // generatePossible will handle if this has happened before
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
        if (local[i][j].type != KING) {
          auto piecePossible = generatePossible(local[i][j], local);
          possible.insert(std::end(possible),
                          std::begin(piecePossible),
                          std::end(piecePossible));
        }
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
void Chess::Game::reset()
{
  _state.isWhiteTurn = true;
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
  display();
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
