#pragma once

#include <vector>
#include <map>
#include "Piece.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

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

    uint8_t _move_count = 0;
    
    std::map<std::string, SDL_Texture*> p_textures; 
    
    SDL_Texture* _circleTexture;
    Mix_Chunk* _move_sound;
    Mix_Chunk* _win_sound;

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

    void move(int from_x, int from_y, int to_x, int to_y, Piece (&b)[8][8]);

    void display();

    // helper functions
    bool isCheckmate();
    void renderPiece(SDL_Texture* txture, Piece p);
    SDL_Texture* loadTexture(const char* filepath);
    bool resultsInCheck(int from_x, int from_y, int to_x, int to_y);
    void renderBackground();
    std::vector<Point> generatePossible(Piece p, Piece (&b)[8][8]);
    void renderPossible();

    constexpr bool validPoint(int x, int y) const {
      return (x >= 0 && x < 8) && (y >=0 && y < 8);
    }
    void reset();
    bool containsPoint(int x, int y, std::vector<Point> possible);
    std::vector<Point> rookPossible(Piece p, Piece (&b)[8][8]);
    std::vector<Point> bishopPossible(Piece p, Piece (&b)[8][8]);
};

} // namespace chess
