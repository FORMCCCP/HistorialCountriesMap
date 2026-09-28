#include "tilestore.h"

#include <QDebug>
#include <QCoreApplication>

TileStore::TileStore() {}


QString TileStore::tilePath(const std::uint16_t id){
    // 获取id的文件地址
    int level = returnLevel(id);
    int col = returnCol(id);
    int row = returnRow(id);
    QString dir = QCoreApplication::applicationDirPath() + "/../../data/";
    return dir + QStringLiteral("terrain_tiles/z%1/%2_%3.jpg").arg(level).arg(col).arg(row);
}

QImage TileStore::loadTile(const std::uint16_t& id){
    // 加载瓦片图片
    const QString path = tilePath(id);
    QImage img(path);
    if(img.isNull()){
        qDebug() << "瓦片图片加载错误 : " << path;
    }
    qDebug() << "瓦片图片加载成功 : " << path;
    return img;
}