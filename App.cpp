#include "App.h"
#include <optional>
#include <iostream>

/******************************************************************************
 *
 * Method: App::App()
 *
 *****************************************************************************/
Chess::App::App() 
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

  p_textures.insert({"resources/pawn_white.png", loadTexture("resources/pawn_white.png") });
  p_textures.insert({"resources/pawn_black.png", loadTexture("resources/pawn_black.png") });
  p_textures.insert({"resources/king_white.png", loadTexture("resources/king_white.png") });
  p_textures.insert({"resources/king_black.png", loadTexture("resources/king_black.png") });
  p_textures.insert({"resources/queen_white.png", loadTexture("resources/queen_white.png") });
  p_textures.insert({"resources/queen_black.png", loadTexture("resources/queen_black.png") });
  p_textures.insert({"resources/rook_white.png", loadTexture("resources/rook_white.png") });
  p_textures.insert({"resources/rook_black.png", loadTexture("resources/rook_black.png") });
  p_textures.insert({"resources/bishop_white.png", loadTexture("resources/bishop_white.png") });
  p_textures.insert({"resources/bishop_black.png", loadTexture("resources/bishop_black.png") });
  p_textures.insert({"resources/knight_white.png", loadTexture("resources/knight_white.png") });
  p_textures.insert({"resources/knight_black.png", loadTexture("resources/knight_black.png") });

  _game = new Game();
  _ai = new ChessAI(Color::BLACK, ChessAI::EASY);
}

/******************************************************************************
 *
 * Method: App::run()
 *
 *****************************************************************************/
void Chess::App::run()
{
  // cached piece that has been previously clicked
  std::optional<Piece> clicked;
  // is_first_frame
  bool first = true;

  // main loop
  // we only render if there is a click on the screen for performance
  while (_state != AppState::EXIT) {

    // render the first frame
    if (first) {
      display();
      auto b = _game->getBoard();
      first = false;
    }

    SDL_Event ev;
    
    SDL_WaitEvent(&ev);

    switch (ev.type) {
      case SDL_QUIT:
        _state = AppState::EXIT;
        break;

      case SDL_MOUSEBUTTONDOWN:
      {
        display();
        auto x = ev.button.x;
        auto y = ev.button.y;
        int grid_x = floor(x / (_screenH / 8));
        int grid_y = floor(y / (_screenW / 8));

        // get the piece that was clicked on
        if (!clicked.has_value()) {
          if (auto this_piece = _game->pieceAt(grid_y, grid_x);
              this_piece)
          { 
            if (_game->colorMatchesTurn(this_piece.Color()))
            {
              clicked = this_piece;
              _possible_moves = _game->genThisPossible(this_piece);
              displayPossible();
            }
          }

        } else {

          if (_game->colorMatchesTurn(clicked.value().Color()))
          {
            Chess::Move m = { Point {clicked.value().x, clicked.value().y },
                              Point {grid_y, grid_x} };
            // if the move took place
            if (_game->move(m, _possible_moves))
            {
              display();

              Mix_PlayChannel( 0, _move_sound, 0 );
              _possible_moves.clear();

              // check for checkmate
              if (_game->isCheckmate()) {
                Mix_PlayChannel(1, _win_sound, 0);
                SDL_Delay(3000);
                _game->reset();
                first = true;
                Mix_PlayChannel(0, _move_sound, 0);
              }

              SDL_Delay(1000);
              if (_ai->move(_game, _game->genAllPossibleOpposing(Color::WHITE))) {
                display();
                Mix_PlayChannel( 0, _move_sound, 0 );

                // check for checkmate
                if (_game->isCheckmate()) {
                  Mix_PlayChannel(1, _win_sound, 0);
                  SDL_Delay(3000);
                  first = true;
                  _game->reset();
                  Mix_PlayChannel(0, _move_sound, 0);
                }
              }
            }
          }
          clicked.reset(); 
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
 * Method: App::display()
 * 
 * - render and display the background and pieces 
 *****************************************************************************/
void Chess::App::display()
{
   SDL_RenderClear(_renderer);

   renderBackground();
   renderAllPieces();

   SDL_RenderPresent(_renderer);
}

/******************************************************************************
 *
 * Method: App::display()
 * 
 * - render and display the background and pieces 
 *****************************************************************************/
void Chess::App::renderAllPieces() {
  // render all peices
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (auto piece = _game->pieceAt(i, j);
          piece)
      {
        renderPiece(p_textures.at(piece.Icon()), piece);
      }
    }
   }
}

/******************************************************************************
 *
 * Method: App::renderPossible()
 * 
 * - display the possible moves
 *****************************************************************************/
void Chess::App::displayPossible()
{
 SDL_RenderClear(_renderer);

 renderBackground();

 renderAllPieces();
 SDL_Rect src = {0,0, 180, 180};
 
 for (auto move : _possible_moves) {
    auto x = move.to.x;
    auto y = move.to.y;

    SDL_Rect dest = {
              _screenW / 8 * y + 30,
		          _screenH / 8 * x + 30,
			        20,
			        20};
    SDL_RenderCopy(_renderer, _circleTexture, &src, &dest);
 }

 SDL_RenderPresent(_renderer);
}

/******************************************************************************
 *
 * Method: App::display()
 * 
 * - render and display the background
 *****************************************************************************/
void Chess::App::renderBackground()
{
	bool white = true;
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (white) {
				SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
			} else {
				SDL_SetRenderDrawColor(_renderer, 83, 132, 172, 255);
			}
			white = !white;
			SDL_Rect rectangle = { i * _screenW / 8,
								  j * _screenH / 8,
								  _screenW / 8,
								  _screenH / 8 };
			SDL_RenderFillRect(_renderer, &rectangle);
		}
		white = !white;
	}
}

/******************************************************************************
 *
 * Method: App::renderPiece()
 *
 *****************************************************************************/
void Chess::App::renderPiece(SDL_Texture *texture, Piece p)
{
  SDL_Rect src = {0, 0, 80, 80};
	SDL_Rect dest = { 
                    _screenW / 8 * p.y + 5,
					          _screenH / 8 * p.x + 5,
					          _screenW / 8 - 10,
					          _screenH / 8 - 10};
  SDL_RenderCopy(_renderer, texture, &src, &dest);
}

/******************************************************************************
 *
 * Method: App::App()
 *
 *****************************************************************************/
SDL_Texture* Chess::App::loadTexture(const char* filepath)
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
 * Method: App::~App()
 *
 *****************************************************************************/
Chess::App::~App()
{
  SDL_DestroyWindow(_window);
  SDL_DestroyRenderer(_renderer);
  Mix_FreeChunk(_move_sound);
  Mix_FreeChunk(_win_sound);
  IMG_Quit();
  Mix_Quit();
  SDL_Quit();
  delete _game;
}
