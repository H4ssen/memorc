#pragma once 
#include <cstddef>
#include <atomic>
#include <memory>
#include <future>


namespace memorc {
    class ITask {
        public:
            using Package = std::shared_ptr<std::packaged_task<void*()>>;
            virtual ~ITask() = default;
            virtual Package create(void* dest , void* src , size_t size) = 0;
            virtual int getId() const =0;
            virtual void* getDest() const = 0;
            virtual void* getSrc() const = 0;
            virtual size_t getSize() const =0;
            virtual Package getPackage() const =0;
    };
}


