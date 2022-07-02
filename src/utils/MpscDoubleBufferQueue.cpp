//
// Created by Liam Zhang on 2022/7/2.
//

#include "MpscDoubleBufferQueue.h"

namespace obm /*orderbookmanager*/{

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
} /// end namespace obm
