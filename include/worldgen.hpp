#pragma once

#include "raylib.h"
#include "gameClasses.hpp"
#include "PerlinNoise.hpp"
#include <random>

#include <iostream>

extern const int WORLD_SIZE;

void generateWorld(terrainPoint* world);