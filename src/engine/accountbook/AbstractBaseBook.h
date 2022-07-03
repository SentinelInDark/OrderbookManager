//
// Created by Liam Zhang on 2022/7/3.
//

#ifndef ORDERBOOKMANAGER_ABSTRACTBASEBOOK_H
#define ORDERBOOKMANAGER_ABSTRACTBASEBOOK_H

#include "../../meta/Order.h"
#include <chrono>
#include <map>
#include <memory>

#include "BookKey.h"
#include "../../meta/Event.h"

namespace obm {
    class AbstractBaseBook {
    public:
        explicit AbstractBaseBook(std::unique_ptr<std::map<BookKey, std::shared_ptr<Order>, BookKeyComparator>>);

        virtual ~AbstractBaseBook() = default;

        void add(std::shared_ptr<Order>, std::vector<std::shared_ptr<Event>> *);
        void replace(const std::shared_ptr<Order>&);
        void cancel(std::shared_ptr<Order>);
        std::shared_ptr<Order> find(const std::shared_ptr<Order>&);
        static BookKey buildBookKeyFromOrder(const std::shared_ptr<Order>&);
        void remove(BookKey &);
        void remove(const std::shared_ptr<Order>&);
        [[nodiscard]] std::shared_ptr<Order> findCandidateOrder() const;

        void trade(const std::shared_ptr<Order>&, std::vector<std::shared_ptr<Event>> *);
        void cleanupOrder(const std::shared_ptr<Order> &orderPtr);
        static void printTradeDetail(const std::shared_ptr<Order>&, quantityType, priceType);

        virtual void print() const = 0;
    protected:
        virtual bool canTrade(const std::shared_ptr<Order>&, const std::shared_ptr<Order>&) const = 0;
    protected:
        std::unique_ptr<std::map<BookKey, std::shared_ptr<Order>, BookKeyComparator>> m_bookMapPtr;
    };

} // obm

#endif //ORDERBOOKMANAGER_ABSTRACTBASEBOOK_H
