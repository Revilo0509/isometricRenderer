#include <iostream>
#include <math.h>
#include <vector>


#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib.h"
#include "resource_dir.h"

#include "gameClasses.hpp"
#include "gameGlobalVariables.h"
#include "other.hpp"

int main () {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "\"God\" Sim");
    InitAudioDevice();
    SearchAndSetResourceDir("resources");

    Sound placeSound = LoadSound("click.wav");

    const Texture cube = LoadTexture("cube.png");
    const Texture terrainBlock = LoadTexture("terrainBlock.png");
    const Texture cobblestone = LoadTexture("cobblestone.png");
    const Texture tree = LoadTexture("tree.png");
    
    Texture heldItem = cube;

    Vector2 mousePos = { 0.0f, 0.0f };
    Vector2 mouseDelta = { 0.0f, 0.0f };
    bool mouseIsDragging = false;

    int zoom = 1;
    int minZoom = 1;
    int maxZoom = 4;
    float scrollSpeed = 1.0f;

    int worldXOffset = 0;
    int worldYOffset = 0;

    terrainPoint world[WORLD_SIZE][WORLD_SIZE];
    generateWorld(world, tree);

    while (!WindowShouldClose()) {

        zoom += (int)(GetMouseWheelMove() * scrollSpeed);
        zoom = std::clamp(zoom, minZoom, maxZoom);

        mousePos = GetMousePosition();
        mouseDelta = GetMouseDelta();
        
        // Convert Mouse Position to world grid position
        int mouse_i = -1;
        int mouse_j = -1;
        bool mouseOnMap = false;
        float best_dist = 30.0f;

        for (int i = 0; i < WORLD_SIZE; i++) {
            for (int j = 0; j < WORLD_SIZE; j++) {
                int x = ((i - j) * (terrainBlock.width / 2) * zoom + WINDOW_WIDTH / 2 - (terrainBlock.width * zoom) / 2) + worldXOffset;
                int y = (((i + j) * (terrainBlock.height / 4) * zoom + WINDOW_HEIGHT / 2 - (terrainBlock.height * zoom * WORLD_SIZE) / 4) + world[i][j].height * zoom) + worldYOffset;

                float dx = mousePos.x - (x + (terrainBlock.width * zoom) / 2);
                float dy = mousePos.y - (y + (terrainBlock.height * zoom) / 2);
                float dist = sqrtf(dx * dx + dy * dy);

                if (dist < best_dist) {
                    best_dist = dist;
                    mouse_i = i;
                    mouse_j = j;
                    mouseOnMap = true;
                };
            };
        };

        // Allow for draging around the world
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            mouseIsDragging = true;
        } else if (IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)) {
            mouseIsDragging = false;
        }

        if (mouseIsDragging) {
            worldXOffset += (int)mouseDelta.x;
            worldYOffset += (int)mouseDelta.y;
            worldXOffset = std::clamp(worldXOffset, -(((terrainBlock.width / 2) * WORLD_SIZE) + terrainBlock.width * 8), ((terrainBlock.width / 2) * WORLD_SIZE) + terrainBlock.width * 8);
            worldYOffset = std::clamp(worldYOffset, -(((terrainBlock.height / 4) * WORLD_SIZE) + terrainBlock.height * 8), ((terrainBlock.height / 4) * WORLD_SIZE) + terrainBlock.height * 8);
        };

        // Swich Active Place Item thingi
        if (IsKeyPressed(KEY_ONE)) { heldItem = cube; }
        else if (IsKeyPressed(KEY_TWO)) { heldItem = terrainBlock; }
        else if (IsKeyPressed(KEY_THREE)) { heldItem = cobblestone; }
        else if (IsKeyPressed(KEY_FOUR)) { heldItem = tree; };

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mouseOnMap) { world[mouse_j][mouse_i].landmark = heldItem; PlaySound(placeSound); };

        BeginDrawing();
            ClearBackground(rgb(88, 134, 193));

            // Draw the world
            for (int i = 0; i < WORLD_SIZE; i++) {
                for (int j = 0; j < WORLD_SIZE; j++) {
                    // Calculate Screen Position
                    int x = ((i - j) * (terrainBlock.width / 2) * zoom + WINDOW_WIDTH / 2 - (terrainBlock.width * zoom) / 2) + worldXOffset;
                    int y = (((i + j) * (terrainBlock.height / 4) * zoom + WINDOW_HEIGHT / 2 - (terrainBlock.height * zoom * WORLD_SIZE) / 4) + world[i][j].height * zoom) + worldYOffset;
                    Rectangle source = { 0, 0, terrainBlock.width, terrainBlock.height };
                    Rectangle terrainPlace = {x, y, terrainBlock.width * zoom, terrainBlock.height * zoom };
                    Rectangle landmarkPlace = { x, (y - (terrainBlock.height * zoom) / 2) + 1, terrainBlock.width * zoom, terrainBlock.height * zoom };
                    
                    // Highligt the hovered block
                    if (i == mouse_i && j == mouse_j) { 
                        DrawTexturePro(terrainBlock, source, terrainPlace, {0, 0}, 0.0f, rgb(130, 207, 216)); 
                    } else { 
                        DrawTexturePro(terrainBlock, source, terrainPlace, {0, 0}, 0.0f, WHITE); 
                    };

                    // Draw the item on top
                    DrawTexturePro(world[j][i].landmark, source, landmarkPlace, {0, 0}, 0.0f, WHITE);
                };
            };
            
            DrawTexture(heldItem, 10, 10, WHITE);
            
            if (GuiButton(Rectangle{ WINDOW_WIDTH - 130, 10, 120, 30 }, "Regenerate World")) { generateWorld(world, tree); };
            

        EndDrawing();
    }

    UnloadSound(placeSound);

    UnloadTexture(cube);
    UnloadTexture(terrainBlock);
    UnloadTexture(cobblestone);
    UnloadTexture(tree);
    UnloadTexture(noTextureImg);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}