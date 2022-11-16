#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main(int argv, char** args) { // Remove arguments for Linux
  SDL_Log("Start");
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};

  Renderer renderer(K_SCREEN_WIDTH, K_SCREEN_HEIGHT);
  Controller controller;
  Game game;
  game.Run(controller, renderer, kMsPerFrame);
  SDL_Log("Game has terminated successfully!");
  SDL_Log("Score: %d", game.GetScore());
  return 0;
}