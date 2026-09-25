#include "mainthreaddispatcher.h"

MainThreadDispatcher::MainThreadDispatcher() {}


void MainThreadDispatcher::post(std::function<void()> task){
    std::lock_guard<std::mutex> lock(m_mutex);
    m_tasks.push(task);
}


void MainThreadDispatcher::processTasks(){
    std::queue<std::function<void()>> tasks;

    // 取任务
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        while(!m_tasks.empty()){
            tasks.push(m_tasks.front());
            m_tasks.pop();
        }
    }

    // 执行
    while(!tasks.empty()){
        std::function<void()> task = tasks.front();
        tasks.pop();

        task();
    }
}