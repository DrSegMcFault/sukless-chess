#pragma once

#include <map>
#include "common_enums.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "BoardManager.h"
#include "AI.h"

class App {
  public:
    App();
    ~App();
    void run();
    void simulate();

  private:
    const int _screenW = 720;
    const int _screenH = 720;

    SDL_Window* _window;
    SDL_Renderer* _renderer;
    SDL_Texture* _circleTexture;
    Mix_Chunk* _move_sound;
    Mix_Chunk* _win_sound;
    Mix_Chunk* _lose_sound;
    std::map<std::string, SDL_Texture*> p_textures;

    std::vector<Move> _possible_moves;

    enum AppState {
      PLAY = 0,
      EXIT = 1
    };

    AppState _state;

    BoardManager* _game;
    AI* _ai;

    using Board = std::vector<std::vector<Piece>>;

    SDL_Texture* loadTexture(const char* filepath);

    void display();
    void displayBoard(const Board& p);
    void displayPossible();
    void renderBackground();
    void renderPiece(SDL_Texture* txture, Piece p);
    void renderAllPieces();
};
