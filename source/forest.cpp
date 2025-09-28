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
static const Rectangle leaveBox = {(160 / 2.0f), 90 - 20, 50, 20};
const Rectangle endBox = {120, 0, 20, 30};
extern Player player;
const int fishNeededToSacrifice = 7;
extern float deltaTime;
int fishSacrificedCount;
extern Camera2D camera;
typedef struct forestAssets {
  Sound sacrifice;
  Texture2D tree;
} forestAssets;
static forestAssets assets;
bool isSacrificing;
Texture2D sacrificeTextures[19];
typedef struct {
  float timeSacrificing;
  bool animationDone;
  float delayBecauseICantFixABug;
} Status;
static Status status;
AnimatedSprite* sacrificeAnimation;
void drawFishSacrifice();

void drawForestTrees() {
  DrawTexture(assets.tree, 0, 30, WHITE);
}
void drawInsideForest() {
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
  DrawRectangleRec(leaveBox, BLACK);
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
    DrawEllipse((overworldRec.x + overworldRec.width/2)+1, (overworldRec.y + overworldRec.height/2)+10, overworldRec.width/2, overworldRec.height/2, {0,0,0,170});
        for (int i = 0; i < overworldRec.width / 30; i++) {
      DrawTexture(assets.tree, overworldRec.x + (i * 30) +10, -40, WHITE);
    }
    for (int i = 0; i < overworldRec.width / 30; i++) {
      DrawTexture(assets.tree, overworldRec.x + (i * 30) -10, -25, WHITE);
    }
    for (int i = 0; i < overworldRec.width / 30; i++) {
      DrawTexture(assets.tree, overworldRec.x + (i * 30) -5, -13, WHITE);
    }
    break;
  default:
    break;
  }
}
void loadForest() {
  assets.sacrifice = LoadSound("resources/rise.ogg");
  assets.tree = LoadTexture("resources/tree.png");
  for(int i = 1; i < 10; i++) {
    sacrificeTextures[i-1] = LoadTexture(("resources/sacrifice/sacrifice_000"+to_string(i) +".png").c_str());
  }
  for(int i = 10; i < 20; i++) {
    sacrificeTextures[i-1] = LoadTexture(("resources/sacrifice/sacrifice_00"+to_string(i) +".png").c_str());
  }
  //the only manual heap allocation I've made this entire time... ofc im not gonna free it! screw you!
  sacrificeAnimation = new AnimatedSprite({70,21},6/60.0f,19,sacrificeTextures);
}

void stopSacrificing() {
  player.canMove = true;
  isSacrificing = false;
  camera.zoom = 1.0f;
  fishSacrificedCount += player.inventory.size();
  player.inventory.clear();
  if(fishNeededToSacrifice == fishSacrificedCount){
    enterEnd();
  }
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
  if(sacrificeAnimation->currentFrame != sacrificeAnimation->frameCount-1 && !status.animationDone){
    if(status.delayBecauseICantFixABug > 0.8f)
    sacrificeAnimation->draw();
  }
  else 
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