#include <gtest/gtest.h>
#include "circuits.hpp"

using namespace Circuits;

TEST(LogicGate, CalculateOutput)
{
    Pos<int> pos { 0, 0 };
    Frame<int> frame { 30, 30 };

    Pin<int> in1 { { 1, 1 }, Pin<int>::State::LogicZero };
    Pin<int> in2 { { 2, 2 }, Pin<int>::State::LogicZero };
    Pin<int> in3 { { 3, 3 }, Pin<int>::State::LogicOne };

    Pin<int> out { { 4, 4 }, Pin<int>::State::LogicOne };
    
    std::shared_ptr<Pin<int>> output = std::make_shared<Pin<int>>(out);
    
    std::vector<std::shared_ptr<Pin<int>>> inputs = 
        { 
            std::make_shared<Pin<int>>(in1),
            std::make_shared<Pin<int>>(in2),
            std::make_shared<Pin<int>>(in3)
        };

    // out = in1 & in2 & in3 (0 | 0 | 1 = 1)
    std::unique_ptr<LogicGate<int>> gate = std::make_unique<Or<int>>(pos, frame, output, inputs);

    // sub-test #1
    EXPECT_EQ(gate->getOutput()->state, Pin<int>::State::LogicOne);

    // Decorate (got NOR)
    // out = !out (!1 = 0)
    gate = std::make_unique<Not<int>>(std::move(gate));

    // (!0 = 1)
    gate = std::make_unique<Not<int>>(std::move(gate));
    // (!1 = 0)
    gate = std::make_unique<Not<int>>(std::move(gate));

    // sub-test #2
    EXPECT_EQ(gate->getOutput()->state, Pin<int>::State::LogicZero);  
    
    ///////////////////////////////////////
    // sub-test #3
    Pin<int> inHighImp { { 3, 3 }, Pin<int>::State::HighImpedance };

    std::vector<std::shared_ptr<Pin<int>>> inputs2 = 
    { 
        std::make_shared<Pin<int>>(in1),
        std::make_shared<Pin<int>>(in2),
        std::make_shared<Pin<int>>(inHighImp)
    };

    std::unique_ptr<LogicGate<int>> gateHighImp = std::make_unique<Or<int>>(pos, frame, output, inputs2);
    gateHighImp = std::make_unique<Not<int>>(std::move(gateHighImp));

    EXPECT_EQ(gateHighImp->getOutput()->state, Pin<int>::State::HighImpedance);  

    ///////////////////////////////////////
    // sub-test #4

    Pin<int> inUndef { { 3, 3 }, Pin<int>::State::Undefined };

    std::vector<std::shared_ptr<Pin<int>>> inputs3 = 
    { 
        std::make_shared<Pin<int>>(in1),
        std::make_shared<Pin<int>>(inHighImp), // take a look (Undefined has higher priority than HighImpedance)
        std::make_shared<Pin<int>>(inUndef)
    };

    std::unique_ptr<LogicGate<int>> gateUndef = std::make_unique<Or<int>>(pos, frame, output, inputs3);
    gateUndef = std::make_unique<Not<int>>(std::move(gateUndef));

    EXPECT_EQ(gate->getOutput()->state, Pin<int>::State::Undefined);      
}