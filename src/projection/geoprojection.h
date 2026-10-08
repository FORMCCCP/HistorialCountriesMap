#pragma once
#include "region.h"

class GeoProjection
{
public:
    static constexpr double PI = 3.14159265358979323846;    // 圆周率
    static constexpr double EARTH_RADIUS = 6378137.0;       // 地球半径

    static Point LonLatToWorld(const Point& lonLat);    // 经纬转换投影

    // 批量处理
    static std::vector<Point> LonLatToWorld(const std::vector<Point>& poly);
};

