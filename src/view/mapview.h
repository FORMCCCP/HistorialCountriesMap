#pragma once

#include <QQmlEngine>
#include <QQuickItem>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPointF>
#include <QTimer>

#include "totalcontroller.h"
#include "renderer.h"
#include "Tile.h"
#include "tileloader.h"

class MapView : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(TotalController* controller READ controller WRITE setController NOTIFY controllerChanged )
public:
    MapView(QQuickItem* parent = nullptr);

    TotalController* controller()const{return m_controller;}
    void setController(TotalController* c);

    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) override;

signals:
    void controllerChanged();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    QMatrix4x4 viewMatrix() const;    // 视图矩阵
    QPointF screenToWorld(const QPointF& screen) const;
    QPointF worldToscreen(const QPointF& world) const;
    double fitScale() const;

    int currentLevel() const;   // 当前层级
    QVector<std::uint16_t> visibleTiles(int level) const;   // 当前可获取瓦片计算
    void loadVisibleTiles(const QVector<std::uint16_t>& tiles); // 加载瓦片

    double m_centerX = 0.0; // 中心坐标
    double m_centerY = 0.0;
    double m_scale = 1.0;   // 缩放

    bool m_dragging = false;    // 是否拖拽
    bool m_moved = false;       // 是否移动
    QPointF m_lastMousePos;

    bool tilesDirty = true;     // 瓦片脏标记
    bool regionsDirty = true;   // 地区脏标记


    Renderer* m_renderer = new Renderer();  // 渲染器
    TotalController* m_controller = nullptr;    // 控制器
    TileLoader* m_tileLoader = new TileLoader(m_dispatcher);    // 瓦片加载器
    MainThreadDispatcher m_dispatcher;  // 主线程调度器
    QTimer* m_timer = nullptr;


    QVector<std::uint16_t> m_lastTilesID;   // 上一次加载的瓦片
    QSet<std::uint16_t> m_loadingTiles;     // 正在加载的瓦片
};

