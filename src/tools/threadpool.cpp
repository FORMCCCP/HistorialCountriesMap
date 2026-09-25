#include "threadpool.h"

ThreadPool::ThreadPool() {
    // 建立线程：4个
    for(int i =0; i<4; ++i){
        std::thread t([this](){
            work();
        });

        m_threads.push_back(t);
    }
}


void ThreadPool::enqueue(std::function<void()> task){
    // 投入任务
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_tasks.push(task);
    }

    m_condition.notify_one();   // 唤醒一个线程
}

void ThreadPool::work(){
    while(true){
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(m_mutex); // 加锁
            // 让线程被阻塞，直到被唤醒后，存在任务
            m_condition.wait(lock, [this](){
                return !m_tasks.empty();
            });

            // 取任务
            task = m_tasks.front();
            m_tasks.pop();
        }

        task(); // 执行任务，在锁外进行(因为在任务中没有线程需要争抢的资源)
    }
}