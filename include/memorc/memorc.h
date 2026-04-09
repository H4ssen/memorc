#pragma once

#include "copy_task.h"
#include "task_queue.h"
#include "worker_pool.h"
#include <cstddef>
#include <future>

namespace memorc {

    class MemOrc {
        public:
            static MemOrc& getInstance() {
                static MemOrc instance;
                return instance;
            }

            //
            MemOrc(const MemOrc&) = delete;
            MemOrc& operator=(const MemOrc&) = delete;
            MemOrc(const MemOrc&&) = delete;
            MemOrc& operator =(const MemOrc&&) = delete;

            //******Utils ******/
            //submit an async copy task
            int submit_copy(void* dest, void* src , size_t size);
            //blocks until copy is done
            void wait_for_completion(std::future<void*> future);
            void wait_for_completion();
            //get queue size
            size_t get_queue_size() const;



        private:
            MemOrc();
            std::unique_ptr<WorkerPool> pool_;
            std::shared_ptr<TaskQueue> queue_;
    };
}