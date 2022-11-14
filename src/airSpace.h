#ifndef AIRSPACE_H
#define AIRSPACE_H

#include <vector>
#include "SDL.h"
#include <memory>

class Missile {
  public:
    virtual void Move() = 0;
    SDL_Point position;
    bool hasReachedTarget = false;
    double angle;
  protected:
    int speed = 4;
    SDL_Point initPosition;
    SDL_Point target;
};

class DefensiveMissile : public Missile {
  public:
    DefensiveMissile(SDL_Point t);
    void Move();
  private:
    int flightTimeElapsed = 0; // the number of frames since the missile's creation
    double flightDuration; // The number of frames the missile will take to reach its target

};

class AirSpace {
 public:
  void Update();
  void LaunchMissile(SDL_Point t);

  SDL_Point mouseCursorPos;
  std::vector<std::unique_ptr<Missile>> missiles;

 private:

};

#endif