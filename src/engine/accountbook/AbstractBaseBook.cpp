//
// Created by Liam Zhang on 2022/7/3.
//

#include "AbstractBaseBook.h"
#include <iostream>

namespace obm {
    AbstractBaseBook::AbstractBaseBook(std::unique_ptr<std::map<BookKey, std::shared_ptr<Order>, BookKeyComparator>> ptr): m_bookMapPtr(std::move(ptr)) {

    }

    void AbstractBaseBook::remove(BookKey &key) {
        auto iter = m_bookMapPtr->find(key);
        if (iter != m_bookMapPtr->end()) {
            m_bookMapPtr->erase(iter);
        }
    }

    void AbstractBaseBook::remove(const std::shared_ptr<Order>& orderPtr) {
        BookKey key = obm::AbstractBaseBook::buildBookKeyFromOrder(orderPtr);
        this->remove(key);
    }

    std::shared_ptr<Order> AbstractBaseBook::findCandidateOrder() const {
        if (m_bookMapPtr->empty()) {
            return nullptr;
        }
        return m_bookMapPtr->begin()->second;
    }

    void AbstractBaseBook::print() const {
        priceType pre = 0;
        for (auto& p : *m_bookMapPtr) {
            if (p.second->m_price != pre) {
                std::cout<<std::endl<<"\t";
                pre = p.second->m_price;
            } else {
                std::cout<<"\t";
            }
            p.second->printSummary();
        }
        std::cout<<std::endl;
    }

    void AbstractBaseBook::add(std::shared_ptr<Order> orderPtr) {
        BookKey key = obm::AbstractBaseBook::buildBookKeyFromOrder(orderPtr);
        m_bookMapPtr->try_emplace(key, orderPtr);
        std::cout<<"New Order submitted, ID = "<<orderPtr->m_orderId<<std::endl<<"> ";
        std::cout.flush();
    }

    void AbstractBaseBook::replace(const std::shared_ptr<Order>& orderPtr) {
        BookKey key = obm::AbstractBaseBook::buildBookKeyFromOrder(orderPtr);
        auto iter = m_bookMapPtr->find(key);
        if (iter != m_bookMapPtr->end()) {

        }
    }

    void AbstractBaseBook::cancel(std::shared_ptr<Order> orderPtr) {
        BookKey key = obm::AbstractBaseBook::buildBookKeyFromOrder(orderPtr);
        //m_bookMapPtr->try_emplace(key, orderPtr);
    }

    std::shared_ptr<Order> AbstractBaseBook::find(const std::shared_ptr<Order>& orderPtr) {
        BookKey key = obm::AbstractBaseBook::buildBookKeyFromOrder(orderPtr);
        auto iter = m_bookMapPtr->find(key);
        if (iter != m_bookMapPtr->end()) {
            return iter->second;
        }
        return nullptr;
    }

    BookKey AbstractBaseBook::buildBookKeyFromOrder(const std::shared_ptr<Order>& orderPtr) {
        return {orderPtr->m_orderId, orderPtr->m_price, orderPtr->m_createTime};
    }
} /// end namespace obm