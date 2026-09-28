#include "renderer.h"
#include <QSGNode>
Renderer::Renderer()
{}
Renderer::~Renderer(){
    releaseTextures();
}
void Renderer::releaseTextures(){
    for(auto tex : m_tileTextures){
        delete tex;
    }
    m_tileTextures.clear();
    m_lru.clear();
}

QSGNode* Renderer::render(QSGNode* oldNode, bool tilesDirty, bool regionsDirty, QVector<std::uint16_t>& visibleTile){
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
        qDebug()<<"瓦片重建";
        r_builer.buildTilesNodes(m_tileNode, visibleTile, m_tileTextures);
    }

    // 区域重建
    if(regionsDirty){
        qDebug()<<"重建区域";
        r_builer.buildRegionNodes(m_regionNode);
    }


    return oldNode;
}


void Renderer::setViewMatrix(QMatrix4x4 matrix){
    // 设置视图矩阵
    m_viewNode->setMatrix(matrix);
    m_viewNode->markDirty(QSGNode::DirtyMatrix);
}

bool Renderer::hasTexture(std::uint16_t id){
    // 缓存中是否有该瓦片
    bool has = m_tileTextures.contains(id);
    if(has){
        updateIDinLRY(id);
    }
    return has;
}

QSGTexture* Renderer::getTexture(std::uint16_t id){
    // 获取已加载的纹理
    if(!m_tileTextures.contains(id)){
        return nullptr;
    }
    return m_tileTextures[id];
}

void Renderer::createTexture(std::uint16_t id, QImage img, QQuickWindow* window){
    // 创建纹理
    QSGTexture* texture = window->createTextureFromImage(img);  // 把图片变成纹理
    m_tileTextures[id] = texture;   // 新纹理添加进缓存
    m_lru.append(id);

    updateLRU();    // 更新缓存的LRU
}



void Renderer::updateLRU(){
    // 更新LRU，并清理缓存

    while(m_lru.size() * (512*512*4) > m_maxTextureBytes){
        // 删除最前面的纹理
        std::uint16_t id = m_lru.first();
        m_lru.removeFirst();

        QSGTexture* tex = m_tileTextures[id];
        delete tex;
        m_tileTextures.remove(id);
    }

    qDebug()<< QString("更新完毕，现在缓存内存为: %1").arg(m_lru.size() * 512);
}

void Renderer::updateIDinLRY(std::uint16_t id){
    // 已存在但需要用的瓦片，把id重新填入RLU中
    if(!m_lru.contains(id)) return;
    m_lru.removeOne(id);
    m_lru.append(id);
}