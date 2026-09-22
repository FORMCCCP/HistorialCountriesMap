#include "mapview.h"
#include "geoprojection.h"

#include <QSGNode>
#include <QPointF>
#include <QTransform>
#include <QMatrix4x4>
#include <QSGFlatColorMaterial>
#include <QCursor>

#include <QtGlobal>



MapView::MapView(QQuickItem* parent)
    : QQuickItem(parent){
    setAcceptedMouseButtons(Qt::LeftButton);    // 设置只接受左键
    setFlag(ItemHasContents, true);
    // 打开 ItemHasContents ，让该Item可以调用updatePaintNode()去渲染


//========================================

    m_centerX = width()/2;
    m_centerY = height()/2;
    update();
}

void MapView::setController(TotalController* c){
    // 设置视图的控制器
    if(c == m_controller) return;

    m_controller = c;
    qDebug() << "Map视图以获取总控制器";
}





QSGNode* MapView::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*){
    oldNode = m_renderer->render(oldNode, tilesDirty, regionsDirty);    // 委托节点建立

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

QPointF MapView::screenToWorld(const QPoint& screen) const{
    // 屏幕坐标转换成世界坐标
    return QPointF(
        (screen.x() - width() / 2.0) / m_scale + m_centerX,
        m_centerY - (screen.y() - height() / 2.0) / m_scale);
}
QPointF MapView::worldToscreen(const QPoint& world) const{
    // 世界坐标转换成屏幕坐标
    return QPointF(
        (world.x() - m_centerX) * m_scale + width() / 2.0,
        (m_centerY - world.y()) * m_scale + height() / 2.0);
}

double MapView::fitScale()const{
    const double worldSize = 2.0 * GeoProjection::PI * GeoProjection::EARTH_RADIUS;
    return qMin(width(), height()) / worldSize;
}


void MapView::mousePressEvent(QMouseEvent* event){
    // 鼠标按压
    if(event->button() != Qt::LeftButton) return;   // 只接受左键

    qDebug()<<"点击左键";
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
    qDebug()<<delta;
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
        qDebug()<<"点击";
    }

    event->accept();
}

void MapView::wheelEvent(QWheelEvent* event){
    // 滚轮缩放
    const double factor = event->angleDelta().y() > 0 ? 1.25 : 0.85;

    const double scale = m_scale * factor;

    if(scale < 1){
        m_scale = 1;
    }else if(scale > 20){
        m_scale = 20;
    }else{
        m_scale = scale;
    }


    update();
}