#include "mapview.h"
#include "geoprojection.h"
#include "tilemath.h"

#include <QSGNode>
#include <QPointF>
#include <QTransform>
#include <QMatrix4x4>
#include <QSGFlatColorMaterial>
#include <QCursor>


#include <QtGlobal>
#include <algorithm>


MapView::MapView(QQuickItem* parent)
    : QQuickItem(parent), first(false){
    setAcceptedMouseButtons(Qt::LeftButton);    // 设置只接受左键
    setFlag(ItemHasContents, true);
    // 打开 ItemHasContents ，让该Item可以调用updatePaintNode()去渲染

    // 设置计时器，处理瓦片任务
    m_timer = new QTimer();
    connect(m_timer, &QTimer::timeout, this, [this](){
        m_dispatcher.processTasks();
    });
    m_timer->start(50);

//========================================

}
MapView::~MapView(){
    delete m_renderer;
}

void MapView::setController(TotalController* c){
    // 设置视图的控制器
    if(c == m_controller) return;

    m_controller = c;
    qDebug() << "Map视图已经获取总控制器";

}


void MapView::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry){
    // 纯虚覆盖函数，窗口改变时触发
    if(first) return;
    first = true;
    m_centerX = 0;
    m_centerY = 0;
    m_scale = fitScale();
    update();
}

void MapView::componentComplete(){
    QQuickItem::componentComplete();
    QQuickWindow* w = window();
    if(w){
        // 渲染线程、QRhi 销毁前同步释放纹理，DirectConnection 保证当场执行
        connect(w, &QQuickWindow::sceneGraphInvalidated, this, [this](){
            m_renderer->releaseTextures();
        }, Qt::DirectConnection);
    }
}

QSGNode* MapView::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*){
    // 图层节点更新总函数 被update()触发调用

    const int level = currentLevel();   // 当前层级
    QVector<std::uint16_t> visibleTile = visibleTiles(level); // 应该渲染的瓦片

    qDebug() << "scale=" << m_scale << "w=" << width() << "h=" << height() << "level=" << level << "所需瓦片数量：" <<visibleTile.size();

    // 比较瓦片，判断是否需要重新渲染瓦片
    if(visibleTile != m_lastTilesID){
        m_lastTilesID = visibleTile;    // 获取新的瓦片
        loadVisibleTiles(visibleTile);  // 加载瓦片
        tilesDirty = true;
    }

    oldNode = m_renderer->render(oldNode, tilesDirty, regionsDirty, visibleTile);    // 委托节点建立

    m_renderer->setViewMatrix(viewMatrix()); // 设置视图节点的变换矩形


    tilesDirty = false;
    regionsDirty = false;
    return oldNode;
}






QMatrix4x4 MapView::viewMatrix()const{
    // 视图节点的变换矩阵

    // tx/ty表示世界原点在屏幕坐标系的坐标位置
    const double tx = -m_centerX * m_scale + width()/2.0;
    const double ty = m_centerY * m_scale + height()/2.0;   // y轴取反
    /*  center * scale 表示的是世界坐标原点在屏幕坐标系中，世界原点到屏幕中心的偏移量
     *  加上 边长/2 得到世界原点在屏幕坐标系中的坐标
     *
     *  平移的核心就是求世界原点在屏幕上的新位置(tx, ty)
     */

    // 把世界坐标原点映射到屏幕坐标系的(tx,ty)
    QTransform t(m_scale, 0, 0, -m_scale, tx, ty);
    return QMatrix4x4(t);
}

QPointF MapView::screenToWorld(const QPointF& screen) const{
    // 屏幕坐标转换成世界坐标
    return QPointF(
        (screen.x() - width() / 2.0) / m_scale + m_centerX,
        m_centerY - (screen.y() - height() / 2.0) / m_scale);
}
QPointF MapView::worldToscreen(const QPointF& world) const{
    // 世界坐标转换成屏幕坐标
    return QPointF(
        (world.x() - m_centerX) * m_scale + width() / 2.0,
        (m_centerY - world.y()) * m_scale + height() / 2.0);
}

double MapView::fitScale()const{
    // 根据当前窗口的标准缩放大小
    const double worldSize = 2.0 * GeoProjection::PI * GeoProjection::EARTH_RADIUS;
    return qMin(width(), height()) / worldSize;
}


int MapView::currentLevel() const{
    // 当前层级 = log2(缩放比例)
    const double fitscale = fitScale();
    const double relative = m_scale / fitscale;

    int level = static_cast<int>(std::log2(relative));

    return std::clamp(level, 0, 6);
}

QVector<std::uint16_t> MapView::visibleTiles(int level) const{
    // 计算当前情况应该用哪些瓦片
    // 左上角右下角的世界坐标
    QPointF topLeftWorld = screenToWorld(QPointF(0, 0));
    QPointF bottomRightWorld = screenToWorld(QPointF(width(), height()));

    int col0 = TileMath::tileFromWorldX(topLeftWorld.x(), level);
    int row0 = TileMath::tileFromWorldY(topLeftWorld.y(), level);
    int col1 = TileMath::tileFromWorldX(bottomRightWorld.x(), level);
    int row1 = TileMath::tileFromWorldY(bottomRightWorld.y(), level);

    // 加一圈冗余
    const int n = 1 << level;
    col0 = qMax(0, col0 - 1);
    row0 = qMax(0, row0 - 1);
    col1 = qMin(n - 1, col1 + 1);
    row1 = qMin(n - 1, row1 + 1);

    // 或取应该渲染的瓦片的id数组
    QVector<std::uint16_t> tiles;
    Tile* t = nullptr;
    for(int row = row0; row <= row1; row++){
        for(int col = col0; col <= col1; col++){
            t = new Tile(level, col, row);
            tiles.append(t->id);
            delete t;
        }
    }
    return  tiles;
}


void MapView::loadVisibleTiles(const QVector<std::uint16_t>& tiles){
    // 加载需要的瓦片，参数为：需要的瓦片的id

    for(size_t i = 0; i < tiles.size(); ++i){
        const std::uint16_t id = tiles[i];

        // renderer的纹理缓存中有，跳过
        if(m_renderer->hasTexture(id)) continue;

        // 正在后台加载，跳过
        if(m_loadingTiles.contains(id)) continue;

        m_loadingTiles.insert(id);  // 标记该瓦片正在从后台加载

        m_tileLoader->load(id, [this](const std::uint16_t id, const QImage& img){
            // 这里的部分是被view从调度器中取出后执行

            m_loadingTiles.remove(id);  // 移除正在加载标记

            if(!img.isNull() && this->window()){
                m_renderer->createTexture(id, img, this->window()); // 在渲染器里建立瓦片纹理
                tilesDirty = true;
                update();
            }
        });
    }
}










// =========================================================

void MapView::mousePressEvent(QMouseEvent* event){
    // 鼠标按压
    if(event->button() != Qt::LeftButton) return;   // 只接受左键


    setCursor(QCursor(Qt::ClosedHandCursor));   // 改变光标
    m_dragging = true;
    m_moved = false;
    m_lastMousePos = event->pos();


    event->accept();
}
void MapView::mouseMoveEvent(QMouseEvent* event){
    // 鼠标移动
    if(!m_dragging) return;

    const QPointF mousePos = event->pos();
    const QPointF delta = m_lastMousePos - mousePos;    // 屏幕坐标差

    if(qAbs(delta.x()) >= 0.1 || qAbs(delta.y()) >= 0.1){
        m_moved = true;
    }
    m_lastMousePos = mousePos;

    // delta / scale = 世界坐标差
    m_centerX += delta.x() / m_scale;
    m_centerY -= delta.y() / m_scale;   // Y轴翻转
    update();

    event->accept();
}

void MapView::mouseReleaseEvent(QMouseEvent* event){
    // 鼠标释放
    if(event->button() != Qt::LeftButton) return;
    setCursor(QCursor(Qt::ArrowCursor));
    m_dragging = false;
    if(!m_moved){

    }

    event->accept();
}

void MapView::wheelEvent(QWheelEvent* event){
    // 滚轮缩放
    const double factor = event->angleDelta().y() > 0 ? 1.25 : 0.85;

    const double scale = m_scale * factor;
    const double fitscale = fitScale();

    m_scale = std::clamp(scale, fitscale, fitscale*100);

    update();
}