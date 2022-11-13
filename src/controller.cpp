#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "airSpace.h"

void Controller::HandleInput(bool &running, AirSpace &airSpace) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN)
    {
      airSpace.fire = true;
      if (!airSpace.missile.health) {
        SDL_GetMouseState(&airSpace.missile.target.x, &airSpace.missile.target.y); // Update target if a new missile will be launched.
      }
    }
    else if (e.type == SDL_MOUSEBUTTONUP)
    {
      airSpace.fire = false;
    }
  }

  SDL_GetMouseState(&airSpace.mouseCursorPos.x, &airSpace.mouseCursorPos.y);

}