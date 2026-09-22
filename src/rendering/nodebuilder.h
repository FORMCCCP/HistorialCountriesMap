#pragma once
#include <QQuickItem>

class NodeBuilder
{
public:
    NodeBuilder();

    void buildTilesNodes();     //建立瓦片节点
    void buildRegionNodes(QSGNode* regionNode);    //建立区域节点

};


