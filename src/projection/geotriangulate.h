#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include "earcut.hpp"
#include "region.h"


// 三角化
inline void triangulateWorldPolygon(const std::vector<Point>& outer,
                             const std::vector<Ring>& holes,
                             std::vector<Point>& out){
    if(outer.size() < 2) return;

    using P = std::array<double, 2>;
    std::vector<std::vector<P>> rings;
    std::vector<P> coords;

    auto appendRing = [&](const std::vector<Point>& poly){
        std::vector<P> ring;
        ring.reserve(poly.size());
        for(const Point& pt : poly){
            ring.push_back({pt.x, pt.y});
            coords.push_back({pt.x, pt.y});
        }
        rings.push_back(std::move(ring));
    };

    appendRing(outer);

    for(const Ring& r : holes){
        if(r.worlds.size() < 3) continue;
        appendRing(r.worlds);
    }

    const std::vector<std::uint32_t> indices = mapbox::earcut<std::uint32_t>(rings);
    if(indices.empty()) return;
    out.reserve(out.size() + indices.size());
    for(std::uint32_t idx : indices){
        const P& p= coords[idx];
        out.push_back(Point{p[0],p[1]});
    }

}