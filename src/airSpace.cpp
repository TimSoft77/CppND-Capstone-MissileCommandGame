#include "airSpace.h"
#include <cmath>
#include <iostream>
#include <algorithm>

void AirSpace::Update() {
  for (Missile &missile : missiles) {
    missile.Move();
  }
    for (Missile missile : missiles) {
  }
  // Remove missiles that have reached their target
  // Algorithm per https://stackoverflow.com/questions/8628951/remove-elements-of-a-vector-inside-the-loop
  missiles.erase(std::remove_if(missiles.begin(), missiles.end(), [](Missile const & m) { return m.hasReachedTarget;}), missiles.end());
}

void AirSpace::LaunchMissile(SDL_Point target) {
  missiles.emplace_back(Missile(target));
}

// Creates a missile with a target, and calculates its path of flight.
Missile::Missile(SDL_Point t) {
  this->target = t;
  this->position.x = 320; // TODO don't hardcode base location here
  this->position.y = 640;
  initPosition = position;
  this->angle = atan2(target.x - position.x, position.y - target.y)*180/M_PI;
  // x and y deltas are flipped to get the angle away from the positive vertical axis.
  // Delta-y calc is backwards to handle the flipped vertical axis used by SDL.
  flightDuration = sqrt(pow(initPosition.x - target.x, 2) + pow(initPosition.y - target.y, 2));
}

// Adjust a missile's position based on its flight time.
// Incrementing position based on dx,dy values did not work 
// (missiles did not proceed to target at low speeds for certain angles).
void Missile::Move() {
  if (flightTimeElapsed < flightDuration) {
    position.x = flightTimeElapsed/flightDuration*(target.x - initPosition.x) + initPosition.x;
    position.y = flightTimeElapsed/flightDuration*(target.y - initPosition.y) + initPosition.y;
    flightTimeElapsed++;
  } else {
    position = target;
    hasReachedTarget = true;
  }

}
