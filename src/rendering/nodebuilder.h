#pragma once
#include <QQuickItem>
#include <QSGTexture>

class NodeBuilder
{
public:
    NodeBuilder();

    void buildTilesNodes(QSGNode* tilesNode, QVector<std::uint16_t>& textures, QHash<std::uint16_t, QSGTexture*>& hashTexture);     //建立瓦片节点
    void buildRegionNodes(QSGNode* regionNode);    //建立区域节点

    QSGGeometryNode* buildTileNode(QSGTexture* tex, const QRectF& world);   // 对一个瓦片进行节点创建
};


