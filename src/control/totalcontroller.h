#pragma once

#include <QObject>
#include "region.h"
#include "sqlrepository.h"


class TotalController : public QObject
{
    Q_OBJECT
public:
    explicit TotalController(QObject *parent = nullptr);


    // 获取当前可见区域
    const std::vector<Region>& visibleRegions() const{return m_visibleRegions;}

    // 加载区域数据
    Q_INVOKABLE QString DasePath();
    Q_INVOKABLE bool loadRegions(const QString& path);

signals:



private:
    SQLRepository m_repo;   // 区域数据层

    std::vector<Region> m_visibleRegions;   // 当前可见的区域
};


