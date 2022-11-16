#ifndef AIRSPACE_H
#define AIRSPACE_H

#include <vector>
#include "SDL.h"
#include <memory>
#include <random>
#include <mutex>

extern const short int K_SCREEN_WIDTH;
extern const short int K_SCREEN_HEIGHT;

enum MissileState
{
  FLIGHT,
  DETONATED,
  AIR_BLAST,
  LAND_DET,
  SHOT_DOWN,
  GONE
};

// Abstract class for all missile types
class Missile
{
public:
  virtual void Move() = 0;
  virtual void React(std::vector<std::shared_ptr<Missile>> missiles) = 0;
  SDL_Point position;
  MissileState state{FLIGHT};
  double angle;
  float blastRadius = 100;

protected:
  int speed = 4;
  SDL_Point initPosition;
  SDL_Point target;
  int cloudCounter{0};     // Timer in frames since missile blew up
  int cloudResideTime{40}; // Frames missile explosion remains on-screen
};

// Missile launched by player.  Travels in straight line from the missile base to where the cursor was when launched.
class DefensiveMissile : public Missile
{
public:
  DefensiveMissile(SDL_Point t);
  void Move();
  void React(std::vector<std::shared_ptr<Missile>> missiles){};

private:
  int flightTimeElapsed = 0; // the number of frames since the missile's creation
  double flightDuration;     // The number of frames the missile will take to reach its target
};

// Goes in straight line from top of screen to bottom of screen.
class HostileMissile : public Missile
{
public:
  HostileMissile(int start_x, int target_x);
  void Move();
  void React(std::vector<std::shared_ptr<Missile>> missiles){};

private:
  int flightTimeElapsed = 0; // the number of frames since the missile's creation
  double flightDuration;     // The number of frames the missile will take to reach its target
};

// Missile that dodges incoming missiles.
class SmartMissile : public Missile
{
public:
  SmartMissile(int start_x, int target_x);
  void Move();
  void React(std::vector<std::shared_ptr<Missile>> missiles);

private:
  int dx;
  int dy;
  double avoidanceRange = 250;
  int reactInterval = 20;
  int reactTimer = 0;
};

class City
{
public:
  SDL_Point position;
  bool isAlive = true;
  City(int x, int y);
};

// Essentially the missile-command specific game object.
class AirSpace
{
public:
  AirSpace();
  void Update();
  void LaunchMissile(SDL_Point t);
  int CountSurvivingCities();

  SDL_Point mouseCursorPos;
  std::vector<std::shared_ptr<Missile>> missiles;
  std::vector<City> cities;

private:
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_real_distribution<double> randomProp;
  const int maxMissiles = 24;         // Includes friendly missiles
  const double missileGenProb = 0.01; // Probability of generating a missile in a given frame
  int blastResideTime = 130;          // Frames that missile explosions stick around
  double smartMissileGenProb = 0.5;   // Probability that a missile created is a smart missile, givent that a hostile missile is generated
};

#endif