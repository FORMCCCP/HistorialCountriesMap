#pragma once
#include <functional>
#include <queue>
#include <mutex>

class MainThreadDispatcher
{
public:
    MainThreadDispatcher();

    void post(std::function<void()> task);  // 投入任务
    void processTasks();    // 处理任务

private:
    std::mutex m_mutex;
    std::queue<std::function<void()>> m_tasks;
};


