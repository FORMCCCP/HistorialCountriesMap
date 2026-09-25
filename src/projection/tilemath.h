#pragma once


class TileMath
{
public:
    static constexpr double PI = 3.14159265358979323846;    // 圆周率
    static constexpr double EARTH_RADIUS = 6378137.0;       // 地球半径

    static int tileFromWorldX(const double worldX, int level);  // 获取瓦片列
    static int tileFromWorldY(const double worldY, int level);  // 获取瓦片行


};


