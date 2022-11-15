#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "airSpace.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height);
  ~Renderer();

  void Render(AirSpace const &airSpace);
  void UpdateWindowTitle(int score, int fps);

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  // Variables to load textures
  SDL_Surface *loadingSurface; // temporary surface to load an image into the main memory
  SDL_Texture* LoadTexture(std::string path);

  // Texture paths and pointers
  SDL_Texture* backgroundTexture;
  std::string backgroundTexturePath = "../assets/backgroundMoon.bmp";
  SDL_Texture* missileTexture;
  std::string missileTexturePath = "../assets/missile.bmp";
  SDL_Texture* targetterTexture;
  std::string targetterTexturePath = "../assets/targetter.bmp";
  std::vector<SDL_Texture*> cityTextures;
    std::vector<std::string> cityTexturePaths{
      "../assets/NewYork.bmp",
      "../assets/Paris.bmp",
      "../assets/Shanghai.bmp",
      "../assets/Singapore.bmp",
      "../assets/London.bmp",
      "../assets/KualaLumpur.bmp",
      "../assets/Dubai.bmp",
      "../assets/Barcelona.bmp",
      "../assets/Bangkok.bmp"};
  SDL_Texture* airBlastTexture;
  std::string airBlastTexturePath = "../assets/explosion.bmp";
  SDL_Texture* landDetTexture;
  std::string landDetTexturePath = "../assets/nuke.bmp";
  SDL_Texture* ruinedCityTexture;
  std::string ruinedCityTexturePath = "../assets/ruinedCity.bmp";

  const std::size_t screen_width;
  const std::size_t screen_height;
};

#endif