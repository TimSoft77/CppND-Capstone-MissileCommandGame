#ifndef AIRSPACE_H
#define AIRSPACE_H

#include <vector>
#include "SDL.h"
#include <memory>
#include <random>

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

class HostileMissile : public Missile {
  public:
    HostileMissile(int start_x, int target_x);
    void Move();
  private:
    int flightTimeElapsed = 0; // the number of frames since the missile's creation
    double flightDuration; // The number of frames the missile will take to reach its target
};

class City {
  public:
    SDL_Point position;
    bool isAlive = true;
    City(int x, int y);
};

class AirSpace {
 public:
  AirSpace();
  void Update();
  void LaunchMissile(SDL_Point t);

  SDL_Point mouseCursorPos;
  std::vector<std::unique_ptr<Missile>> missiles;
  std::vector<City> cities;

 private:
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_real_distribution<double> randomProp;
  const int maxMissiles = 24; // Includes friendly missiles
  const double missileGenProb = 0.01; // Probability of generating a missile in a given frame
};

#endif