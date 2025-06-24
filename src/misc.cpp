#include "misc.hpp"

Color rgb(int r, int g, int b, int a) {
    return Color{ (unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a };
}

bool allSame(std::initializer_list<int> values) {
    if (values.size() < 2) return false;
    int first = *values.begin();
    for (int val : values) {
        if (val != first) return false;
    }
    return true;
}