#include "tilemath.h"
#include <algorithm>

int TileMath::tileFromWorldX(double worldX, int level){
    const double ws = 2 * PI * EARTH_RADIUS;
    const double u = worldX / ws;
    const int n = 1 << level;
    return std::clamp(static_cast<int>(u * n), 0, n-1);
}
int TileMath::tileFromWorldY(double worldY, int level){
    const double ws = 2 * PI * EARTH_RADIUS;
    const double u = worldY / ws;
    const int n = 1 << level;
    return std::clamp(static_cast<int>(u * n), 0, n-1);
}
