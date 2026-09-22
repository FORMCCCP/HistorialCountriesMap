#pragma once

#include <QtGlobal>

// 瓦片的基本类
struct Tile{
    int level = 0;  // 层级
    int col = 0;    // 列号
    int row = 0;    // 行号
    std::uint16_t id;   // id标记

    Tile(int l, int c, int r) : level(l),col(c),row(r){
        id = static_cast<std::uint16_t>((level<<12)|(col<<6)|row);
    }

};
