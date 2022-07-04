/************************************************************************
Copyright @author.
Implementation of order book management
Order book explanation:
    https://www.investopedia.com/terms/o/orderbook.asp
**************************************************************************/

#include <gtest/gtest-spi.h>
#include <memory>

#include "spdlog/spdlog.h"
#include "../../src/engine/ProcessEngine.h"
#include "../../src/storage/EventWarehouse.h"

namespace obm {

    class ProcessEngineTest : public ::testing::Test {
    protected:
        ProcessEngineTest() = default;

        ~ProcessEngineTest() override = default;

        void SetUp() override {
            m_commandQueue = std::make_shared<MpscDoubleBufferQueue<std::shared_ptr<Command>>>();
            m_eventQueue = std::make_shared<MpscDoubleBufferQueue<std::shared_ptr<Event>>>();

            m_dataStorePath = "./test";
            m_processEngine = std::make_unique<ProcessEngine>(m_dataStorePath, m_commandQueue, m_eventQueue);
            m_eventWarehouse = std::make_unique<EventWarehouse>(m_dataStorePath, m_eventQueue);
            m_isRunning = true;

            m_commands.emplace_back(std::make_shared<Command>(Command::CommandType::NEW,
                                                              std::make_shared<Order>(1, OrderSide::BUY, 100, 1)));
            m_commands.emplace_back(std::make_shared<Command>(Command::CommandType::NEW,
                                                              std::make_shared<Order>(2, OrderSide::BUY, 99, 2)));
            m_commands.emplace_back(std::make_shared<Command>(Command::CommandType::NEW,
                                                              std::make_shared<Order>(3, OrderSide::BUY, 100, 3)));
            m_commands.emplace_back(std::make_shared<Command>(Command::CommandType::NEW,
                                                              std::make_shared<Order>(4, OrderSide::SELL, 100, 2)));
        }

        void TearDown() override {
            m_commandQueue->shutdown();
            m_eventQueue->shutdown();
            m_processEngine->shutdown();
            m_eventWarehouse->shutdown();
            m_isRunning = false;
        }

        std::thread m_commandProcessThread;
        std::thread m_eventWarehouseThread;

        std::unique_ptr<ProcessEngine> m_processEngine;
        std::unique_ptr<EventWarehouse> m_eventWarehouse;

        std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Command>>> m_commandQueue;
        std::shared_ptr<MpscDoubleBufferQueue<std::shared_ptr<Event>>> m_eventQueue;

        bool m_isRunning;
        std::string m_dataStorePath;

        std::vector<std::shared_ptr<Command>> m_commands;
    };

    TEST_F(ProcessEngineTest, DoExecuteRequired) {
        for (const auto &command: m_commands) {
            m_commandQueue->enqueue(command);
        }

        while (!m_commandQueue->empty()) {
            auto cmdPtr = m_commandQueue->dequeue();
            m_processEngine->processCommand(cmdPtr);
        }

        while (!m_eventQueue->empty()) {
            m_eventWarehouse->persist(m_eventQueue->dequeue());
        }

        auto orderPtr = m_processEngine->getOrder(2);
        EXPECT_EQ(orderPtr->m_orderId, uint64_t(2));
        EXPECT_EQ(orderPtr->m_status, OrderStatus::NEW);
        EXPECT_EQ(orderPtr->m_price, uint64_t(99));

        orderPtr = m_processEngine->getOrder(4);
        EXPECT_EQ(orderPtr, nullptr);
    }

}  /// end namespace obm
