#pragma once

#include <vector>
#include <string>

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





// 环结构，包括外环或者内环
struct Ring
{
    std::vector<Point> points;  // 经纬度坐标
    std::vector<Point> worlds;  // 处理后的投影坐标

    // 该环的起始时间和终点
    int startYear;
    int endYear;

    bool activeAt(int year) const;    // 判断该环在某年是否存在
};

// 多边形结构(一个完整的多边形: 一个外环 + 若干个内环/洞)
struct Polygon{
    Ring out;   // 唯一外环
    std::vector<Ring> holes;    // 若干内环洞
};

// 区域结构
struct Region{
    std::string id;             // id
    std::string name;           // 区域名
    std::string description;    // 区域描述
    std::string color;          // 区域显示颜色

    int startYear = -9999;
    int endYear = 9999;

    std::vector<Polygon> polygons;  // 一个区域带有多个多边形
    Box worldBox;
    std::vector<Point> worldTriangles;  // 三角化

    bool activeAt(int year) const;  // 判断该区域在某年是否存在
    Box boundingBox() const;        // 经纬度包围盒
    void prepareWorldProjection();  // 预处理
};



// ====================================

// 计算某个多边形的包围盒
Box ringBox(const std::vector<Point> &ring);