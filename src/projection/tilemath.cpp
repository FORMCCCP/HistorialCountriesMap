#include "tilemath.h"
#include "Tile.h"
#include <algorithm>
#include <QDebug>


// 世界坐标转换瓦片编号
int TileMath::tileFromWorldX(double worldX, int level){
    const double ww = 2 * PI * EARTH_RADIUS;
    const double u = worldX / ww + 0.5; // x坐标在总周长上的比例位置
    const int cols = 1 << (level + 1);  // 2n 列
    return std::clamp(static_cast<int>(u * cols), 0, cols-1);
}
int TileMath::tileFromWorldY(double worldY, int level){
    const double wh = PI * EARTH_RADIUS;
    const double u = 0.5 - worldY / wh;
    const int n = 1 << level;
    return std::clamp(static_cast<int>(u * n), 0, n-1);
}

QRectF TileMath::tileWorldRect(const std::uint16_t id){
    const double ww = 2 * PI * EARTH_RADIUS;
    const double wh = PI * EARTH_RADIUS;

    const int level = returnLevel(id);
    const int col = returnCol(id);
    const int row = returnRow(id);

    const int rows = 1 << level;
    const int cols = 1 << (level + 1);
    const double tileSizeW = ww / cols; // 瓦片边长
    const double tileSizeH = wh / rows;


    const double left = -ww / 2.0 + col * tileSizeW;         // 左边(左上角x坐标)
    const double top = wh / 2.0 - (row + 1) * tileSizeH;     // 顶边(左上角y坐标)

    return QRectF(left, top, tileSizeW, tileSizeH);
}


void TileMath::boundaryRestriction(double& centerX, double& centerY, double& scale, double width, double height){
    // 拖动范围限制

    const double halfWindowWidthWorld = width / (2.0 * scale);  // 窗口在世界坐标下的宽度的一半
    const double boundsWidth = maxX - minX; // 可拖动范围的宽度

    if(boundsWidth > 2.0 * halfWindowWidthWorld){
        // 如果可拖动宽度大于窗口宽度
        // 把视图中心点限制在范围内，已达成限制移动的功能
        centerX = std::clamp(centerX, minX , maxX );
    }


    // y轴部分
    const double halfWindowHeightWorld = height / (2.0 * scale);
    const double doundsHeight = maxY - minY;

    if(doundsHeight > 2.0 * halfWindowHeightWorld){
        centerY = std::clamp(centerY, minY + halfWindowHeightWorld, maxY - halfWindowHeightWorld);
    }
}