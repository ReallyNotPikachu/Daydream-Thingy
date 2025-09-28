#include "player.hpp"
#include "animatedSprite.hpp"
#include <cstdio>
#include <raylib.h>
#include <vector>
using namespace std;
extern float deltaTime;
extern bool debugActive;
extern vector<Rectangle> hitboxes;
extern vector<Interaction> interactionBoxes;
extern float timeSinceLastFished;
Texture2D leftTextures[3];
Texture2D rightTextures[3];
Texture2D downTextures[3];
Texture2D upTextures[3];
Texture2D idleFrames[2];
// I'm switching to rust soon dw
void iHateCPP() {
  // im using commas... to end lines.. and its working??
  leftTextures[0] = LoadTexture("resources/player/player_0012.png");
  leftTextures[1] = LoadTexture("resources/player/player_0013.png");
  leftTextures[2] = LoadTexture("resources/player/player_0014.png");
  upTextures[0] = LoadTexture("resources/player/player_0003.png"),
  upTextures[1] = LoadTexture("resources/player/player_0004.png"),
  upTextures[2] = LoadTexture("resources/player/player_0005.png");
  downTextures[0] = LoadTexture("resources/player/player_0006.png"),
  downTextures[1] = LoadTexture("resources/player/player_0007.png"),
  downTextures[2] = LoadTexture("resources/player/player_0008.png");
  rightTextures[0] = LoadTexture("resources/player/player_0009.png"),
  rightTextures[1] = LoadTexture("resources/player/player_0010.png"),
  rightTextures[2] = LoadTexture("resources/player/player_0011.png");
  idleFrames[0] = LoadTexture("resources/player/player_0001.png"),
  idleFrames[1] = LoadTexture("resources/player/player_0002.png");
}

// this smells funny
// but apparently i have to do this???
Player::Player() 
  : idle({10.0f, 10.0f}, (50/60.0f), 2, idleFrames),
      runLeft({10.0f, 10.0f}, (15/60.0f), 3, leftTextures),
    runRight({10.0f, 10.0f}, (15/60.0f), 3, rightTextures),
    runUp({10.0f, 10.0f}, (15/60.0f), 3, upTextures),
      runDown({10.0f, 10.0f}, (15/60.0f), 3, downTextures)
{
  canMove = true;
  isFishing = false;
  x = 10.0f;
  y = 10.0f;
  direction = South;
}
Rectangle Player::getHitbox() {
  return (Rectangle){x, y, 12, 16}; }

Vector2 directionToVector(Direction direction) {
  switch (direction) {
  case North:
    return {0, -1};
    break;
  case South:
    return {0, 1};
  case West:
    return {-1, 0};
  case East:
    return {1, 0};
  }
}

void Player::draw() {
  if (!isMoving) {
    idle.draw();
  } else {
    switch (direction) {
    case North:
      runUp.draw();
      break;
    case South:
      runDown.draw();
      break;
    case East:
      runRight.draw();
      break;
    case West:
      runLeft.draw();
      break;
    }
  }
  if (debugActive) {
    DrawRectangleLinesEx(getInteractionBox(), 2, RED);
  }
}

Rectangle Player::getInteractionBox() {
  // WHAT THE FLIPPITY FLIPPERS WHY WHY WHY WHY DO I HAVE TO DO IT THIS WAY WHY
  // CANT IT JUST WORK
  //  Rectangle rec;
  const float BOXWIDTH = 10.0f;
  const float BOXHEIGHT = 16.0f;
  switch (direction) {
  case North:
    return {x - BOXWIDTH + (12), y - BOXHEIGHT, BOXWIDTH, BOXHEIGHT};
  case South:
    return {x + (16 / 2.0f) - BOXWIDTH + 5, y + 16, BOXWIDTH, BOXHEIGHT};
  case West:
    return {x - BOXHEIGHT, y + (16 / 2.0f) - BOXWIDTH / 2, BOXHEIGHT, BOXWIDTH};
  case East:
    // I GIVE UP I AHTE THIS SO MUCH
    return {x + 12, y + 3 + (16) - BOXHEIGHT, BOXHEIGHT, BOXWIDTH};
  }
}

bool Player::willCollide(Vector2 &velocity) {
  Rectangle future = {x + velocity.x, y + velocity.y, 12, 16};
  printf("%lu\n", hitboxes.size());
  for (int i = 0; i < hitboxes.size(); i++) {
    if (CheckCollisionRecs(future, hitboxes.at(i))) {
      return true;
    }
  }
  return false;
}
void Player::updatePos(float x, float y) {
  Vector2 temp = {x,y};
  this->x = x;
  this->y = y;
  idle.pos = temp;
  runDown.pos = temp;
  runUp.pos = temp;
  runLeft.pos = temp;
  runRight.pos = temp;
}
void Player::update() {
  // figure it out
  if (!canMove)
    return;
  if (isFishing) {
    return;
  }
  const float sprintMultiplier = 1.75f;
  isMoving = false;
  Vector2 direction;
  direction.x = 0.0f;
  direction.y = 0.0f;
  // sprint
  if (IsKeyDown(KEY_W)) {
    isMoving = true;
    this->direction = North;
    direction.y = -1;
  }
  if (IsKeyDown(KEY_S)) {
    isMoving = true;
    this->direction = South;
    direction.y = 1;
  }
  if (IsKeyDown(KEY_D)) {
    isMoving = true;
    this->direction = East;
    direction.x = 1;
  }
  if (IsKeyDown(KEY_A)) {
    isMoving = true;
    this->direction = West;
    direction.x = -1;
  }
  if (IsKeyDown(KEY_J)) {
    direction.x *= sprintMultiplier;
    direction.y *= sprintMultiplier;
  }
  if (IsKeyPressed(KEY_K)) {
    isMoving = false;
    if (timeSinceLastFished < 2.0f)
      return;
    for (Interaction interaction : interactionBoxes) {
      if (CheckCollisionRecs(interaction.box, getInteractionBox())) {
        interaction.func();
      }
    }
  }
  // debug
  if (IsKeyPressed(KEY_F4)) {
    debugActive = !debugActive;
  }
  float dX = direction.x * deltaTime * 23.0f;
  float dY = direction.y * deltaTime * 23.0f;
  Vector2 temp = {dX, dY};
  if (willCollide(temp)) {
    return;
  }
  updatePos(x+dX, y+dY);
}