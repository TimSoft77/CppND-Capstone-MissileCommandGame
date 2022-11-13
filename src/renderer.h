#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "airSpace.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height);
  ~Renderer();

  void Render(AirSpace const airSpace);
  void UpdateWindowTitle(int score, int fps);
  SDL_Texture* backgroundTexture;
  SDL_Texture* missileTexture;
  SDL_Texture* targetterTexture;

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
};

#endif