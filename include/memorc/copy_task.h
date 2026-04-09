#pragma once
#include <cstddef>
#include <memory>
#include "i_task.h"

namespace memorc {
    class CopyTask : public ITask {
        
        public:
            CopyTask(void* dest, void* src, size_t size);
            Package create(void* dest , void* src , size_t size) override;
            void* getDest() const override {return dest_;}
            void* getSrc() const override {return src_;}
            size_t getSize() const override {return size_;}
            int getId() const override {return id_;}
            Package getPackage() const { return packagedTask_;}

        private:
            void* dest_;
            void* src_;
            size_t size_;
            int id_;
            static std::atomic<int> nextId_;
            Package packagedTask_;

    };
    
}