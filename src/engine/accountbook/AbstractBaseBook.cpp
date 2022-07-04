/************************************************************************
Copyright @author.
Implementation of order book management
Order book explanation:
    https://www.investopedia.com/terms/o/orderbook.asp
**************************************************************************/

#include <iostream>
#include "AbstractBaseBook.h"
#include "../../logger/Logger.h"
#include "absl/strings/str_format.h"

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

    void AbstractBaseBook::printTradeDetail(const std::shared_ptr<Order>& orderPtr, quantityType qty, priceType price) {
        //Order ID=1 FullyFilled filledPrice=100 filledQty=2
        //Order ID=5 PartiallyFilled filledPrice=100 filledQty=2 leaves 1@99
        if (orderPtr->isFullyFilled()) {
            auto msg = absl::StrFormat("Order ID=%ld FullyFilled filledPrice==%ld filledQty=%ld",
                                       orderPtr->m_orderId, price, qty);
            std::cout<<msg<<std::endl;
        } else {
            auto msg = absl::StrFormat("Order ID=%ld PartiallyFilled filledPrice==%ld filledQty=%ld leaves %ld@%ld",
                                       orderPtr->m_orderId, price, qty, orderPtr->m_quantity, orderPtr->m_price);
            std::cout<<msg<<std::endl;
        }
    }

    void AbstractBaseBook::trade(const std::shared_ptr<Order>& newOrderPtr, std::vector<std::shared_ptr<Event>> *pEvents) {
        while (true) {
            auto candidateOrder = this->findCandidateOrder();
            if (!canTrade(candidateOrder, newOrderPtr)) {
                break;
            }
            auto qty = std::min(candidateOrder->m_quantity, newOrderPtr->m_quantity);
            auto price = std::max(candidateOrder->m_price, newOrderPtr->m_price);

            candidateOrder->decreaseQuantity(qty);
            pEvents->push_back(std::make_shared<Event>(candidateOrder->m_orderId, candidateOrder->m_status, candidateOrder->m_side, candidateOrder->m_price, candidateOrder->m_quantity, qty));
            printTradeDetail(candidateOrder, qty, price);

            newOrderPtr->decreaseQuantity(qty);
            pEvents->push_back(std::make_shared<Event>(newOrderPtr->m_orderId, newOrderPtr->m_status, newOrderPtr->m_side, newOrderPtr->m_price, newOrderPtr->m_quantity, qty));
            printTradeDetail(newOrderPtr, qty, price);

            this->cleanupOrder(candidateOrder);
            if (newOrderPtr->isFullyFilled()) {
                break;
            }
        }
    }

    void AbstractBaseBook::cleanupOrder(const std::shared_ptr<Order>& orderPtr) {
        if (orderPtr && orderPtr->isFullyFilled()) {
            this->remove(orderPtr);
        }
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

    void AbstractBaseBook::restore(const std::shared_ptr<Event> &eventPtr) {
        auto orderPtr = std::make_shared<Order>(eventPtr->m_orderId, eventPtr->m_side, eventPtr->m_price, eventPtr->m_quantity);
        switch (eventPtr->m_status) {
            case OrderStatus::NEW: {
                SPDLOG_INFO("restore - new, order id {}, side {}, price {}, qty {}", orderPtr->m_orderId, orderPtr->m_side, orderPtr->m_price, orderPtr->m_quantity);
                this->add(orderPtr, nullptr);
                break;
            }
            case OrderStatus::FULLY_FILLED:
            case OrderStatus::CANCELED: {
                this->remove(orderPtr);
                break;
            }
            case OrderStatus::PARTIALLY_FILLED: {
                auto curOrderPtr = this->find(orderPtr);
                if (curOrderPtr) {
                    SPDLOG_INFO("update - order id {}, side {}, price {}, qty {}", orderPtr->m_orderId, orderPtr->m_side, orderPtr->m_price, orderPtr->m_quantity);
                    curOrderPtr->m_quantity = orderPtr->m_quantity;
                }
                break;
            }
            case OrderStatus::REJECTED: {
                break;
            }
        }
    }

    void AbstractBaseBook::add(std::shared_ptr<Order> orderPtr, std::vector<std::shared_ptr<Event>> *pEvents) {
        BookKey key = obm::AbstractBaseBook::buildBookKeyFromOrder(orderPtr);
        m_bookMapPtr->try_emplace(key, orderPtr);
        if (pEvents) {
            pEvents->push_back(std::make_shared<Event>(orderPtr->m_orderId, orderPtr->m_status, orderPtr->m_side, orderPtr->m_price, orderPtr->m_quantity, 0));
            std::cout<<"New Order submitted, ID = "<<orderPtr->m_orderId<<std::endl;
        }
    }

    void AbstractBaseBook::replace(const std::shared_ptr<Order>& orderPtr, std::vector<std::shared_ptr<Event>> *pEvents) {
        std::shared_ptr<Order> originalOrderPtr = this->find(orderPtr->m_orderId);
        if (!originalOrderPtr) {
            std::cout<<"Order ID = "<<orderPtr->m_orderId<<" does not exist."<<std::endl;
            return;
        }
        if (originalOrderPtr->isNew()) {
            this->remove(originalOrderPtr);
            BookKey key = AbstractBaseBook::buildBookKeyFromOrder(orderPtr);
            m_bookMapPtr->try_emplace(key, orderPtr);
            if (pEvents) {
                originalOrderPtr->m_status = OrderStatus::CANCELED;
                pEvents->push_back(std::make_shared<Event>(originalOrderPtr->m_orderId, originalOrderPtr->m_status, originalOrderPtr->m_side, originalOrderPtr->m_price, originalOrderPtr->m_quantity, 0));
                pEvents->push_back(std::make_shared<Event>(orderPtr->m_orderId, orderPtr->m_status, orderPtr->m_side, orderPtr->m_price, orderPtr->m_quantity, 0));
                std::cout<<"Order replaced, ID = "<<orderPtr->m_orderId<<std::endl;
            }
        } else {
            std::cout<<"Order can not be replaced, ID = "<<orderPtr->m_orderId<<std::endl;
        }
    }

    ///TODO clarify cancellation logic
    void AbstractBaseBook::cancel(const std::shared_ptr<Order>& orderPtr, std::vector<std::shared_ptr<Event>> *pEvents) {
        BookKey key = obm::AbstractBaseBook::buildBookKeyFromOrder(orderPtr);
        std::shared_ptr<Order> originalOrderPtr = this->find(orderPtr);
        if (!originalOrderPtr) {
            std::cout<<"Order ID = "<<orderPtr->m_orderId<<" does not exist."<<std::endl;
            return;
        }
        if (originalOrderPtr->isNew()) {
            this->remove(key);
            if (pEvents) {
                originalOrderPtr->m_status = OrderStatus::CANCELED;
                pEvents->push_back(std::make_shared<Event>(originalOrderPtr->m_orderId, originalOrderPtr->m_status, originalOrderPtr->m_side, originalOrderPtr->m_price, originalOrderPtr->m_quantity, 0));
                std::cout<<"Order canceled, ID = "<<orderPtr->m_orderId<<std::endl;
            }
        } else {
            std::cout<<"Order can not be canceled, ID = "<<orderPtr->m_orderId<<std::endl;
        }
    }

    std::shared_ptr<Order> AbstractBaseBook::find(const std::shared_ptr<Order>& orderPtr) {
        BookKey key = obm::AbstractBaseBook::buildBookKeyFromOrder(orderPtr);
        auto iter = m_bookMapPtr->find(key);
        if (iter != m_bookMapPtr->end()) {
            return iter->second;
        }
        return nullptr;
    }

    /// TODO : performance optimization
    std::shared_ptr<Order> AbstractBaseBook::find(orderIdType orderId) const {
        for (auto &iter : *m_bookMapPtr) {
            if (iter.first.orderId == orderId) {
                return iter.second;
            }
        }
        return nullptr;
    }

    BookKey AbstractBaseBook::buildBookKeyFromOrder(const std::shared_ptr<Order>& orderPtr) {
        return {orderPtr->m_orderId, orderPtr->m_price, orderPtr->m_createTime};
    }
} /// end namespace obm
