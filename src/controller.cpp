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
      SDL_Point target;
      SDL_GetMouseState(&target.x, &target.y); // Update target if a new missile will be launched.
      airSpace.LaunchMissile(target);
    }
  }

  SDL_GetMouseState(&airSpace.mouseCursorPos.x, &airSpace.mouseCursorPos.y);

}