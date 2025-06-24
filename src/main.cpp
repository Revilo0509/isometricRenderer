#include "main.hpp"

int main() {
    init();

    // Load resources
    Sound placeSound = LoadSound("click.wav");
    const Texture cube = LoadTexture("cube.png");
    const Texture terrainBlock = LoadTexture("terrainBlock.png");
    const Texture cobblestone = LoadTexture("cobblestone.png");
    const Texture tree = LoadTexture("tree.png");
    const Texture noTexture = LoadTexture("no_texture");
    Texture heldItem = cube;

    const terrainTextures grassTextures = { "terrain", "block.png", "dirt.png", "slope_E.png", "slope_N.png", "slope_S.png", "slope_w.png", "corner_EN.png", "corner_NW.png", "corner_SE.png", "corner_WS.png", "cornerDip_EN.png", "cornerDip_NW.png", "cornerDip_SE.png", "cornerDip_WS.png" };

    // State variables
    Vector2 mousePos = { 0.0f, 0.0f };
    Vector2 mouseDelta = { 0.0f, 0.0f };
    bool mouseIsDragging = false;
    int zoom = 1, minZoom = 1, maxZoom = 4;
    float scrollSpeed = 1.0f;
    int worldXOffset = 0, worldYOffset = 0;

    terrainPoint world[WORLD_SIZE][WORLD_SIZE];
    generateWorld(*world, tree);

    // --- Main Loop ---
    while (!WindowShouldClose()) {
        handleZoom(zoom, minZoom, maxZoom, scrollSpeed);
        handleMouse(mousePos, mouseDelta);

        int mouse_i, mouse_j;
        bool mouseOnMap;
        getMouseGridPosition(mousePos, terrainBlock, zoom, worldXOffset, worldYOffset, world, mouse_i, mouse_j, mouseOnMap);

        handleDragging(mouseIsDragging, mouseDelta, worldXOffset, worldYOffset, terrainBlock);

        handleHeldItemSwitch(heldItem, cube, terrainBlock, cobblestone, tree);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mouseOnMap) {
            world[mouse_j][mouse_i].landmark = heldItem;
            PlaySound(placeSound);
        }

        BeginDrawing();
            ClearBackground(rgb(88, 134, 193));
            drawWorld(world, grassTextures, zoom, worldXOffset, worldYOffset, mouse_i, mouse_j, heldItem);
            DrawTexture(heldItem, 10, 10, WHITE);
            if (GuiButton(Rectangle{ WINDOW_WIDTH - 130, 10, 120, 30 }, "Regenerate World")) {
                generateWorld(*world, tree);
            }
            DrawTexture(grassTextures.corner_EN, 10, 50, WHITE);

        EndDrawing();
    }

    // Cleanup
    UnloadSound(placeSound);
    UnloadTexture(cube);
    UnloadTexture(terrainBlock);
    UnloadTexture(cobblestone);
    UnloadTexture(tree);
    UnloadTexture(emptyTexture);
    UnloadTexture(noTexture);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}

// --- Helper Functions ---

void handleZoom(int& zoom, int minZoom, int maxZoom, float scrollSpeed) {
    zoom += (int)(GetMouseWheelMove() * scrollSpeed);
    zoom = std::clamp(zoom, minZoom, maxZoom);
}

void handleMouse(Vector2& mousePos, Vector2& mouseDelta) {
    mousePos = GetMousePosition();
    mouseDelta = GetMouseDelta();
}

void getMouseGridPosition(const Vector2& mousePos, const Texture& terrainBlock, int zoom, int worldXOffset, int worldYOffset,
                          terrainPoint world[WORLD_SIZE][WORLD_SIZE], int& mouse_i, int& mouse_j, bool& mouseOnMap) {
    mouse_i = -1;
    mouse_j = -1;
    mouseOnMap = false;
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
            }
        }
    }
}

void handleDragging(bool& mouseIsDragging, const Vector2& mouseDelta, int& worldXOffset, int& worldYOffset, const Texture& terrainBlock) {
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
    }
}

void handleHeldItemSwitch(Texture& heldItem, const Texture& cube, const Texture& terrainBlock, const Texture& cobblestone, const Texture& tree) {
    if (IsKeyPressed(KEY_ONE)) { heldItem = cube; }
    else if (IsKeyPressed(KEY_TWO)) { heldItem = terrainBlock; }
    else if (IsKeyPressed(KEY_THREE)) { heldItem = cobblestone; }
    else if (IsKeyPressed(KEY_FOUR)) { heldItem = tree; }
}

void drawWorld(terrainPoint world[WORLD_SIZE][WORLD_SIZE], const terrainTextures& textures, int zoom, int worldXOffset, int worldYOffset, int mouse_i, int mouse_j, Texture heldItem) {
    for (int i = 0; i < WORLD_SIZE; i++) {
        for (int j = 0; j < WORLD_SIZE; j++) {
            // Highlight selected block
            Color highlight = WHITE;
            if (i == mouse_i && j == mouse_j) {
                highlight = rgb(130, 207, 216);
            }

            // Defensive: Clamp neighbor indices
            int ourHeight = world[i][j].height;
            int overHeight   = (i > 0)                ? world[i-1][j].height   : ourHeight;
            int rightHeight  = (j < WORLD_SIZE - 1)   ? world[i][j+1].height   : ourHeight;
            int bottomHeight = (i < WORLD_SIZE - 1)   ? world[i+1][j].height   : ourHeight;
            int leftHeight   = (j > 0)                ? world[i][j-1].height   : ourHeight;

            // Choose the correct texture
            Texture currentTexture = textures.block;
            bool isSlopeOrCorner = false;

            // Flat
            if (ourHeight == overHeight && ourHeight == rightHeight && ourHeight == bottomHeight && ourHeight == leftHeight) {
                currentTexture = textures.block;
            }
            // Slopes
            else if (ourHeight - overHeight == 1 && rightHeight == ourHeight && bottomHeight == ourHeight && leftHeight == ourHeight) {
                currentTexture = textures.slope_W;
                isSlopeOrCorner = true;
            }
            else if (ourHeight - rightHeight == 1 && overHeight == ourHeight && bottomHeight == ourHeight && leftHeight == ourHeight) {
                currentTexture = textures.slope_S;
                isSlopeOrCorner = true;
            }
            else if (ourHeight - bottomHeight == 1 && overHeight == ourHeight && rightHeight == ourHeight && leftHeight == ourHeight) {
                currentTexture = textures.slope_E;
                isSlopeOrCorner = true;
            }
            else if (ourHeight - leftHeight == 1 && overHeight == ourHeight && rightHeight == ourHeight && bottomHeight == ourHeight) {
                currentTexture = textures.slope_N;
                isSlopeOrCorner = true;
            }
            // Corners
            else if (ourHeight - overHeight == 1 && ourHeight - rightHeight == 1 && bottomHeight == ourHeight && leftHeight == ourHeight) {
                currentTexture = textures.corner_WS;
                isSlopeOrCorner = true;
            }
            else if (ourHeight - overHeight == 1 && ourHeight - leftHeight == 1 && rightHeight == ourHeight && bottomHeight == ourHeight) {
                currentTexture = textures.corner_NW;
                isSlopeOrCorner = true;
            }
            else if (ourHeight - bottomHeight == 1 && ourHeight - rightHeight == 1 && overHeight == ourHeight && leftHeight == ourHeight) {
                currentTexture = textures.corner_SE;
                isSlopeOrCorner = true;
            }
            else if (ourHeight - bottomHeight == 1 && ourHeight - leftHeight == 1 && overHeight == ourHeight && rightHeight == ourHeight) {
                currentTexture = textures.corner_EN;
                isSlopeOrCorner = true;
            }
            // Corner dips
            else if (overHeight - ourHeight == 1 && rightHeight - ourHeight == 1 && bottomHeight == ourHeight && leftHeight == ourHeight) {
                currentTexture = textures.cornerDip_WS;
            }
            else if (overHeight - ourHeight == 1 && leftHeight - ourHeight == 1 && rightHeight == ourHeight && bottomHeight == ourHeight) {
                currentTexture = textures.cornerDip_NW;
            }
            else if (bottomHeight - ourHeight == 1 && rightHeight - ourHeight == 1 && overHeight == ourHeight && leftHeight == ourHeight) {
                currentTexture = textures.cornerDip_SE;
            }
            else if (bottomHeight - ourHeight == 1 && leftHeight - ourHeight == 1 && overHeight == ourHeight && rightHeight == ourHeight) {
                currentTexture = textures.cornerDip_EN;
            }
            // Default
            else {
                currentTexture = textures.block;
            }

            // Calculate isometric screen position for this block
            int blockWidth = textures.block.width;
            int blockHeight = textures.block.height;

            // The vertical offset for the block is always based on its height and the block's vertical step
            int x = ((i - j) * (blockWidth / 2) * zoom)
                    + WINDOW_WIDTH / 2
                    - (blockWidth * zoom) / 2
                    + worldXOffset;

            // Offset for slopes/corners: move up by one block height
            int slopeOffset = isSlopeOrCorner ? (blockHeight / 4) * zoom : 0;

            int y = ((i + j) * (blockHeight / 4) * zoom)
                    + WINDOW_HEIGHT / 2
                    - (blockHeight * zoom * WORLD_SIZE) / 4
                    + (world[i][j].height * (blockHeight / 4)) * zoom
                    - slopeOffset
                    + worldYOffset;

            // Source rectangle (entire texture)
            Rectangle source = {
                0.0f,
                0.0f,
                (float)blockWidth,
                (float)blockHeight
            };

            // Where to draw the terrain block
            Rectangle terrainPlace = {
                (float)x,
                (float)y,
                (float)(blockWidth * zoom),
                (float)(blockHeight * zoom)
            };

            // Where to draw the landmark/object (slightly above the block)
            Rectangle landmarkPlace = {
                (float)x,
                (float)((((y - (zoom ^ 2)) - (blockHeight * zoom) / 2) + 1)) + blockHeight / 4,
                (float)(blockWidth * zoom),
                (float)(blockHeight * zoom)
            };

            DrawTexturePro(currentTexture, source, terrainPlace, {0, 0}, 0.0f, highlight);


            DrawTexturePro(world[j][i].landmark, source, landmarkPlace, {0, 0}, 0.0f, WHITE);
        }
    }
}

void init() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "\"God\" Sim");
    InitAudioDevice();
    SearchAndSetResourceDir("resources");
};