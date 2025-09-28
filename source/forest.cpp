#include "forest.hpp"
#include "animatedSprite.hpp"
#include "end.hpp"
#include "lake.hpp"
#include "player.hpp"
#include <raylib.h>
#include <raymath.h>
#include <string>
extern Location location;
extern vector<Interaction> interactionBoxes;
extern vector<Rectangle> hitboxes;
static const Rectangle overworldRec = {70, 0, 60 + 70, 40};
static const Rectangle sacrificeBox = {80 - 7, 45 - 10, 13, 15};
static const Rectangle leaveBox = {(0), 90 - 20, 160, 20};
const Rectangle endBox = {120, 0, 20, 30};
extern Player player;
const int fishNeededToSacrifice = 7;
extern float deltaTime;
extern Color groundTint;
int fishSacrificedCount;
extern Camera2D camera;
extern Camera2D secondCamera;
extern Sound success;
extern int WINDOW_WIDTH;
typedef struct forestAssets {
  Sound sacrifice;
  Texture2D tree;
  Texture2D crack;
  Texture2D deadTree;
} forestAssets;
extern bool debugActive;
static forestAssets assets;
bool isSacrificing;
Texture2D sacrificeTextures[19];
typedef struct {
  float timeSacrificing;
  bool animationDone;
  float delayBecauseICantFixABug;
} Status;
static Status status;
AnimatedSprite *sacrificeAnimation;
void drawFishSacrifice();

Texture2D getCrack() { return assets.crack; }
void drawForestTrees() { DrawTexture(assets.tree, 0, 30, WHITE); }
void drawForestCracks() {
  if (fishSacrificedCount >= 1)
    DrawTexture(assets.crack, 0, 10, WHITE);
  if (fishSacrificedCount > 2)
    DrawTextureRec(assets.crack, {0, 0, -50, 50}, {110, 10}, WHITE);
  if (fishSacrificedCount > 3)
    DrawTextureEx(assets.crack, {10, 90}, 270, 1, WHITE);
  if (fishSacrificedCount > 5)
    DrawTextureEx(assets.crack, {95, 0}, 90, 1, GRAY);
}

void drawInsideForest() {
  Texture2D tree = fishSacrificedCount > 4 ? assets.deadTree : assets.tree;
  // draw the cracks
  if (fishSacrificedCount >= fishNeededToSacrifice)
    drawEndInForest();
  // draw the silly pentagram
  for (int i = 0; i < 3; i++) {
    // its broken but honestly im keeping it
    DrawCircleLines(80, 45, 10 - i, RED);
  }
  // draw the exclamation mark
  string text =
      (to_string(fishSacrificedCount) + "/" + to_string(fishNeededToSacrifice));
  DrawText(text.c_str(), 72, 25, 8, RED);
  // draw leave area
  // DrawRectangleRec(leaveBox, BLACK);
  DrawTexture(tree, 80, leaveBox.y - 10, BLACK);
  DrawTexture(tree, 80 + 10, leaveBox.y - 14, BLACK);
  DrawTexture(tree, 80 + 25, leaveBox.y - 18, BLACK);

  // MORE DECORATION!!
  DrawTexture(tree, 0, leaveBox.y - 10, BLACK);
  DrawTexture(tree, 10, leaveBox.y - 14, BLACK);
  DrawTexture(tree, 25, leaveBox.y - 18, BLACK);
  if (debugActive)
    DrawRectangleLinesEx(leaveBox, 2, GREEN);
  if (isSacrificing)
    drawFishSacrifice();
}

void drawForest() {
  switch (location) {
  case Forest:
    drawInsideForest();
    break;
    // draw the forest outside
  case Overworld:
    DrawEllipse((overworldRec.x + overworldRec.width / 2) + 1,
                (overworldRec.y + overworldRec.height / 2) + 10,
                overworldRec.width / 2, overworldRec.height / 2,
                {0, 0, 0, 170});
    Texture2D tree;
    if (fishSacrificedCount < 6) {
      tree = assets.tree;
    } else {
      tree = assets.deadTree;
    }
    for (int i = 0; i < overworldRec.width / 30; i++) {
      DrawTexture(tree, overworldRec.x + (i * 30) + 10, -40, WHITE);
    }
    for (int i = 0; i < overworldRec.width / 30; i++) {
      DrawTexture(tree, overworldRec.x + (i * 30) - 10, -25, WHITE);
    }
    for (int i = 0; i < overworldRec.width / 30; i++) {
      DrawTexture(tree, overworldRec.x + (i * 30) - 5, -13, WHITE);
    }
    break;
  default:
    break;
  }
}
void loadForest() {
  assets.sacrifice = LoadSound("resources/rise.ogg");
  assets.tree = LoadTexture("resources/tree.png");
  assets.crack = LoadTexture("resources/crack.png");
  assets.deadTree = LoadTexture("resources/deadTree.png");
  for (int i = 1; i < 10; i++) {
    sacrificeTextures[i - 1] = LoadTexture(
        ("resources/sacrifice/sacrifice_000" + to_string(i) + ".png").c_str());
  }
  for (int i = 10; i < 20; i++) {
    sacrificeTextures[i - 1] = LoadTexture(
        ("resources/sacrifice/sacrifice_00" + to_string(i) + ".png").c_str());
  }
  // the only manual heap allocation I've made this entire time... ofc im not
  // gonna free it! screw you!
  sacrificeAnimation =
      new AnimatedSprite({70, 21}, 6 / 60.0f, 19, sacrificeTextures);
}

void stopSacrificing() {
  player.canMove = true;
  isSacrificing = false;
  PlaySound(success);
  camera.zoom = 1.0f;
  fishSacrificedCount += player.inventory.size();
  player.inventory.clear();
  if (fishNeededToSacrifice == fishSacrificedCount) {
    enterEnd();
  }
  groundTint.a -= fishSacrificedCount;
  groundTint.r += fishSacrificedCount;
}
void enterForest() {
  location = Forest;
  status.timeSacrificing = 0.0f;
  interactionBoxes.clear();
  hitboxes.clear();
  player.x = 50;
  player.y = 72;
}

// TODO draw this
void drawFishSacrifice() {
  if (sacrificeAnimation->currentFrame != sacrificeAnimation->frameCount - 1 &&
      !status.animationDone) {
    if (status.delayBecauseICantFixABug > 0.8f)
      sacrificeAnimation->draw();
  } else
    status.animationDone = true;
}

void drawFishSacrificeHud() {
  if (status.timeSacrificing < 1.7f) {
    DrawRectangleRec({0, 0, 160, 90}, {0, 0, 0, 40});
    DrawText("You lay your fish down", 3, 45, 8, RAYWHITE);
  }
}
void updateFishSacrifice() {
  // printf("%f\n",status.timeSacrificing);
  // status.isSacrificing = true;
  if (status.timeSacrificing > 3.7f || IsKeyPressed(KEY_K)) {
    stopSacrificing();
    return;
  }
  status.timeSacrificing += deltaTime;
  status.delayBecauseICantFixABug += deltaTime;
}

void exitForest() {
  location = Overworld;
  interactionBoxes.clear();
  hitboxes.clear();
  player.updatePos(80, 59);
}

void sacrificeFish() {
  if (player.inventory.size() < 1) {
    return;
  }
  PlaySound(assets.sacrifice);
  isSacrificing = true;
  status.timeSacrificing = 0.0f;
  player.canMove = false;
  status.animationDone = false;
  status.delayBecauseICantFixABug = 0.0f;
  sacrificeAnimation->currentFrame = 0;
}

void updateForestInsideForest() {
  // lol
  hitboxes.push_back(sacrificeBox);
  interactionBoxes.push_back({sacrificeBox, sacrificeFish});
  interactionBoxes.push_back({leaveBox, exitForest});
  // if(fishSacrificedCount > fishNeededToSacrifice)
  interactionBoxes.push_back({endBox, enterEnd});

  if (isSacrificing) {
    updateFishSacrifice();
  }
}

void updateForest() {
  if (location == Overworld) {
    hitboxes.push_back(overworldRec);
    interactionBoxes.push_back({overworldRec, enterForest});
  } else {
    updateForestInsideForest();
  }
}
void endCameraShake() { secondCamera.offset = {0, 0}; }
// believe it or not, I did not copy this from anywhere! desmos!
void cameraShake(float strength) {
  static float time = 0.0f;
  // based on 720p
  const float speed = (120.0f / 10);
  float maxDistance = 5.0f * (WINDOW_WIDTH / 1280.0f) * strength;
  float offset = sin((time * PI) * speed) * maxDistance;
  secondCamera.offset = {offset, offset};
  time += deltaTime;
}