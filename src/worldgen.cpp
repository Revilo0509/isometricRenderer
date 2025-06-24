#include "worldgen.hpp"

void generateWorld(terrainPoint* world) {
    siv::PerlinNoise perlin{ std::random_device{}() };
    siv::PerlinNoise perlin2{ std::random_device{}() + 1 };
    double frequency = 4.0;
    int octaves = 2;
    int treeCutOff = 60;

    int maxHeight = 10;

    for (int i = 0; i < WORLD_SIZE; ++i) {
        for (int j = 0; j < WORLD_SIZE; ++j) {
            double f = frequency / WORLD_SIZE;
            double noise = perlin.octave2D_01(i * f, j * f, octaves);
            int height = static_cast<int>(noise * maxHeight + 0.5); // round to nearest int
            world[i * WORLD_SIZE + j].height = height;
            double noise2 = perlin2.octave2D_01(i * f, j * f, octaves) * 100;
            // if (noise2 > treeCutOff) { world[i * WORLD_SIZE + j].landmark = tree; } else { world[i * WORLD_SIZE + j].landmark = emptyTexture; };
        }
    }
}
