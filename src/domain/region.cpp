#include "region.h"
#include "geoprojection.h"
#include "geotriangulate.h"




bool Ring::activeAt(int year) const{
    if(year < startYear || year > endYear){
        return false;
    }
    return true;
}

bool Region::activeAt(int year) const{
    if(year < startYear || year > endYear){
        return false;
    }
    return true;
}

Box Region::boundingBox() const{
    // 计算一个区域的包围盒
    Box out;
    bool first = true;
    for(const Polygon& p: polygons){
        const Box b = ringBox(p.out.worlds);    // 计算单个多边形外环的包围盒
        if(first){
            out = b;
        }else{
            out.minX = std::min(out.minX, b.minX);
            out.minY = std::min(out.minY, b.minY);
            out.maxX = std::max(out.maxX, b.maxX);
            out.maxY = std::max(out.maxY, b.maxY);
        }
    }
    return out;
}


void Region::prepareWorldProjection(){
    // 预处理：经纬度转化 + 三角化处理

    // 多边形的外环和内环的经纬度进行投影转换
    for(Polygon& p: this->polygons){
        // p.out.worlds.clear();
        // p.out.worlds.reserve(p.out.points.size());
        p.out.worlds = GeoProjection::LonLatToWorld(p.out.points);

        for(Ring& r : p.holes){
            // r.worlds.clear();
            // r.worlds.reserve(r.points.size());
            r.worlds = GeoProjection::LonLatToWorld(r.points);
        }
    }

    // 包围盒
    worldBox = boundingBox();

    // 三角化
    this->worldTriangles.clear();
    for(const Polygon& p: polygons){
        triangulateWorldPolygon(p.out.worlds, p.holes, worldTriangles);
    }
}






Box ringBox(const std::vector<Point>& ring){
    // 计算一个环的包围盒
    Box b;
    bool first = true;
    for(const Point& p: ring){
        if(first){
            b.minX = b.maxX = p.x;
            b.minY = b.maxY = p.y;
            first = false;
        }else{
            b.minX = std::min(b.minX, p.x);
            b.minY = std::min(b.minY, p.y);
            b.maxX = std::max(b.maxX, p.x);
            b.maxY = std::max(b.maxY, p.y);
        }
    }
    return b;
}
