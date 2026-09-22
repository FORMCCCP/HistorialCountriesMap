#pragma once

#include <QQuickItem>

#include "nodebuilder.h"

// 渲染类
class Renderer
{
public:
    Renderer(NodeBuilder* builer);

    QSGNode* render(QSGNode* oldNode, bool tilesDirty, bool regionsDirty);  // 建立节点
    void setViewMatrix(QMatrix4x4 matrix);

private:
    QSGNode* m_regionNode = nullptr;        // 地区渲染节点
    QSGNode* m_tileNode = nullptr;          // 瓦片渲染节点
    QSGTransformNode* m_viewNode = nullptr; // 视图节点

    NodeBuilder* r_builer = nullptr;
};


