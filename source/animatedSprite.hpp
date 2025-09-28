#pragma once
#include <cstdint>
#include <raylib.h>

class AnimatedSprite {
public:
  uint16_t frameCount;
  Texture2D *frames;
  Vector2 pos;
  uint16_t currentFrame;
  float timePerFrame;
  float timeSinceFrameChange;
  AnimatedSprite(Vector2 pos, float timePerFrame, uint16_t frames,
                 Texture2D *textures);
  void draw();
};