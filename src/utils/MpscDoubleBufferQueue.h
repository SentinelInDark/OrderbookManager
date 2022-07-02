//
// Created by Liam Zhang on 2022/7/2.
//

#ifndef ORDERBOOKMANAGER_MPSCDOUBLEBUFFERQUEUE_H
#define ORDERBOOKMANAGER_MPSCDOUBLEBUFFERQUEUE_H

#include <atomic>
#include <list>
#include <memory>
#include <mutex>

#include <spdlog/spdlog.h>

#include "MpscQueue.h"

namespace obm /*orderbookmanager*/{
    template<typename T>
    class MpscDoubleBufferQueue final : public MpscQueue<T> {
    public:
        MpscDoubleBufferQueue() {
            mProducerQueue = std::make_unique<Queue>();
            mConsumerQueue = std::make_unique<Queue>();
        }

        ~MpscDoubleBufferQueue() override = default;

        void enqueue(const T &) override;
        const T dequeue() override;

        [[nodiscard]] uint64_t size() const override { return mConsumerQueue->size(); }
        [[nodiscard]] uint64_t estimateTotalSize() const override {
            return mConsumerQueue->size() + mProducerQueue->size();
        }
        [[nodiscard]] bool empty() const override { return mQueueSize == 0; }

        void shutdown() override {
            mShouldExit = true;
            mCondVar.notify_one();
            SPDLOG_INFO("Stop queue");
        }

    private:
        using Queue = std::list<T>;

        /// queue that producer writes to
        std::unique_ptr<Queue> mProducerQueue;
        /// queue that consumer reads from
        std::unique_ptr<Queue> mConsumerQueue;

        /// Mutex for \p mProducerQueue
        std::mutex mMutex;
        /// CV for flipping \p mProducerQueue and \p mConsumerQueue.
        std::condition_variable mCondVar;

        /// Queue Size for both \p mProducerQueue and \p mConsumerQueue.
        std::atomic<uint64_t> mQueueSize = 0;

        /// true if the \p mProducerQueue should
        /// stop accepting new requests from producer.
        std::atomic<bool> mShouldExit = false;
    };
} ///end namespace obm

#endif //ORDERBOOKMANAGER_MPSCDOUBLEBUFFERQUEUE_H
