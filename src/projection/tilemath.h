#pragma once

#include <QRectF>

class TileMath
{
public:
    static constexpr double PI = 3.14159265358979323846;    // 圆周率
    static constexpr double EARTH_RADIUS = 6378137.0;       // 地球半径

    static int tileFromWorldX(const double worldX, int level);  // 获取瓦片列
    static int tileFromWorldY(const double worldY, int level);  // 获取瓦片行

    static QRectF tileWorldRect(const std::uint16_t id);    // 获取瓦片的包围盒

    // 拖动范围限制
    static void boundaryRestriction(double& centerX, double& centerY, double& scale, double width, double height);

    // 可拖动范围
    static constexpr double minX = - PI * EARTH_RADIUS * 3 / 2;
    static constexpr double minY = - PI * EARTH_RADIUS;
    static constexpr double maxX = PI * EARTH_RADIUS * 3 / 2;
    static constexpr double maxY = PI * EARTH_RADIUS;
};


