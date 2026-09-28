#include "tilemath.h"
#include "Tile.h"
#include <algorithm>
#include <QDebug>


// 世界坐标转换瓦片编号
int TileMath::tileFromWorldX(double worldX, int level){
    const double ws = 2 * PI * EARTH_RADIUS;
    const double u = worldX / ws + 0.5; // x坐标在总周长上的比例位置
    const int n = 1 << level;
    return std::clamp(static_cast<int>(u * n), 0, n-1);
}
int TileMath::tileFromWorldY(double worldY, int level){
    const double ws = 2 * PI * EARTH_RADIUS;
    const double u = 0.5 - worldY / ws;
    const int n = 1 << level;
    return std::clamp(static_cast<int>(u * n), 0, n-1);
}

QRectF TileMath::tileWorldRect(const std::uint16_t id){
    const double ws = 2 * PI * EARTH_RADIUS;    // 世界边长(地球赤道周长)

    const int level = returnLevel(id);
    const int col = returnCol(id);
    const int row = returnRow(id);

    const int n = 1 << level;       // 每行或每列的瓦片数量 0层为1，1层为2，4层为16
    const double tileSize = ws / n; // 瓦片边长


    const double left = -ws / 2.0 + col * tileSize;         // 左边(左上角x坐标)
    const double top = ws / 2.0 - (row + 1) * tileSize;     // 顶边(左上角y坐标)

    return QRectF(left, top, tileSize, tileSize);
}