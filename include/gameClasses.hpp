#pragma once

#include "raylib.h"
#include "iostream"

extern const Texture emptyTexture;

class object {
    public:
        Texture texture;
        int x;
        int z;

        object(int initX = 0, int initZ = 0, Texture initTexture = emptyTexture) {
            texture = initTexture;
            x = initX;
            z = initZ;
        }
};

class terrainTextures {
    public: 
        Texture block;
        Texture dirt;

        Texture slope_E;
        Texture slope_N;
        Texture slope_S;
        Texture slope_W;

        Texture corner_EN;
        Texture corner_NW;
        Texture corner_SE;
        Texture corner_WS;

        Texture cornerDip_EN;
        Texture cornerDip_NW;     
        Texture cornerDip_SE;
        Texture cornerDip_WS;

        terrainTextures(std::string resourseFolder, std::string initBlock, std::string initDirt, std::string initSlope_E, std::string initSlope_N, std::string initSlope_S, std::string initSlope_W, 
                                                                           std::string initCorner_EN, std::string initCorner_NW, std::string initCorner_SE, std::string initCorner_WS,
                                                                           std::string initCornerDip_EN, std::string initCornerDip_NW, std::string initCornerDip_SE, std::string initCornerDip_WS ) {
            block = LoadTexture((resourseFolder + "/" + initBlock).c_str());
            dirt = LoadTexture((resourseFolder + "/" + initDirt).c_str());

            slope_E = LoadTexture((resourseFolder + "/" + initSlope_E).c_str());
            slope_N = LoadTexture((resourseFolder + "/" + initSlope_N).c_str());
            slope_S = LoadTexture((resourseFolder + "/" + initSlope_S).c_str());
            slope_W = LoadTexture((resourseFolder + "/" + initSlope_W).c_str());

            corner_EN = LoadTexture((resourseFolder + "/" + initCorner_EN).c_str());
            corner_NW = LoadTexture((resourseFolder + "/" + initCorner_NW).c_str());
            corner_SE = LoadTexture((resourseFolder + "/" + initCorner_SE).c_str());
            corner_WS = LoadTexture((resourseFolder + "/" + initCorner_WS).c_str());

            cornerDip_EN = LoadTexture((resourseFolder + "/" + initCornerDip_EN).c_str());
            cornerDip_NW = LoadTexture((resourseFolder + "/" + initCornerDip_NW).c_str());
            cornerDip_SE = LoadTexture((resourseFolder + "/" + initCornerDip_SE).c_str());
            cornerDip_WS = LoadTexture((resourseFolder + "/" + initCornerDip_WS).c_str());
        };
        
        ~terrainTextures() {
            UnloadTexture(block);
            UnloadTexture(dirt);
            
            UnloadTexture(slope_E);
            UnloadTexture(slope_N);
            UnloadTexture(slope_S);
            UnloadTexture(slope_W);

            UnloadTexture(corner_EN);
            UnloadTexture(corner_NW);
            UnloadTexture(corner_SE);
            UnloadTexture(corner_WS);

            UnloadTexture(cornerDip_EN);
            UnloadTexture(cornerDip_NW);
            UnloadTexture(cornerDip_SE);
            UnloadTexture(cornerDip_WS);
        };
};

typedef struct {
    int height = 0;
    Texture landmark = emptyTexture; 
    // ^ no idea why it doesn't render the real texture instead of it just being transparent. 
    // But it works so i won't touch it
    // PS: Reminder it also resets in in generateWorld()
} terrainPoint;