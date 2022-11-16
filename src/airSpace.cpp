#include "airSpace.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <thread>
#include <future>

// Calculate the Euclidean distance between two points in 2d space
float distance(SDL_Point const &p1, SDL_Point const &p2)
{
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

  // Allow smart missiles to react
  // Parallelized to account for potential for future AI.
  std::vector<std::future<void>> futures;
  for (auto &missile : missiles)
  {

    futures.emplace_back(std::async(&Missile::React, missile, missiles));
  }
  for (const std::future<void> &ftr : futures)
    ftr.wait();

  // Move missiles in the airSpace
  for (auto &missile : missiles)
  {
    missile->Move();
  }

  // Introduce hostile missiles
  if (randomProp(engine) < missileGenProb && missiles.size() < maxMissiles)
  {
    if (randomProp(engine) < smartMissileGenProb)
    {
      missiles.emplace_back(std::make_shared<SmartMissile>(random_w(engine), random_w(engine)));
    }
    else
    {
      missiles.emplace_back(std::make_shared<HostileMissile>(random_w(engine), random_w(engine)));
    }
  }

  // Resolve missiles that have detonated
  for (auto &missile : missiles)
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
        missile->position.y = 640 - 1; // Draw land detonations on the horizon TODO should not be hardcoded
      }
      else
      {
        missile->state = AIR_BLAST;
      }
    }
  }

  // Remove missiles that have reached their target
  // Algorithm per https://stackoverflow.com/questions/8628951/remove-elements-of-a-vector-inside-the-loop
  missiles.erase(std::remove_if(missiles.begin(), missiles.end(), [&](auto const &m)
                                { return (m->state == GONE); }),
                 missiles.end());
}

// Launch a missile, provided any cities are left
void AirSpace::LaunchMissile(SDL_Point target)
{
  if (CountSurvivingCities() > 0)
  {
    missiles.emplace_back(std::make_shared<DefensiveMissile>(target));
  }
}

int AirSpace::CountSurvivingCities()
{
  return std::count_if(cities.begin(), cities.end(), [&](auto const &c)
                       { return c.isAlive; });
}

// Creates a missile with a target, and calculates its path of flight.
DefensiveMissile::DefensiveMissile(SDL_Point t)
{
  target = t;
  position.x = 320; // TODO don't hardcode base location here
  position.y = 640;
  initPosition = position;
  angle = atan2(target.x - position.x, position.y - target.y) * 180 / M_PI;
  // x and y deltas are flipped to get the angle away from the positive vertical axis.
  // Delta-y calc is backwards to handle the flipped vertical axis used by SDL.
  flightDuration = distance(initPosition, target) / speed;
}

// Adjust a missile's position based on its flight time.
// Incrementing position based on dx,dy values did not work
// (missiles did not proceed to target at low speeds for certain angles).
void DefensiveMissile::Move()
{
  if (state == FLIGHT && flightTimeElapsed < flightDuration)
  {
    position.x = flightTimeElapsed / flightDuration * (target.x - initPosition.x) + initPosition.x;
    position.y = flightTimeElapsed / flightDuration * (target.y - initPosition.y) + initPosition.y;
    flightTimeElapsed++;
  }
  else if (state == FLIGHT)
  {
    position = target;
    state = DETONATED;
  }
  else if (state != GONE)
  {
    cloudCounter++;
    if (cloudCounter > cloudResideTime)
    {
      state = GONE;
    }
  }
}

// Creates a hostile missile with a start and target point
HostileMissile::HostileMissile(int start_x, int target_x)
{
  initPosition.x = start_x;
  initPosition.y = 0;
  position = initPosition;
  target.x = target_x;
  target.y = 640;
  angle = atan2(target.x - position.x, position.y - target.y) * 180 / M_PI;
  // x and y deltas are flipped to get the angle away from the positive vertical axis.
  // Delta-y calc is backwards to handle the flipped vertical axis used by SDL.
  flightDuration = distance(initPosition, target) / speed;
}

// Adjust a missile's position based on its flight time.
// Incrementing position based on dx,dy values did not work
// (missiles did not proceed to target at low speeds for certain angles).
// TODO This is identical to DefensiveMissile::Move().  Consider refactoring to avoid duplicate code.
void HostileMissile::Move()
{
  if (state == FLIGHT && flightTimeElapsed < flightDuration)
  {
    position.x = flightTimeElapsed / flightDuration * (target.x - initPosition.x) + initPosition.x;
    position.y = flightTimeElapsed / flightDuration * (target.y - initPosition.y) + initPosition.y;
    flightTimeElapsed++;
  }
  else if (state == FLIGHT)
  {
    position = target;
    state = DETONATED;
  }
  else if (state != GONE)
  {
    cloudCounter++;
    if (cloudCounter > cloudResideTime)
    {
      state = GONE;
    }
  }
}

SmartMissile::SmartMissile(int start_x, int target_x)
{
  initPosition.x = start_x;
  initPosition.y = 0;
  position = initPosition;
  target.x = target_x;
  target.y = 640;
  angle = atan2(target.x - position.x, position.y - target.y); // Initial angle in radians for calculating dx, dy
  dx = speed * sin(angle);
  dy = -speed * cos(angle);            // sin/cos used opposite of convention because the 0 axis is pointed vertically up.  The negative accounts for the y-axis being flipped.
  angle = atan2(dx, -dy) * 180 / M_PI; // Calculating proper angle for animation.  Need to un-flip the y-axis again.
}

void SmartMissile::Move()
{
  if (state == FLIGHT)
  {
    position.x += dx;
    position.y += dy;
    if (position.y > 640)
    {
      position.y = 640;
      state = DETONATED;
    }
    else if (position.x < 0 || position.x > 640 - 1) // Remove missiles that have flown off the screen TODO should not be hardcoded
    {
      state = GONE;
    }
  }
  else if (state != GONE)
  {
    cloudCounter++;
    if (cloudCounter > cloudResideTime)
    {
      state = GONE;
    }
  }
}

void SmartMissile::React(std::vector<std::shared_ptr<Missile>> missiles)
{
  reactTimer--;
  if (reactTimer <= 0)
  {
    // Find the closest missile ahead of it
    SDL_Point closestMissileAhead;
    closestMissileAhead.x = avoidanceRange * 2;
    closestMissileAhead.y = avoidanceRange * 2;
    double closestDistance = avoidanceRange + 0.1;
    for (auto const &missile : missiles)
    {
      if (missile->position.y >= this->position.y)
      { // Ignore any missile above the current missile.  This should also avoid the missile reacting to itself.
        double missileDistance = distance(this->position, missile->position);
        if (missileDistance < closestDistance && missileDistance > 0.1)
        { // ">0.1" avoids the missile reacting to itself.  Redundant if the line above works.
          closestDistance = missileDistance;
          closestMissileAhead = missile->position;
        }
      }
    }

    // React if possible
    if (closestDistance > avoidanceRange)
    {
      // no missile nearby, proceed towards target
      if (target.x > position.x && dx < 0)
      {
        dx++;
      }
      else if (target.x < position.x && dx > 0)
      {
        dx--;
      }
    }
    else
    {
      if (closestMissileAhead.x > position.x)
      {
        dx--;
      }
      else
      {
        dx++;
      }
    }

    // Keep missile flying downwards
    if (dy <= 0)
    {
      dy = 1;
      if (dx > 0)
      {
        dx--;
      }
      else
      {
        dx++;
      }
    }

    // Scale dy to maintain speed
    double speedRatio = speed / sqrt(pow(dx, 2) + pow(dy, 2));
    dx *= speedRatio;
    dy *= speedRatio;

    // Update angle
    angle = atan2(dx, -dy) * 180 / M_PI;

    // Reset reactTimer
    reactTimer = reactInterval;
  }
}

// Initialize a city at a specified location
City::City(int x, int y)
{
  position.x = x;
  position.y = y;
}