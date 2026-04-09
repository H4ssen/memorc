#include "worker.h"
#include "i_task.h"
#include "task_queue.h"
#include <atomic>
#include <iostream>

namespace memorc {
    Worker::Worker(size_t id,
                    std::shared_ptr<TaskQueue> tasks,
                    std::mutex& mtx,
                    std::condition_variable& cv,
                    std::atomic<size_t>& activeTasks,
                    std::condition_variable& finished
                    ) : 
                    running_(true),
                    workerId_(id), tasks_(tasks),
                    queueMutex_(mtx),condVar_(cv),
                    activeTasks_(activeTasks),
                    finishedCondition_(finished)

        {
            
            worker_ = std::thread(&Worker::run, this);
    }

        Worker::Worker(Worker&& other) noexcept
                :running_(other.running_.load()),
                 workerId_(other.workerId_),
                tasks_(other.tasks_),
                queueMutex_(other.queueMutex_),
                condVar_(other.condVar_),
                activeTasks_(other.activeTasks_),
                finishedCondition_(other.finishedCondition_),
                worker_(std::move(other.worker_)),
                shutdown_(other.shutdown_),
                future_(std::move(other.future_)),
                currentTask_(std::move(other.currentTask_))
          {}

    Worker::~Worker() {
        if (running_) {
            stop();
        }
    }

    void
    Worker::run() {
        while (!shutdown_) {
            std::shared_ptr<ITask> task;
            {   
                std::unique_lock<std::mutex> lock(queueMutex_);
                condVar_.wait(lock,[&]{
                    return shutdown_ || !tasks_->empty();
                });
                if (shutdown_ && tasks_->empty()) {
                    return;
                }
                if (!tasks_->empty()) {
                    task = std::move(tasks_->pull());
                    activeTasks_++;
                }
                
            }
            if (task) {
                executeTask(task);
            }
            activeTasks_--;
            finishedCondition_.notify_all();         
        }
    }

    void 
    Worker::stop() {
        running_ = false;
        shutdown_ = true;
        condVar_.notify_all();
        if (worker_.joinable()) {
            worker_.join();
        }
        
    }

    bool Worker::isRunning() const {
        return running_;
    }

    void Worker::executeTask(std::shared_ptr<ITask> task) {
        if (task) {
            auto package = task->getPackage();
            if (package) {
                future_ = package->get_future();
                (*package)();
            }

        }
    }
}