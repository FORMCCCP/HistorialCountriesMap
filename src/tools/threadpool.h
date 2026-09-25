#pragma once

#include <thread>
#include <functional>
#include <queue>
#include <mutex>

// 线程池
class ThreadPool
{
public:
    ThreadPool();
    ~ThreadPool();

    void enqueue(std::function<void()> task);
private:
    void work();

    std::vector<std::thread> m_threads; // 线程
    std::queue<std::function<void()>>   m_tasks; // 任务队列

    std::mutex m_mutex; // 互斥锁
    std::condition_variable m_condition; // 条件变量
};

