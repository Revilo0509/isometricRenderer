#include "worldgen.hpp"

Color rgb(int r, int g, int b, int a) {
    return Color{ (unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a };
}

void generateWorld(terrainPoint* world, Texture tree) {
    siv::PerlinNoise perlin{ std::random_device{}() };
    siv::PerlinNoise perlin2{ std::random_device{}() + 1 };
    double frequency = 4.0;
    int octaves = 2;
    int treeCutOff = 60;

    for (int i = 0; i < WORLD_SIZE; ++i) {
        for (int j = 0; j < WORLD_SIZE; ++j) {
            double f = frequency / WORLD_SIZE;
            double noise = perlin.octave2D_01(i * f, j * f, octaves);
            world[i * WORLD_SIZE + j].height = noise * 80.0;
            double noise2 = perlin2.octave2D_01(i * f, j* f, octaves) * 100;
            if (noise2 > treeCutOff) { world[i * WORLD_SIZE + j].landmark = tree; } else { world[i * WORLD_SIZE + j].landmark = noTextureImg; };
            
        };
    };
};