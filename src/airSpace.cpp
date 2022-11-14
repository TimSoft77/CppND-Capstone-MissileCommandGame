#include "airSpace.h"
#include <cmath>
#include <iostream>
#include <algorithm>

// Calculate the Euclidean distance between two points in 2d space
float distance(SDL_Point const &p1, SDL_Point const &p2) {
  return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

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

void AirSpace::Update()
{
  // Move missiles in the airSpace
  for (auto &missile : missiles)
  {
    missile->Move();
  }

  // Introduce hostile missiles
  if (randomProp(engine) < missileGenProb && missiles.size() < maxMissiles)
  {
    missiles.emplace_back(std::make_unique<HostileMissile>(random_w(engine), random_w(engine)));
  }

  // Resolve missiles that have detonated
  for (std::unique_ptr<Missile> &missile : missiles)
  {
    if (missile->state == DETONATED)
    {
      for (auto &otherMissile : missiles)
      {
        if (otherMissile->state == FLIGHT)
        { // Saves computations and avoids the missile measuring distance to itself
          if (distance(missile->position, otherMissile->position) < missile->blastRadius)
          {
            otherMissile->state = SHOT_DOWN;
          }
        }
      }
      for (City &city : cities)
      {
        if (distance(missile->position, city.position) < missile->blastRadius)
        {
          city.isAlive = false;
        }
      }
      // Set the detonation type
      if (missile->position.y > 640 - missile->blastRadius)
      {
        missile->state = LAND_DET;
      }
      else
      {
        missile->state = AIR_BLAST;
      }
    }
  }

  // Remove missiles that have reached their target
  // Algorithm per https://stackoverflow.com/questions/8628951/remove-elements-of-a-vector-inside-the-loop
  missiles.erase(std::remove_if(missiles.begin(), missiles.end(), [&](std::unique_ptr<Missile> const &m)
                                { return (m->state == GONE); }),
                 missiles.end());
}

// Launch a missile, provided any cities are left
void AirSpace::LaunchMissile(SDL_Point target)
{
  if (CountSurvivingCities() > 0)
  {
    missiles.emplace_back(std::make_unique<DefensiveMissile>(target));
  }
}

int AirSpace::CountSurvivingCities() {
  return std::count_if(cities.begin(), cities.end(), [&](auto const &c) {return c.isAlive;});
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
  flightDuration = distance(initPosition, target)/speed;
}

// Adjust a missile's position based on its flight time.
// Incrementing position based on dx,dy values did not work 
// (missiles did not proceed to target at low speeds for certain angles).
void DefensiveMissile::Move() {
  if (state == FLIGHT && flightTimeElapsed < flightDuration) {
    position.x = flightTimeElapsed/flightDuration*(target.x - initPosition.x) + initPosition.x;
    position.y = flightTimeElapsed/flightDuration*(target.y - initPosition.y) + initPosition.y;
    flightTimeElapsed++;
  } else if (state == FLIGHT) {
    position = target;
    state = DETONATED;
  } else  if (state != GONE) {
      cloudCounter++;
    if (cloudCounter > cloudResideTime) {
      state = GONE;
    }
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
  flightDuration = distance(initPosition, target)/speed;
}

// Adjust a missile's position based on its flight time.
// Incrementing position based on dx,dy values did not work 
// (missiles did not proceed to target at low speeds for certain angles).
void HostileMissile::Move() {
  if (state == FLIGHT && flightTimeElapsed < flightDuration) {
    position.x = flightTimeElapsed/flightDuration*(target.x - initPosition.x) + initPosition.x;
    position.y = flightTimeElapsed/flightDuration*(target.y - initPosition.y) + initPosition.y;
    flightTimeElapsed++;
  } else if (state == FLIGHT) {
    position = target;
    state = DETONATED;
  } else  if (state != GONE) {
      cloudCounter++;
    if (cloudCounter > cloudResideTime) {
      state = GONE;
    }
  }
}

// Initialize a city at a specified location
City::City(int x, int y) {
  position.x = x;
  position.y = y;
}