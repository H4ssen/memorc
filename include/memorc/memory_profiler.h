#pragma once 
#include <cstddef>
#include <queue>
#include <future>

#include "copy_task.h"

namespace memorc {
    struct copyStats {
        size_t total_copied;
        size_t completed_tasks;
        double avg_latency_ms;
    };
    class TaskMonitor {
        public:
            using FTasks =  std::queue<std::future<void*>>;
            TaskMonitor();
            void* getResult(int id);
            void* getResults();
            void getStats() ;
            void setFutureTask(std::future<void*> fresult);
            FTasks& getFutureResults();
        private:
            FTasks taskResults_;


    };
}