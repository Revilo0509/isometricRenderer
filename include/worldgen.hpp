#pragma once

#include "raylib.h"
#include "gameClasses.hpp"
#include "PerlinNoise.hpp"
#include <random>

extern const int WORLD_SIZE;

Color rgb(int r, int g, int b, int a = 255);

void generateWorld(terrainPoint* world, Texture tree);