#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height)
    : screen_width(screen_width),
      screen_height(screen_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Missile Command Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Hide the cursor
  SDL_ShowCursor(SDL_DISABLE);

  // Load textures
  SDL_Surface *loadingSurface; // temporary surface to load an image into the main memory

  // Load background
  loadingSurface = SDL_LoadBMP("../assets/backgroundMoon.bmp");
  if (!loadingSurface) // Let the user know if the file failed to load
  {
    SDL_Log("Failed to load image at %s: %s\n", "assets/backgroundMoon.bmp", SDL_GetError());
    return;
  }
  // loads image to our graphics hardware memory.
  backgroundTexture = SDL_CreateTextureFromSurface(sdl_renderer, loadingSurface);
  SDL_FreeSurface(loadingSurface);

  // Load missile
  loadingSurface = SDL_LoadBMP("../assets/missile.bmp");
  if (!loadingSurface) // Let the user know if the file failed to load
  {
    SDL_Log("Failed to load image at %s: %s\n", "../assets/missile.bmp", SDL_GetError());
    return;
  }
  // loads image to our graphics hardware memory.
  missileTexture = SDL_CreateTextureFromSurface(sdl_renderer, loadingSurface);
  SDL_FreeSurface(loadingSurface);

  // Load targetter
  // Load missile
  loadingSurface = SDL_LoadBMP("../assets/targetter.bmp");
  if (!loadingSurface) // Let the user know if the file failed to load
  {
    SDL_Log("Failed to load image at %s: %s\n", "../assets/targetter.bmp", SDL_GetError());
    return;
  }
  // loads image to our graphics hardware memory.
  targetterTexture = SDL_CreateTextureFromSurface(sdl_renderer, loadingSurface);
  SDL_FreeSurface(loadingSurface);

  // Load cities
  std::vector<std::string> cityPaths{
      "../assets/NewYork.bmp",
      "../assets/Paris.bmp",
      "../assets/Shanghai.bmp",
      "../assets/Singapore.bmp",
      "../assets/London.bmp",
      "../assets/KualaLumpur.bmp",
      "../assets/Dubai.bmp",
      "../assets/Barcelona.bmp",
      "../assets/Bangkok.bmp"};

  for (std::string path : cityPaths)
  {
    loadingSurface = SDL_LoadBMP(&path[0]);
    if (!loadingSurface) // Let the user know if the file failed to load
    {
      SDL_Log("Failed to load image at %s: %s\n", path, SDL_GetError());
      return;
    }
    // loads image to our graphics hardware memory.
    cityTextures.emplace_back(SDL_CreateTextureFromSurface(sdl_renderer, loadingSurface));
    SDL_FreeSurface(loadingSurface);
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(AirSpace const &airSpace) {
  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 64, 64, 64, 0xFF);
  SDL_RenderClear(sdl_renderer);

 // Render background
  SDL_RenderCopy(sdl_renderer, backgroundTexture, NULL, NULL);

// Initialize a rectangle, to be used for all renders
    SDL_Rect dstrect;

  // Render cities
  int i = 0;
  for (City const &city : airSpace.cities) {
    dstrect.w = 100;
    dstrect.h = 100;
    dstrect.x = city.position.x - 50;
    dstrect.y = city.position.y - 100;
    SDL_RenderCopy(sdl_renderer, cityTextures[i], NULL, &dstrect);
    i++;
    // Loops back to the first texture if there are more cities than textures
    if (i >= cityTextures.size()) { i = 0;}
  }

  // Render missiles
  for (auto const& missile : airSpace.missiles) {
    dstrect.w = 32;
    dstrect.h = 32;
    dstrect.x = missile->position.x - 16;
    dstrect.y = missile->position.y - 16;
    SDL_RenderCopyEx(sdl_renderer, missileTexture, NULL, &dstrect, missile->angle, NULL, SDL_FLIP_NONE);
  }
  
  // Render Targetter
  dstrect.w = 64;
  dstrect.h = 64;
  dstrect.x = airSpace.mouseCursorPos.x - 32;
  dstrect.y = airSpace.mouseCursorPos.y - 32;
  SDL_RenderCopy(sdl_renderer, targetterTexture, NULL, &dstrect);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Missile Command Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
