#pragma once

#include "threadpool.h"
#include "mainthreaddispatcher.h"
#include "tilestore.h"

#include <functional>

class TileLoader
{
public:
    TileLoader(MainThreadDispatcher& dipatcher);

    void load(const std::uint16_t id, std::function<void(const std::uint16_t id, const QImage& img)> mainTask);


private:
    ThreadPool* m_pool;
    MainThreadDispatcher& m_dipatcher;
    TileStore m_store;
};


