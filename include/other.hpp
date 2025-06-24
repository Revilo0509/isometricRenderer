#pragma once

#include "raylib.h"
#include "gameClasses.hpp"
#include "gameGlobalVariables.h"
#include "PerlinNoise.hpp"
#include <random>

Color rgb(int r, int g, int b, int a = 255);

void generateWorld(terrainPoint world[WORLD_SIZE][WORLD_SIZE], Texture tree);