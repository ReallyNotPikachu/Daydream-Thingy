#pragma once
#include "fish.hpp"
#include <raylib.h>
#include <vector>
using namespace std;
#include <raymath.h>
#include "animatedSprite.hpp"

typedef enum { North, South, East, West } Direction;
class Player {
public:
  bool canMove;
  bool isFishing;
  float x, y, width, height;
  Direction direction;
  void update();
  void move();
  bool isMoving;
  AnimatedSprite idle;
  AnimatedSprite runLeft;
  AnimatedSprite runRight;
  AnimatedSprite runUp;
  AnimatedSprite runDown;
  Texture2D sacrifice;
  void draw();
  Player();
  vector<Fish> inventory;
  Rectangle getInteractionBox();
  Rectangle getHitbox();
  bool willCollide(Vector2 &velocity);
  void updatePos(float x, float y);
};
typedef enum {
  Overworld = 0,
  Forest = 1, 
  End = 2,
  Nullpointer = 3
} Location;

typedef struct {
  Rectangle box;
  void (*func)(void);
} Interaction;
void iHateCPP();