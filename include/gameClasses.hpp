#pragma once

#include "raylib.h"
#include "iostream"
#include "gameGlobalVariables.h"

class object {
    public:
        Texture texture;    
        int x;
        int z;

        object(int initialX = 0, int initialZ = 0, Texture initialTexture = noTextureImg) {
            texture = initialTexture;
            x = initialX;
            z = initialZ;
        }
};

typedef struct {
    int height = 0;
    Texture landmark = noTextureImg; 
    // ^ no idea why it doesn't render the real texture instead of it just being transparent. 
    // But it works so i won't touch it
    // PS: Reminder it also resets in in generateWorld()
} terrainPoint;