#include "task_queue.h"
#include <future>
#include <mutex>


namespace memorc {
    void
    TaskQueue::push(std::shared_ptr<ITask> task) {
        std::lock_guard<std::mutex> glock(queueMutex_);
        tasks_.push(task);
    }   

    std::shared_ptr<ITask>
    TaskQueue::pull() {
        std::lock_guard<std::mutex> glock(queueMutex_);
        if (tasks_.empty()) {
            return nullptr;
        }
        auto task = tasks_.front();
        tasks_.pop();
        return task;
    }

    bool
    TaskQueue::empty() const {
        std::lock_guard<std::mutex> glock(queueMutex_);
        return tasks_.empty();
    }

    size_t
    TaskQueue::size() const {
        std::lock_guard<std::mutex> glock(queueMutex_);
        return tasks_.size();
    }
}