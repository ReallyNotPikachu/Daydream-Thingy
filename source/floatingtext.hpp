#include <raylib.h>
#include <string>
using namespace std;
class floatingText {
public:
  string text;
  float x;
  float y;
  bool visible;
  void draw();
  void update();
  void updateOffset();
  void initText();
  floatingText(Vector2 pos, string text);
};