#pragma once

#include <vector>

#include "Piece.h"
#include "SDL.h"
#include "SDL_image.h"

namespace Chess {

class Game {
  public: 
    Game();
    ~Game();
    
    void run();

    enum GameState {
      PLAY = 0,
      EXIT = 1
    };
    
  private:

    const int _screenW = 640;
    const int _screenH = 640;
    static const int ROWS = 8;
    static const int COLS = 8;

    SDL_Window* _window;
    SDL_Renderer* _renderer;

    Piece _board[ROWS][COLS];

    struct State {
      GameState game = GameState::PLAY;
      bool isWhiteTurn = true; 
    };

    State _state; 

    void renderPiece(SDL_Texture* txture, Piece p);

    bool move(int from_x, int from_y, int to_x, int to_y);

    void display();

    SDL_Texture* loadTexture(const char* filepath);

    bool valid(int from_x, int from_y, int to_x, int to_y);

    // helper functions
    bool jumpedPiece(int from_x, int from_y, int to_x, int to_y);
    bool resultsInCheck(int from_x, int from_y, int to_x, int to_y);
    void renderBackground();
    void renderPossible(Piece p);
    void printBoard();
};

} // namespace chess
