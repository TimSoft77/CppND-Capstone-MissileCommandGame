#ifndef AIRSPACE_H
#define AIRSPACE_H

#include <vector>
#include "SDL.h"

class Missile {
  public:
    SDL_Point position;
    bool health = false;
    int speed = -16;
    SDL_Point target;
};

class AirSpace {
 public:
  void Update();

  bool fire = false;
  Missile missile;
  SDL_Point mouseCursorPos;

 private:

};

#endif