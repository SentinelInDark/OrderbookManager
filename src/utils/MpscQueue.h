/************************************************************************
Copyright @author.
Implementation of order book management
Order book explanation:
    https://www.investopedia.com/terms/o/orderbook.asp
**************************************************************************/

#ifndef ORDERBOOKMANAGER_MPSCQUEUE_H
#define ORDERBOOKMANAGER_MPSCQUEUE_H

#include <cstdint>
#include <exception>

namespace obm /*orderbookmanager*/{

    class QueueStoppedException : public ::std::exception {
    public:
        [[nodiscard]] const char *what() const noexcept override {
            return "Queue has stopped.";
        }
    };

    template<typename T>
    class MpscQueue {
    public:
        virtual ~MpscQueue() = default;

        /**
         * Append an item to the tail of the queue.
         * This method will block until the item has been successfully inserted.
         * @throw ::obm::QueueStoppedException if queue has been shut down
         */
        virtual void enqueue(const T &) = 0;

        /**
         * Remove and return the item at the head of the queue.
         * This method will block until the queue is not empty.
         * @return the item at the head of the queue
         * @throw ::obm::QueueStoppedException if queue has been shut down
         */
        virtual const T dequeue() = 0;

        /**
         * Return the total count of items in the consumer queue.
         * @return the total count of items in the consumer queue
         */
        [[nodiscard]] virtual uint64_t size() const = 0;

        /**
         * Return the total count of items in both consumer and producer queue.
         * It might not be correct due to concurrent access
         * @return the total count of items in both consumer and producer queue
         */
        [[nodiscard]] virtual uint64_t estimateTotalSize() const = 0;

        /**
         * Return true, if queue is empty.
         */
        [[nodiscard]] virtual bool empty() const = 0;

        /**
         * Stop accepting new requests from producer.
         * Entries already in the queue will be all consumed.
         */
        virtual void shutdown() = 0;
    };

}  /// namespace obm

#endif //ORDERBOOKMANAGER_MPSCQUEUE_H
