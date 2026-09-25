#pragma once

#include <QQuickItem>
#include <QSGTexture>
#include <QHash>
#include <QList>

#include "nodebuilder.h"

// 渲染类
class Renderer
{
public:
    Renderer();

    QSGNode* render(QSGNode* oldNode, bool tilesDirty, bool regionsDirty);  // 建立节点
    void setViewMatrix(QMatrix4x4 matrix);  // 设置视觉矩阵
    bool hasTexture(std::uint16_t id);  // 查看缓存有没有
    void createTexture(std::uint16_t id, QImage img, QQuickWindow* window);  // 创建纹理

    void buildTerriaNode(QVector<std::uint16_t> tiles);


    void updateLRU();                       // 更新LRU
    void updateIDinLRY(std::uint16_t id);   // 更新某个ID，设置为最新

private:
    QSGNode* m_regionNode = nullptr;        // 地区渲染节点
    QSGNode* m_tileNode = nullptr;          // 瓦片渲染节点
    QSGTransformNode* m_viewNode = nullptr; // 视图节点

    NodeBuilder r_builer;   // 构建器

    QHash<std::uint16_t, QSGTexture*> m_tileTextures;   // 瓦片缓存
    QList<std::uint16_t> m_lru; // 最近最少原则
    size_t m_maxTextureBytes = 512 * 1024 * 1024;   // 缓存的最大内存
    size_t m_currentTextureBytes = 0;   // 当前缓存内存大小
};


