#include "nodebuilder.h"
#include "tilemath.h"
#include <QSGNode>
#include <QSGFlatColorMaterial>
#include <QSGOpaqueTextureMaterial>
#include <QRectF>
NodeBuilder::NodeBuilder() {}



void NodeBuilder::buildTilesNodes(QSGNode* tilesNode, QVector<std::uint16_t>& texture, QHash<std::uint16_t, QSGTexture*>& hashTexture){
    // 建立瓦片节点
    tilesNode->removeAllChildNodes();

    for(int i = 0; i<texture.size(); i++){
        const std::uint16_t id = texture[i];

        if(!hashTexture.contains(id)) continue;

        QSGTexture* tex = hashTexture[id];              // 获取纹理对象
        const QRectF world = TileMath::tileWorldRect(id);     // 获取该瓦片的世界范围坐标

        QSGGeometryNode* node = buildTileNode(tex, world);
        if(node){
            tilesNode->appendChildNode(node);
        }
    }

}

QSGGeometryNode* NodeBuilder::buildTileNode(QSGTexture* tex, const QRectF& world){
    // 创建单一瓦片节点

    QSGGeometry* g = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 6);
    QSGGeometry::TexturedPoint2D* v = g->vertexDataAsTexturedPoint2D();

    v[0].set(world.left(), world.bottom(), 0.0f, 0.0f);
    v[1].set(world.right(), world.bottom(), 1.0f, 0.0f);
    v[2].set(world.right(), world.top(), 1.0f, 1.0f);
    v[3].set(world.left(), world.bottom(), 0.0f, 0.0f);
    v[4].set(world.right(), world.top(), 1.0f, 1.0f);
    v[5].set(world.left(), world.top(), 0.0f, 1.0f);

    g->setDrawingMode(QSGGeometry::DrawTriangles);
    QSGOpaqueTextureMaterial* mat = new QSGOpaqueTextureMaterial();
    mat->setTexture(tex);

    QSGGeometryNode* node = new QSGGeometryNode();
    node->setGeometry(g);
    node->setMaterial(mat);

    return node;
}



void NodeBuilder::buildRegionNodes(QSGNode* regionNode){;
    // 创建区域节点

    QSGGeometry* g = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 3);
    QSGGeometry::Point2D* v= g->vertexDataAsPoint2D();
    v[0].x = 0;
    v[0].y = 0;
    v[1].x = 8000000;
    v[1].y = 0;
    v[2].x = 8000000;
    v[2].y = 2000000;
    g->setDrawingMode(QSGGeometry::DrawTriangles);  // 三角形绘制

    // 设置纯色材质
    QSGFlatColorMaterial* mat = new QSGFlatColorMaterial();
    mat->setColor(Qt::red);

    QSGGeometryNode* Snode = new QSGGeometryNode();
    Snode->setGeometry(g);
    Snode->setMaterial(mat);

    regionNode->appendChildNode(Snode);

}