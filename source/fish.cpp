#include "fish.hpp"
#include <cstdint>
#include <string>
#include <raylib.h>
using namespace std;

const string names[] = {
    "Salmon",
    "Tuna",
    "Cod",
    "Trout",
};

string getFishName(int i) {
    return names[i];
}

Fish getFish() {
    uint8_t id = (uint8_t)rand() % 4;
    return {
        .name = getFishName(id),
        .id = (uint8_t)id,
        .texture = {0}
    };
}