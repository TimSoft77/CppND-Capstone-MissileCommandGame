#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main(int argv, char** args) { // Remove arguments for Linux
  SDL_Log("Start");
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};

  Renderer renderer(kScreenWidth, kScreenHeight);
  Controller controller;
  Game game;
  game.Run(controller, renderer, kMsPerFrame);
  SDL_Log("Game has terminated successfully!");
  SDL_Log("Score: %d", game.GetScore());
  return 0;
}