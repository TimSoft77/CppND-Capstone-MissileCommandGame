#include "renderer.h"
#include <iostream>
#include <string>

SDL_Texture* Renderer::LoadTexture(std::string path)
{
  loadingSurface = SDL_LoadBMP(&path[0]);
  if (!loadingSurface) // Let the user know if the file failed to load
  {
    SDL_Log("Failed to load image at %s: %s\n", path, SDL_GetError());
    return nullptr;
  }
  // loads image to our graphics hardware memory.
  SDL_Texture* texture = SDL_CreateTextureFromSurface(sdl_renderer, loadingSurface);
  SDL_FreeSurface(loadingSurface);
  return texture;
}

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

  // Load background
  backgroundTexture = LoadTexture(backgroundTexturePath);

  // Load targetter
  targetterTexture = LoadTexture(targetterTexturePath);

  // Load missile
  missileTexture = LoadTexture(missileTexturePath);

  // Load explosion
  airBlastTexture = LoadTexture(airBlastTexturePath);

  // Load nuke
  landDetTexture = LoadTexture(landDetTexturePath);

  // Load ruined city
 ruinedCityTexture = LoadTexture(ruinedCityTexturePath);

  // Load cities
  for (std::string path : cityTexturePaths)
  {
    cityTextures.emplace_back(LoadTexture(path));
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
    for (City const &city : airSpace.cities)
    {
      dstrect.w = 100;
      dstrect.h = 100;
      dstrect.x = city.position.x - 50;
      dstrect.y = city.position.y - 100;
      if (city.isAlive)
      {
        SDL_RenderCopy(sdl_renderer, cityTextures[i], NULL, &dstrect);
      }
      else
      {
        SDL_RenderCopy(sdl_renderer, ruinedCityTexture, NULL, &dstrect);
      }
      i++;
      // Loops back to the first texture if there are more cities than textures
      if (i >= cityTextures.size())
      {
        i = 0;
      }
    }

    // Render missiles
    for (auto const &missile : airSpace.missiles)
    {

      switch (missile->state)
      {
      case FLIGHT:
        dstrect.w = 32;
        dstrect.h = 32;
        dstrect.x = missile->position.x - 16;
        dstrect.y = missile->position.y - 16;
        SDL_RenderCopyEx(sdl_renderer, missileTexture, NULL, &dstrect, missile->angle, NULL, SDL_FLIP_NONE);
        break;
      case AIR_BLAST:
        dstrect.w = missile->blastRadius;
        dstrect.h = missile->blastRadius;
        dstrect.x = missile->position.x - missile->blastRadius / 2;
        dstrect.y = missile->position.y - missile->blastRadius / 2;
        SDL_RenderCopy(sdl_renderer, airBlastTexture, NULL, &dstrect);
        break;
      case LAND_DET:
        dstrect.w = missile->blastRadius;
        dstrect.h = missile->blastRadius;
        dstrect.x = missile->position.x - missile->blastRadius / 2;
        dstrect.y = missile->position.y - missile->blastRadius * 0.8; // 0.8 is a hack to make the current landDetTexture show up on the horizon
        SDL_RenderCopy(sdl_renderer, landDetTexture, NULL, &dstrect);
        break;
      case SHOT_DOWN:
        dstrect.w = 32;
        dstrect.h = 32;
        dstrect.x = missile->position.x - 16;
        dstrect.y = missile->position.y - 16;
        SDL_RenderCopy(sdl_renderer, airBlastTexture, NULL, &dstrect);
      }
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
