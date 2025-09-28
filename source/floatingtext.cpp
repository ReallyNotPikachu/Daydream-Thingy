#include <raylib.h>
#include <string>
using namespace std;
extern float deltaTime;
#include "floatingtext.hpp"
static float yOffset = 0.0f;
static float timeSinceMove = 0.0f;
void floatingText::draw() { 
    DrawText(text.c_str(), x, y + yOffset, 10, WHITE);
 }
void floatingText::update() {

}
void floatingText::updateOffset() {
  if (timeSinceMove > (8/60.0f)) {
    yOffset++;
    if (yOffset > 6) {
      yOffset = 0;
    }
    timeSinceMove = 0.0f;
  }
  timeSinceMove += deltaTime;
}
floatingText::floatingText(Vector2 pos, string text) {
  x = pos.x;
  y = pos.y;
  visible = true;
  this->text = text;
}