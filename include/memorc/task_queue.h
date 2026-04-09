#pragma once

#include <queue>
#include "i_task.h"

namespace memorc {
    class TaskQueue {
        public:
            TaskQueue() = default;
            void push(std::shared_ptr<ITask> task);
            std::shared_ptr<ITask> pull();
            bool empty() const;
            size_t size() const;

        private:
            mutable std::mutex queueMutex_;
            std::queue<std::shared_ptr<ITask>> tasks_;

    };
}