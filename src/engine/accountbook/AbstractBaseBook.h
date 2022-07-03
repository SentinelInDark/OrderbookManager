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

namespace obm {
    class AbstractBaseBook {
    public:
        explicit AbstractBaseBook(std::unique_ptr<std::map<BookKey, std::shared_ptr<Order>, BookKeyComparator>>);

        virtual ~AbstractBaseBook() = default;

        void add(std::shared_ptr<Order>);
        void replace(std::shared_ptr<Order>);
        void cancel(std::shared_ptr<Order>);
        std::shared_ptr<Order> find(const std::shared_ptr<Order>&);
        static BookKey buildBookKeyFromOrder(const std::shared_ptr<Order>&);
        void remove(BookKey &);
        void remove(const std::shared_ptr<Order>&);
        [[nodiscard]] std::shared_ptr<Order> findSuitableOrder() const;

        virtual void print() const = 0;
    protected:
        std::unique_ptr<std::map<BookKey, std::shared_ptr<Order>, BookKeyComparator>> m_bookMapPtr;
    };

} // obm

#endif //ORDERBOOKMANAGER_ABSTRACTBASEBOOK_H
