#pragma once

#include <vector>
#include <map>
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
    
    std::map<std::string, SDL_Texture*> p_textures; 
    
    SDL_Texture* _circleTexture;

    struct State {
      GameState game = GameState::PLAY;
      bool isWhiteTurn = true; 
    };

    struct Point {
      int x = 0;
      int y = 0;
    };

    std::vector<Point> _possible_moves;

    State _state;

    bool _has_render_auth = true;

    bool move(int from_x, int from_y, int to_x, int to_y);

    void display();

    // helper functions
    void renderPiece(SDL_Texture* txture, Piece p);
    SDL_Texture* loadTexture(const char* filepath);
    bool resultsInCheck(int from_x, int from_y, int to_x, int to_y);
    void renderBackground();
    void renderPossible(Piece p);
    void printBoard();
    const bool validPoint(int x, int y);
    bool containsPoint(int x, int y, std::vector<Point> possible);
    void rookPossible(Piece p);
    void bishopPossible(Piece p);
};

} // namespace chess
