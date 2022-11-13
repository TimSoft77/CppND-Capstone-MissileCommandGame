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
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight);
  game.Run(controller, renderer, kMsPerFrame);
  SDL_Log("Game has terminated successfully!");
  SDL_Log("Score: %d", game.GetScore());
  SDL_Log("Size: %d", game.GetSize());
  return 0;
}