#pragma once

#include "raylib.h"
#include <initializer_list>

Color rgb(int r, int g, int b, int a = 255);
bool allSame(std::initializer_list<int> values);