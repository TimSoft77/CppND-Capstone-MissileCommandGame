#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
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
  loadingSurface = SDL_LoadBMP("../assets/missileBase.bmp");
  if (!loadingSurface) // Let the user know if the file failed to load
  {
    SDL_Log("Failed to load image at %s: %s\n", "assets/missileBase.bmp", SDL_GetError());
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
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(Snake const snake, SDL_Point const &food) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 64, 64, 64, 0xFF);
  SDL_RenderClear(sdl_renderer);

 // Render background
  SDL_RenderCopy(sdl_renderer, backgroundTexture, NULL, NULL);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

    SDL_Rect dstrect;
  // Render missile
  if (snake.missile.health)
  {
    dstrect.w = 32;
    dstrect.h = 32;
    dstrect.x = snake.missile.position.x - 16;
    dstrect.y = snake.missile.position.y - 16;
    SDL_RenderCopy(sdl_renderer, missileTexture, NULL, &dstrect);
  }

  // Render Targetter
  dstrect.w = 64;
  dstrect.h = 64;
  dstrect.x = snake.mouseCursorPos.x - 32;
  dstrect.y = snake.mouseCursorPos.y - 32;
  SDL_RenderCopy(sdl_renderer, targetterTexture, NULL, &dstrect);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
