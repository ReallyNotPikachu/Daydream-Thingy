#include "end.hpp"
#include "floatingtext.hpp"
#include "player.hpp"
#include <climits>
#include <cstdlib>
#include <raylib.h>
extern float deltaTime;
extern vector<Interaction> interactionBoxes;
extern vector<Rectangle> hitboxes;
extern Player player;
extern Location location;
typedef struct {
  float timeInEnd;
} Status;
Status status;
extern bool worldExists;
extern Camera2D camera;
static const Rectangle endBox = {120, 0, 20, 30};
floatingText text1 = floatingText({30, 62}, "Do you know why\nI want fish?");
floatingText text2 = floatingText({30, 62}, "It's a rather long story\n");
floatingText text3 =
    floatingText({30, 62}, "Actually nevermind, this will do\n");
floatingText text4 = floatingText({30, 62}, "Go run along now, goodbye\n");
bool cameraShaking = false;
// end at x=250 y = -50
// i just realized this isn't needed... OOPS
uint8_t yOffsets[196];
uint8_t xOffsets[196];

void endWorld() {
  ClearBackground(BLACK);
  DrawText("Thanks for playing.", 0, 0, 10, WHITE);
}

void drawStars() {
  for (int x = 1; x < 14; x++) {
    for (int y = 1; y < 14; y++) {
      DrawRectangle(-100 + (300 / 14.0f * x) + xOffsets[x],
                    -50 + (300 / 14.0f * y) + yOffsets[y], 3, 3, WHITE);
    }
  }
}
void drawEndInForest() { DrawRectangleRec(endBox, BLACK); }

void enterEnd() {
  location = End;
  player.canMove = true;
  player.isFishing = false;
  player.x = 0;
  player.y = 72;
  hitboxes.clear();
}

void drawEndInEnd() {
  // yes
  ClearBackground(BLACK);
  drawStars();
  if (status.timeInEnd < 2.0f) {
    text1.draw();
  } else if (status.timeInEnd < 4.0f) {
    text2.draw();
  } else if (status.timeInEnd < 6.0f) {
    text3.draw();
  } else {
    text4.draw();
  }
  cameraShaking = status.timeInEnd > 10.0f;
  // end the world
}

void loadEnd() {
  for (int i = 0; i < 196; i++) {
    xOffsets[i] = GetRandomValue(0, 4);
    yOffsets[i] = GetRandomValue(0, 4);
  }
}

void drawEnd() {
  // unneeded
  switch (location) {
  case Forest:
    drawEndInForest();
    break;
  case End:
    drawEndInEnd();
    break;
  default:
    break;
  }
}

// update the end ooohh spoopy
void updateEnd() {
  hitboxes.push_back({-10, 0, 10, 900});
  hitboxes.push_back({160, 0, 10, 900});
  hitboxes.push_back({0, -10, 900, 10});
  hitboxes.push_back({0, 200, 900, 10});
  text1.updateOffset();
  status.timeInEnd += deltaTime;
  if (status.timeInEnd > 15.0f) {
    worldExists = false;
  }
}