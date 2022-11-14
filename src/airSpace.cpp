#include "airSpace.h"
#include <cmath>
#include <iostream>
#include <algorithm>

AirSpace::AirSpace() : engine(dev()),
                       random_w(0, static_cast<int>(640 - 1)), // TODO These shouldn't be hard-coded
                       randomProp(0.0, 1.0)
{
  // Creating cities.  Locations are hard-coded because I don't necessarily want to space them to a pattern.
  cities.emplace_back(City(80, 640 - 1));
  cities.emplace_back(City(220, 640 - 1));
  cities.emplace_back(City(420, 640 - 1));
  cities.emplace_back(City(560, 640 - 1));
}

void AirSpace::Update() {
  // Move missiles in the airSpace
  for (auto& missile : missiles) {
    missile->Move();
  }

  // Introduce hostile missiles
  if (randomProp(engine) < missileGenProb && missiles.size() < maxMissiles) {
    missiles.emplace_back(std::make_unique<HostileMissile>(random_w(engine), random_w(engine)));
  }

  // Remove missiles that have reached their target
  // Algorithm per https://stackoverflow.com/questions/8628951/remove-elements-of-a-vector-inside-the-loop
  missiles.erase(std::remove_if(missiles.begin(), missiles.end(), [](auto const & m) { return m->hasReachedTarget;}), missiles.end());
}

void AirSpace::LaunchMissile(SDL_Point target) {
  missiles.emplace_back(std::make_unique<DefensiveMissile>(target));
}

// Creates a missile with a target, and calculates its path of flight.
DefensiveMissile::DefensiveMissile(SDL_Point t) {
  target = t;
  position.x = 320; // TODO don't hardcode base location here
  position.y = 640;
  initPosition = position;
  angle = atan2(target.x - position.x, position.y - target.y)*180/M_PI;
  // x and y deltas are flipped to get the angle away from the positive vertical axis.
  // Delta-y calc is backwards to handle the flipped vertical axis used by SDL.
  flightDuration = sqrt(pow(initPosition.x - target.x, 2) + pow(initPosition.y - target.y, 2));
}

// Adjust a missile's position based on its flight time.
// Incrementing position based on dx,dy values did not work 
// (missiles did not proceed to target at low speeds for certain angles).
void DefensiveMissile::Move() {
  if (flightTimeElapsed < flightDuration) {
    position.x = flightTimeElapsed/flightDuration*(target.x - initPosition.x) + initPosition.x;
    position.y = flightTimeElapsed/flightDuration*(target.y - initPosition.y) + initPosition.y;
    flightTimeElapsed++;
  } else {
    position = target;
    hasReachedTarget = true;
  }
}

// Creates a hostile missile with a start and target point
HostileMissile::HostileMissile(int start_x, int target_x) {
  initPosition.x = start_x;
  initPosition.y = 0;
  position = initPosition;
  target.x = target_x;
  target.y = 640;
  angle = atan2(target.x - position.x, position.y - target.y)*180/M_PI;
  // x and y deltas are flipped to get the angle away from the positive vertical axis.
  // Delta-y calc is backwards to handle the flipped vertical axis used by SDL.
  flightDuration = sqrt(pow(initPosition.x - target.x, 2) + pow(initPosition.y - target.y, 2));
}

// Adjust a missile's position based on its flight time.
// Incrementing position based on dx,dy values did not work 
// (missiles did not proceed to target at low speeds for certain angles).
void HostileMissile::Move() {
  if (flightTimeElapsed < flightDuration) {
    position.x = flightTimeElapsed/flightDuration*(target.x - initPosition.x) + initPosition.x;
    position.y = flightTimeElapsed/flightDuration*(target.y - initPosition.y) + initPosition.y;
    flightTimeElapsed++;
  } else {
    position = target;
    hasReachedTarget = true;
  }
}

// Initialize a city at a specified location
City::City(int x, int y) {
  position.x = x;
  position.y = y;
}