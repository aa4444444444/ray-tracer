#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include <algorithm>
#include <cmath>
#include <vector>

class PerlinNoise {
public:
    PerlinNoise();
    PerlinNoise(unsigned int seed);

    double noise(double x, double y, double z) const;

private:
    std::vector<int> p; // Permutation vector

    double fade(double t) const;
    double lerp(double t, double a, double b) const;
    double grad(int hash, double x, double y, double z) const;
};

#endif // PERLIN_NOISE_H
