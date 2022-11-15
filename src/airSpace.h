#ifndef AIRSPACE_H
#define AIRSPACE_H

#include <vector>
#include "SDL.h"
#include <memory>
#include <random>

enum MissileState{
  FLIGHT,
  DETONATED,
  AIR_BLAST,
  LAND_DET,
  SHOT_DOWN,
  GONE
};

// Abstract class for all missile types
class Missile {
  public:
    virtual void Move() = 0;
    virtual void React(std::vector<std::unique_ptr<Missile>> &missiles) {}; // By default this won't be implemented
    SDL_Point position;
    MissileState state{FLIGHT};
    double angle;
    float blastRadius = 100;
  protected:
    int speed = 4;
    SDL_Point initPosition;
    SDL_Point target;
    int cloudCounter{0}; // Timer in frames since missile blew up
    int cloudResideTime{40}; // Frames missile explosion remains on-screen
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

class SmartMissile : public Missile {
  public:
    SmartMissile(int start_x, int target_x);
    void Move();
    void React(std::vector<std::unique_ptr<Missile>> &missiles);
  private:
    int dx;
    int dy;
    double avoidanceRange = 250;
    int reactInterval = 20;
    int reactTimer = 0;
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
  int CountSurvivingCities();

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
  int blastResideTime = 130; // Frames that missile explosions stick around
  double smartMissileGenProb = 1; // Probability that a missile created is a smart missile, givent that a hostile missile is generated TODO reduce once tested
};

#endif