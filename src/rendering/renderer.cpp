#include "renderer.h"
#include <QSGNode>
Renderer::Renderer(NodeBuilder* builer)
    : r_builer(builer)
{}


QSGNode* Renderer::render(QSGNode* oldNode, bool tilesDirty, bool regionsDirty){
    // 初始化节点
    if(!oldNode){
        oldNode = new QSGNode();

        m_viewNode = new QSGTransformNode();
        oldNode->appendChildNode(m_viewNode);

        m_tileNode = new QSGNode();
        m_regionNode = new QSGNode();
        m_viewNode->appendChildNode(m_tileNode);
        m_viewNode->appendChildNode(m_regionNode);

        tilesDirty = true;
        regionsDirty = true;
    }


    // 瓦片重建
    if(tilesDirty){

    }

    // 区域重建
    if(regionsDirty){
        qDebug()<<"重建区域";
        r_builer->buildRegionNodes(m_regionNode);
    }


    return oldNode;
}


void Renderer::setViewMatrix(QMatrix4x4 matrix){
    // 设置视图矩阵
    m_viewNode->setMatrix(matrix);
    m_viewNode->markDirty(QSGNode::DirtyMatrix);
}