#include "airSpace.h"
#include <cmath>
#include <iostream>

void AirSpace::Update() {

  // Fire control
  if (fire && !missile.health) {
    missile.position.x = 320; // TODO don't hardcode base location here
    missile.position.y = 640;
    missile.health = true;
  }
  if (missile.health) {
    missile.position.y += missile.speed;
    if (missile.position.y > 640 || missile.position.y < 0 || missile.position.x < 0 || missile.position.x > 640) { // TODO don't hardcode screen height
      missile.health = false;
    }
  }
}
