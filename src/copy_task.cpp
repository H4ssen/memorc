#include <cstring>
#include "copy_task.h"
#include <memory>
#include <functional>
#include <future>

namespace memorc {
    std::atomic<int> CopyTask::nextId_{0};

    CopyTask::CopyTask(void* dest, void* src, size_t size) :
            dest_(dest),
            src_(src),
            size_(size),
            id_(nextId_++)
    {
        packagedTask_ = create(dest,src,size);
    }

    ITask::Package
    CopyTask::create(void* dest, void* src, size_t size) {
        auto packaged = std::make_shared<std::packaged_task<void*()>>(
            [dest, src, size]() -> void* {
                std::memcpy(dest,src,size);
                return dest;
            }
        );
        return packaged;
    }
}