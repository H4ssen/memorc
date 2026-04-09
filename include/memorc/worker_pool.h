#pragma once

#include <vector>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "worker.h"
#include "task_queue.h"
#include "i_task.h"

namespace memorc {
    class WorkerPool {
        public:
            WorkerPool(size_t numberOfThreads, std::shared_ptr<TaskQueue> queue);
            
            ~WorkerPool() {
                shutdown();
            }

            void notify_one();
            bool isShutDown() const;
            void wait_for_all_workers();

        private:
            void shutdown();
            size_t numOfWorkers_;
            std::shared_ptr<TaskQueue> tasks_;
            std::vector<Worker> workers_;
            std::mutex queueMutex_;
            std::condition_variable condVar_;
            std::condition_variable finishedCondition_;
            std::atomic<bool> shutdown_;
            std::atomic<size_t> activeWorkers_;
            std::atomic<size_t> activeTasks_{0};

    };
}