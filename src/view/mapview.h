#pragma once

#include <QQmlEngine>
#include <QQuickItem>
#include <QMouseEvent>
#include <QWheelEvent>

#include "totalcontroller.h"

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
    QMatrix4x4 viewMatrix();

    double m_centerX = 0.0; // 中心坐标
    double m_centerY = 0.0;
    double m_scale = 1.0;   // 缩放

    bool m_dragging = false;    // 是否拖拽
    bool m_moved = false;       // 是否移动
    QPointF m_lastMousePos;


    QSGNode* m_regionNode = nullptr;        // 地区渲染节点
    QSGTransformNode* m_viewNode = nullptr; // 视图节点

    TotalController* m_controller = nullptr;
};

