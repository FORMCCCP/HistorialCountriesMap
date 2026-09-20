#pragma once



// 二维坐标
struct Point
{
    double x = 0.0;
    double y = 0.0;
};

// 包围盒，能包裹整个区域的且平行于坐标轴的最小矩形
struct Box
{
    double minX = 0.0;
    double maxX = 0.0;
    double minY = 0.0;
    double maxY = 0.0;

    double width() const{return maxX - minX;}
    double height() const{return maxY - minY;}
    // 判断某点是否在该包围盒里
    bool contains(const Point& point) const{return point.x >= minX
                                                && point.x <= maxX
                                                && point.y >= minY
                                                && point.y <= maxY;}
};