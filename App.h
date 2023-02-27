#pragma once

#include <map>
#include "common_enums.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "Game.h"
#include "AI.h"

class App {
  public:
    App();
    ~App();
    void run();
    void simulate();

  private:
    const int _screenW = 640;
    const int _screenH = 640;
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    std::map<std::string, SDL_Texture*> p_textures; 
    SDL_Texture* _circleTexture;
    Mix_Chunk* _move_sound;
    Mix_Chunk* _win_sound;
    std::vector<Move> _possible_moves;
    AI* _ai;

    enum AppState {
      PLAY = 0,
      EXIT = 1
    };

    AppState _state;

    Game* _game;

    void renderPiece(SDL_Texture* txture, Piece p);
    void renderAllPieces();
    void displayBoard(Game::Board p);

    SDL_Texture* loadTexture(const char* filepath);
    void display();
    void renderBackground();
    void displayPossible();
};
