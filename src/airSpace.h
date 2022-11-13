#ifndef AIRSPACE_H
#define AIRSPACE_H

#include <vector>
#include "SDL.h"

class Missile {
  public:
    Missile(SDL_Point t);
    SDL_Point position;
    bool hasReachedTarget = false;
    double angle;
    void Move();
  private:
    int flightTimeElapsed = 0; // the number of frames since the missile's creation
    int speed = 4;
    double flightDuration; // The number of frames the missile will take to reach its target
    SDL_Point initPosition;
    SDL_Point target;
};

class AirSpace {
 public:
  void Update();
  void LaunchMissile(SDL_Point t);

  SDL_Point mouseCursorPos;
  std::vector<Missile> missiles;

 private:

};

#endif