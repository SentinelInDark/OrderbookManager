/************************************************************************
Copyright @author.
Implementation of order book management
Order book explanation:
    https://www.investopedia.com/terms/o/orderbook.asp
**************************************************************************/

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

    template<typename T>
    void MpscDoubleBufferQueue<T>::enqueue(const T &t) {
        if (mShouldExit) {
            throw QueueStoppedException();
        }

        std::unique_lock lock(mMutex);

        bool isEmpty = mProducerQueue->empty();
        mProducerQueue->push_back(t);

        ++mQueueSize;

        if (isEmpty) {
            mCondVar.notify_one();
        }
    }

    template<typename T>
    const T MpscDoubleBufferQueue<T>::dequeue() {
        if (mConsumerQueue->empty()) {
            std::unique_lock lock(mMutex);

            if (mProducerQueue->empty()) {
                /// this call should not block
                /// if it's empty and the shouldExit signal is fired before
                if (mShouldExit) {
                    throw QueueStoppedException();
                }

                mCondVar.wait(lock, [this] { return !mProducerQueue->empty() || mShouldExit; });

                /**
                 * after wakeup, it will be following three case:
                 *   1) mProducerQueue not empty, mShouldExit is true
                 *   2) mProducerQueue not empty, mShouldExit is false
                 *   3) mProducerQueue empty, mShouldExit is true
                 *
                 * attention that, for case 1, we need flip mProducerQueue and mConsumerQueue
                 * to handle the last left requests in producer queue.
                 */
                if (mProducerQueue->empty()) {
                    throw QueueStoppedException();
                }
            }

            std::swap(mProducerQueue, mConsumerQueue);
        }

        --mQueueSize;

        const T t = mConsumerQueue->front();
        mConsumerQueue->pop_front();

        return t;
    }
} ///end namespace obm

#endif //ORDERBOOKMANAGER_MPSCDOUBLEBUFFERQUEUE_H
