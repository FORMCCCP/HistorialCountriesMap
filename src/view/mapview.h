#pragma once

#include <QQmlEngine>
#include <QQuickItem>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPointF>

#include "totalcontroller.h"
#include "renderer.h"
#include "nodebuilder.h"

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
    QPointF screenToWorld(const QPoint& screen) const;
    QPointF worldToscreen(const QPoint& world) const;
    double fitScale() const;

    double m_centerX = 0.0; // 中心坐标
    double m_centerY = 0.0;
    double m_scale = 1.0;   // 缩放

    bool m_dragging = false;    // 是否拖拽
    bool m_moved = false;       // 是否移动
    QPointF m_lastMousePos;

    bool tilesDirty = true;     // 瓦片脏标记
    bool regionsDirty = true;   // 地区脏标记


    NodeBuilder* m_noderbuilder= new NodeBuilder();
    Renderer* m_renderer = new Renderer(m_noderbuilder);

    TotalController* m_controller = nullptr;
};

