#pragma once

#include <iostream>
#include <math.h>
#include <vector>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib.h"
#include "resource_dir.h"

#include "gameClasses.hpp"
#include "worldgen.hpp"
#include "misc.hpp"

const int WINDOW_HEIGHT = 800;
const int WINDOW_WIDTH = 1280;

const int WORLD_SIZE = 32;

const Texture noTextureImg = LoadTexture("no_texture.png");