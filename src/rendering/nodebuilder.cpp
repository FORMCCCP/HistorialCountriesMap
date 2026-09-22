#include "nodebuilder.h"

#include <QSGNode>
#include <QSGFlatColorMaterial>
NodeBuilder::NodeBuilder() {}



void NodeBuilder::buildTilesNodes(){


}

void NodeBuilder::buildRegionNodes(QSGNode* regionNode){;
    // 创建区域节点

    QSGGeometry* g = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 3);
    QSGGeometry::Point2D* v= g->vertexDataAsPoint2D();
    v[0].x = 0;
    v[0].y = 0;
    v[1].x = 100;
    v[1].y = 0;
    v[2].x = 100;
    v[2].y = 20;
    g->setDrawingMode(QSGGeometry::DrawTriangles);  // 三角形绘制

    // 设置纯色材质
    QSGFlatColorMaterial* mat = new QSGFlatColorMaterial();
    mat->setColor(Qt::red);

    QSGGeometryNode* Snode = new QSGGeometryNode();
    Snode->setGeometry(g);
    Snode->setMaterial(mat);

    regionNode->appendChildNode(Snode);

}