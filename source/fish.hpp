#pragma once
#include <cstdint>
#include <raylib.h>
#include <string>
using namespace std;
typedef struct {
  string name;
  uint8_t id;
  Texture2D texture;
} Fish;

// string getFishName();
Fish getFish();