#include "memorc.h"
#include "copy_task.h"
#include "task_queue.h"
#include <memory>
#include <iostream>

namespace memorc {

    MemOrc::MemOrc() {
        queue_ = std::make_shared<TaskQueue>();
        pool_ = std::make_unique<WorkerPool>(4,queue_);
    }

    int
    MemOrc::submit_copy(void* dest, void* src, size_t size) {
        if (!dest || !src || ! size) {
            return -1;
        }
        std::shared_ptr<CopyTask> ctask = std::make_shared<CopyTask>(dest,src,size);
        queue_->push(ctask);
        pool_->notify_one();

        return ctask->getId();
    }   

    size_t
    MemOrc::get_queue_size() const {
        if (queue_) {
            return queue_->size();
        }
            return 0;
    }

    void MemOrc::wait_for_completion(std::future<void*> future) {
        if (future.valid()) {
            future.wait();
        }
    }

    void MemOrc::wait_for_completion() {
        
        if (pool_) {
            pool_->wait_for_all_workers();
        }
    }

}