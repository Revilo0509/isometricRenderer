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

const int WORLD_SIZE = 128;

const Texture emptyTexture = LoadTexture("no_texture.png");

void init();
void handleZoom(int& zoom, int minZoom, int maxZoom, float scrollSpeed);
void handleMouse(Vector2& mousePos, Vector2& mouseDelta);
void getMouseGridPosition(const Vector2& mousePos, const Texture& terrainBlock, int zoom, int worldXOffset, int worldYOffset,
                          terrainPoint world[WORLD_SIZE][WORLD_SIZE], int& mouse_i, int& mouse_j, bool& mouseOnMap);
void handleDragging(bool& mouseIsDragging, const Vector2& mouseDelta, int& worldXOffset, int& worldYOffset, const Texture& terrainBlock);
void handleHeldItemSwitch(Texture& heldItem, const Texture& cube, const Texture& terrainBlock, const Texture& cobblestone, const Texture& tree);
void drawWorld(terrainPoint world[WORLD_SIZE][WORLD_SIZE], const terrainTextures& textures, int zoom, int worldXOffset, int worldYOffset, int mouse_i, int mouse_j, Texture heldItem);