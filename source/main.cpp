/*
This entire codebase should be refactored lmao
but I HAVE NO TIME!!!
*/
#include "end.hpp"
#include "forest.hpp"
#include "lake.hpp"
#include "player.hpp"
#include <cmath>
#include <cstddef>
#include <emscripten.h>
#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>
// unmaintanable "heck"
using namespace std;
int fishCaught;
Location location = Overworld;
float deltaTime;
Camera2D secondCamera;
extern bool isSacrificing;
RenderTexture render;
Player player;
bool worldExists = true;
double seconds = 0.0;
void resize();
bool debugActive = false;
extern float timeSinceLastFished;
vector<Rectangle> hitboxes;
extern bool cameraShaking;
vector<Interaction> interactionBoxes;
Camera2D camera;
bool intro = true;
float virtualRatio = 1280 / 160.0f;
int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;
Color groundTint = WHITE;
uint64_t frameCounter;
void cameraFollowPlayer() {
  const float lerpAmount = 8.0f;
  camera.target = {
      Lerp(camera.target.x, player.x + 16 / 2.0f, deltaTime * lerpAmount),
      Lerp(camera.target.y, player.y + 12 / 2.0f, deltaTime * lerpAmount)};
  camera.offset = {80, 45};
}
void clearBoxes() {
  interactionBoxes.clear();
  hitboxes.clear();
}
void drawHitboxes() {
  for (Rectangle hitbox : hitboxes) {
    DrawRectangleLines(hitbox.x, hitbox.y, hitbox.width, hitbox.height, WHITE);
  }
  DrawRectangleRec(player.getInteractionBox(), RED);
}

void gameIntro() {
  BeginDrawing();
  BeginTextureMode(render);
  ClearBackground(WHITE);
  // DrawText("Fishing Simulator 1692", 3, 4, 10, RED);
  DrawText("Fish are quite desirable\nBring fish to the woods\n -- Controls "
           "--\nK to confirm/fish\nJ to sprint\nWASD "
           "obviously to move\n click for sound btw",
           3, 4, 10, BLACK);
  EndTextureMode();
  DrawTexturePro(render.texture,
                 (Rectangle){0.0f, 0.0f, (float)render.texture.width,
                             -(float)render.texture.height},
                 (Rectangle){-virtualRatio, -virtualRatio,
                             WINDOW_WIDTH + (virtualRatio * 2),
                             WINDOW_HEIGHT + (virtualRatio * 2)},
                 (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
  EndDrawing();
}
void drawFishCount() {
  string str = "Fish: " + to_string(player.inventory.size());
  DrawText(str.c_str(), 2, 1, 7, RED);
}
void resize() {
  WINDOW_HEIGHT = GetScreenHeight();
  WINDOW_WIDTH = GetScreenWidth();
  virtualRatio = WINDOW_WIDTH / 160.0f;
}
void loop() {
  //i give up
  if (IsWindowResized()) {
    resize();
  }
  if (intro) {
    if (IsKeyPressed(KEY_K)) {
      intro = false;
    }
    gameIntro();
    return;
  }
  if (location == Overworld) {
    updateLake();
  }
  if (location == End) {
    updateEnd();
  }
  if (player.isFishing)
    fish();
  if (location != End && location != Nullpointer)
    updateForest();
  // DO NOT UPDATE HITBOXES AFTER THIS POINT
  player.update();
  BeginDrawing();
  deltaTime = GetFrameTime();
  BeginTextureMode(render);
  BeginMode2D(camera);
  ClearBackground(WHITE);
  if (location == Overworld) {
    drawlake();
  } else if (location == End) {
    drawEnd();
  }
  if (location == Forest)
    drawForestCracks();
  if (location != Nullpointer) {
    player.draw();
  }
  drawForest();

  if (debugActive) {
    drawHitboxes();
  }

  if (isSacrificing)
    drawFishSacrificeHud();
  if (player.isFishing)
    drawFishingHud();
  if (!(location == End))
    drawFishCount();
  drawYouGotAMenu();
  if (location == Nullpointer) {
    puts("gqwhgiuqwgehiqwhg");
    DrawRectangle(0, 0, 160, 90, BLACK);
    endWorld();
  }
  EndMode2D();
  EndTextureMode();
  BeginMode2D(secondCamera);
  DrawTexturePro(render.texture,
                 (Rectangle){0.0f, 0.0f, (float)render.texture.width,
                             -(float)render.texture.height},
                 (Rectangle){-virtualRatio, -virtualRatio,
                             WINDOW_WIDTH + (virtualRatio * 2),
                             WINDOW_HEIGHT + (virtualRatio * 2)},
                 (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
  EndMode2D();
  EndDrawing();
  if (location == End) {
    cameraFollowPlayer();
  }
  if (isSacrificing) {
    cameraShake(1);
  } else {
    endCameraShake();
  }
  frameCounter++;
  seconds += deltaTime;
  clearBoxes();
}

int main(void) {
  srand(time(NULL));
  SetRandomSeed(time(NULL));
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "fishing");
  SetWindowMinSize(160, 90);
  InitAudioDevice();
  iHateCPP(); // need to load before player
  // load
  loadEnd();
  loadLake();
  hitboxes = vector<Rectangle>();
  interactionBoxes = vector<Interaction>();
  loadForest();
  camera = {{0, 0}, {0.0f, 0.0f}, 0.0f, 1.0f};
  secondCamera = camera;
  player = Player();
  render = LoadRenderTexture(160, 90);
  // enterEnd();
  emscripten_set_main_loop(loop, 0, true);
}