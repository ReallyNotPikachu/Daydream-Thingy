#include "animatedSprite.hpp"
#include <raylib.h>
extern float deltaTime;

//handles updating and logic all in one!
void AnimatedSprite::draw() {
  timeSinceFrameChange += deltaTime;
  DrawTexture(frames[currentFrame], pos.x, pos.y, WHITE);
  if (timeSinceFrameChange >= timePerFrame) {
    currentFrame = (currentFrame + 1) % frameCount;
    timeSinceFrameChange = 0.0f;
  }
}

AnimatedSprite::AnimatedSprite(Vector2 pos, float timePerFrame, uint16_t frames,
                               Texture2D *textures) {
  this->pos = pos;
  this->timePerFrame = timePerFrame;
  this->frameCount = frames;
  this->frames = textures;
  currentFrame = 0;
  timeSinceFrameChange = 0.0f;
}