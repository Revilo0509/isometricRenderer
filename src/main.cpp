#include "main.hpp"

int main() {
    init();

    // Load resources
    Sound placeSound = LoadSound("click.wav");
    const Texture cube = LoadTexture("cube.png");
    const Texture terrainBlock = LoadTexture("terrainBlock.png");
    const Texture cobblestone = LoadTexture("cobblestone.png");
    const Texture tree = LoadTexture("tree.png");
    Texture heldItem = cube;

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
            drawWorld(world, terrainBlock, zoom, worldXOffset, worldYOffset, mouse_i, mouse_j, heldItem);
            DrawTexture(heldItem, 10, 10, WHITE);
            if (GuiButton(Rectangle{ WINDOW_WIDTH - 130, 10, 120, 30 }, "Regenerate World")) {
                generateWorld(*world, tree);
            }
        EndDrawing();
    }

    // Cleanup
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

void drawWorld(terrainPoint world[WORLD_SIZE][WORLD_SIZE], const Texture& terrainBlock, int zoom, int worldXOffset, int worldYOffset, int mouse_i, int mouse_j, Texture heldItem) {
    for (int i = 0; i < WORLD_SIZE; i++) {
        for (int j = 0; j < WORLD_SIZE; j++) {
            int x = ((i - j) * (terrainBlock.width / 2) * zoom + WINDOW_WIDTH / 2 - (terrainBlock.width * zoom) / 2) + worldXOffset;
            int y = (((i + j) * (terrainBlock.height / 4) * zoom + WINDOW_HEIGHT / 2 - (terrainBlock.height * zoom * WORLD_SIZE) / 4) + world[i][j].height * zoom) + worldYOffset;
            Rectangle source = { 0.0f, 0.0f, (float)terrainBlock.width, (float)terrainBlock.height };
            Rectangle terrainPlace = { (float)x, (float)y, (float)(terrainBlock.width * zoom), (float)(terrainBlock.height * zoom) };
            Rectangle landmarkPlace = { (float)x, (float)((y - (terrainBlock.height * zoom) / 2) + 1), (float)(terrainBlock.width * zoom), (float)(terrainBlock.height * zoom) };
            if (i == mouse_i && j == mouse_j) {
                DrawTexturePro(terrainBlock, source, terrainPlace, {0, 0}, 0.0f, rgb(130, 207, 216));
            } else {
                DrawTexturePro(terrainBlock, source, terrainPlace, {0, 0}, 0.0f, WHITE);
            }
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