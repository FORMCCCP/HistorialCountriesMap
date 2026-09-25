#pragma once

#include <QString>
#include <QImage>

#include <Tile.h>

class TileStore
{
public:
    explicit TileStore();

    QImage loadTile(const std::uint16_t& id);   // 加载瓦片

private:
    QString tilePath(const std::uint16_t id);   // 组成瓦片的文件路径

};


