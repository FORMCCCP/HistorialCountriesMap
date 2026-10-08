#include "geoprojection.h"


Point GeoProjection::LonLatToWorld(const Point& lonLat){
    const double lonRad = lonLat.x * (PI / 180.0);  // 经度转弧度
    const double latRad = lonLat.y * (PI / 180.0);  // 纬度转弧度

    // 等距圆柱投影公式
    const double x = EARTH_RADIUS * lonRad;
    const double y = EARTH_RADIUS * latRad;
    return Point{x,y};
}

std::vector<Point> GeoProjection::LonLatToWorld(const std::vector<Point>& poly){
    // 批量处理一个环的坐标
    std::vector<Point> out;
    out.reserve(poly.size());
    for(const Point& pt : poly){
        out.push_back(LonLatToWorld(pt));
    }
    return out;
}