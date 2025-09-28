#include "lake.hpp"
#include "fish.hpp"
#include "player.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <raylib.h>
#include <string>
#include <variant>
#include <vector>
using namespace std;
extern int fishCaught;
extern vector<Rectangle> hitboxes;
extern vector<Interaction> interactionBoxes;
extern Player player;
static Rectangle box = {0, 90 - 30, 60, 70};
// coorespond with index of fish name
extern float deltaTime;
float timeSinceLastFished = 4.0f;
typedef struct lakeResource {
  Texture2D grass;
  Texture2D grassWater;
  Texture2D grassWaterSide;
  Texture2D water;
  Texture2D bobber;
  Texture2D grassWaterCorner;
  Sound moveBobber;
} lakeResources;
static lakeResources assets;
typedef struct {
  Fish currentFish;
  float floaterPoint;
  bool fishingSuccessful;
  float timeLeft;
  float currentTime;
  float timeSinceFishAction;
  // 15-25
  float fishSpeed;
  string lastFishName;
  float timeWhenFishAction;
  bool firstCycle;
} FishingStatus;
Sound fishing;
extern bool debugActive;
static FishingStatus status;

void drawRod() {
  if (player.direction == South) {
    DrawTexture(assets.bobber, player.x + 6, player.y + 34, WHITE);
    DrawRectangle(player.x + 5, player.y + 16, 2, 16, BLACK);

  } else {
    DrawTexture(assets.bobber, player.x - 19, player.y + 16, WHITE);
    DrawRectangle(player.x - 15, player.y + 6, 16, 2, BLACK);
  }
}

string twoDecimals(string f32) {
  size_t location = f32.find('.');
  if (location == variant_npos)
    return f32;
  if (f32.length() > location + 2) {
    return f32.substr(0, location + 2);
  } else {
    if (f32.length() > location + 1) {
      f32[location + 1] = '\0';
      return f32.substr(0, location + 1);
    }
    return f32;
  }
}
void drawYouGotAMenu() {
  if (timeSinceLastFished < 2.0f) {
    if (status.fishingSuccessful) {
      DrawText(("You got a " + status.lastFishName).c_str(), 50, 57,
               8 + timeSinceLastFished, BLACK);
    } else {
      DrawText("You suck!", 80, 57, 10, RED);
    }
  }
}
double randomTime() {
  default_random_engine gen;
  uniform_real_distribution<double> distribution(1, 2);
  return distribution(gen);
}
void drawFishingHud() {
  DrawText(twoDecimals(to_string(status.timeLeft - status.currentTime)).c_str(),
           140, 4, 8, BLACK);
  if (status.firstCycle) {
    DrawText("Fish caught!", 80 - MeasureText("Fish Caught!", 8), 45, 8, RED);
    DrawText("(k to reel)", 80, 57, 8, BLACK);
  }
  DrawRectangle(142, 17, 14, 56, {0,0,0,100});
  DrawRectangle(140, 15, 15, 60, RED);
  // good zone
  DrawRectangle(140, 15 + 15, 15, 30, GREEN);
  // bobber
  // DrawRectangle(141, status.floaterPoint + 15, 13, 15, ORANGE);
  DrawEllipse(142+1+4, status.floaterPoint + 15 , 6,3, ORANGE);
  // draw the
  // DrawPixel(142+1+4, status.floaterPoint + 15+2, RED);
}
// update
void endFishing() {
  StopSound(fishing);
  status.fishingSuccessful =
      status.floaterPoint >= 15 && status.floaterPoint <= 45;
  if (status.fishingSuccessful) {
    player.inventory.push_back(getFish());
    status.lastFishName = player.inventory.back().name;
  }
  player.isFishing = false;
  timeSinceLastFished = 0;
}
void fish(void) {
  printf("%f\n", status.floaterPoint);
  // end fishing
  if (status.timeLeft - status.currentTime <= 0) {
    endFishing();
  }
  if (status.floaterPoint > 60 || status.floaterPoint < 0) {
    endFishing();
  }
  if (IsKeyPressed(KEY_K)) {
    status.floaterPoint -= 10;
  }
  if (debugActive) {
    if (IsKeyPressed(KEY_NINE)) {
      endFishing();
    }
  }
  if (status.timeSinceFishAction >= status.timeWhenFishAction) {
    status.floaterPoint += GetRandomValue(-11, 11);
    status.timeSinceFishAction = 0.0f;
    status.timeWhenFishAction = randomTime();
    status.firstCycle = false;
  }
  status.floaterPoint += status.fishSpeed * deltaTime;
  status.timeSinceFishAction += deltaTime;
  status.currentTime += deltaTime;
}

void loadLake() {
  fishing = LoadSound("resources/splish.ogg");
  assets.grass = LoadTexture("resources/grass.png");
  assets.grassWaterCorner = LoadTexture("resources/wateredgecorner.png");
  assets.grassWaterSide = LoadTexture("resources/wateredgeright.png");
  assets.grassWater = LoadTexture("resources/wateredge.png");
  assets.water = LoadTexture("resources/water.png");
  assets.bobber = LoadTexture("resources/bobber.png");
}

void startFishing(void) {
  PlaySound(fishing);
  player.isFishing = true;
  status.floaterPoint = 0.1f;
  status.currentTime = 0.0f;
  while ((status.timeLeft = rand() % 10) < 3.0f)
    ;
  status.timeWhenFishAction = 3.0f;
  status.firstCycle = true;
  status.fishSpeed = 25.0f;
}
void updateLake() {
  hitboxes.push_back(box);
  interactionBoxes.push_back({box, startFishing});
  if (!player.isFishing)
    timeSinceLastFished += deltaTime;
}
void drawlake() {
  for (int x = 0; x < 160 / 16; x++) {
    for (int y = 0; y < (90 / 16) + 1; y++) {
      DrawTexture(assets.grass, 0 + (16 * x), 0 + (16 * y), WHITE);
    }
  }
  // draw the lake
  // 90-23
  for (int i = 0; i < (box.width / 16); i++) {
    DrawTexture(assets.grassWater, 0 + (i * 16), box.y, WHITE);
  }
  DrawTexture(assets.grassWaterCorner, box.width - 10, box.y, WHITE);
  for (int y = 0; y < box.height / 16; y++) {
    for (int x = 0; x < (box.width / 16); x++) {
      DrawTexture(assets.water, 0 + (x * 16), (box.y + 16) + (y * 16), WHITE);
    }
  }
  DrawTexture(assets.grassWaterSide, box.width - 10, box.y + 16, WHITE);
  DrawTexture(assets.grassWaterSide, box.width - 10, box.y + 32, WHITE);
  DrawTexture(assets.grassWaterSide, box.width - 10, box.y + 48, WHITE);
  DrawTexture(assets.grassWaterSide, box.width - 10, box.y + 64, WHITE);
  // DrawRectangleRec(box, BLUE);
  if (player.isFishing) {
    drawRod();
  }
}