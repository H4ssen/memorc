#include <cstring>

#include "worker_pool.h"
#include "copy_task.h"
#include "i_task.h"
#include "task_queue.h"
#include <stdexcept>
#include <functional>
#include <future>
#include <iostream>

namespace memorc {
    WorkerPool::WorkerPool(size_t numberOfThreads, std::shared_ptr<TaskQueue> taskQueue) : 
        numOfWorkers_(numberOfThreads) ,
        tasks_(taskQueue)
        {
        if (numberOfThreads == 0) {
            throw std::invalid_argument("Number of threads must be greater than 0");
        }

        if (!taskQueue) {
            throw std::invalid_argument("TaskQueue cannot be null");
        }

        workers_.reserve(numOfWorkers_);
        for (size_t n = 0 ; n < numOfWorkers_ ; n++) {
            workers_.emplace_back(n,tasks_,queueMutex_,condVar_,activeTasks_,finishedCondition_);
        }
    }

    void
    WorkerPool::shutdown() {

        for (auto& worker : workers_) {
            worker.stop();
        }
    }

    void
    WorkerPool::notify_one() {
        condVar_.notify_one();
    }

    bool
    WorkerPool::isShutDown() const {
        return shutdown_;
    }

    void
    WorkerPool::wait_for_all_workers() {
        std::unique_lock<std::mutex> lock(queueMutex_);
        finishedCondition_.wait(lock, [this]{
            return tasks_->empty() && activeTasks_ == 0 ;
        });
    }
}