#pragma once


#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>

#include "task_queue.h"
#include "i_task.h"

namespace memorc {
    class Worker {

        public:
            Worker(size_t id,std::shared_ptr<TaskQueue> tasks,std::mutex& mtx,
                    std::condition_variable& cv, std::atomic<size_t>& activeTasks,std::condition_variable& finished);
            ~Worker();
            
            Worker(Worker&& other) noexcept;
            void run();
            void stop();
            bool isRunning() const;
            void executeTask(std::shared_ptr<ITask> task);
            std::future<void*> getFuture() const;
            size_t getWorkerId() const {return workerId_;}

        private:
            std::atomic<bool> running_;
            size_t workerId_;
            std::shared_ptr<TaskQueue> tasks_;
            std::mutex& queueMutex_;
            std::condition_variable& condVar_;
            std::atomic<size_t>& activeTasks_;
            std::condition_variable& finishedCondition_;
            std::thread worker_;
            bool shutdown_{false};
            std::future<void*> future_;
            std::shared_ptr<ITask> currentTask_;
    };
}