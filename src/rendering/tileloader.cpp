#include "tileloader.h"

TileLoader::TileLoader(MainThreadDispatcher& dipatcher)
    : m_dipatcher(dipatcher)
{
    m_pool = new ThreadPool();
}


void TileLoader::load(const std::uint16_t id, std::function<void(const std::uint16_t id, const QImage& img)> mainTask){
    // 投递线程池
    m_pool->enqueue([this, id, mainTask](){
        QImage img = m_store.loadTile(id);// 读取图片

        // 投递主线程调度器
        m_dipatcher.post([id, img, mainTask](){
            // 等待被主线程调用
            mainTask(id, img);
        });
    });
}
