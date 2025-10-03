#ifndef CIRCUITS_HPP
#define CIRCUITS_HPP

#include <iostream>
#include <type_traits>
#include <vector>
#include <memory>
#include <utility>
#include <concepts>

namespace Circuits 
{
    template<typename T>
    concept Number = std::integral<T> || std::floating_point<T>;

    template<Number N>
    struct Pos 
    {
        N x, y;
    };

    template<Number N>
    struct Frame 
    {
        N width, length;
    };

    template<Number N>
    struct Pin
    {
        enum class State 
        {
            LogicZero,
            LogicOne,
            HighImpedance,
            Undefined
        };

        // For debugging
        friend std::ostream& operator<<(std::ostream& stream, State state)
        { 
            switch(state)
            {
                case State::LogicOne: return stream << "Logic One";
                case State::LogicZero: return stream << "Logic Zero";
                case State::HighImpedance: return stream << "High Impedance";
                case State::Undefined: return stream << "Undefined";
                default: return stream << "Unknown";
            }
        }

        Pos<N> pos;
        State state { State::HighImpedance };
    };

    // Abstract class (Interface)
    template <
        Number N,
        template<typename, typename ...> 
            class InputsType = std::vector
    >
    class LogicGate 
    {
    public:
        LogicGate() = default;

        LogicGate(const Pos<N>& pos,
                  const Frame<N>& frame,
                  std::shared_ptr<Pin<N>>& output, 
                  InputsType<std::shared_ptr<Pin<N>>>& inputs) :
            m_pos(pos), m_frame(frame), m_output(output), m_inputs(inputs) { }

        // Copy operations
        LogicGate(const LogicGate& other) = default;
        LogicGate& operator=(const LogicGate& other) = default;
            
        // Move operations
        LogicGate(LogicGate&& other) noexcept = default;
        LogicGate& operator=(LogicGate&& other) noexcept = default;

        // Getters non-const
        InputsType<Pin<N>*> getInputs() 
        {
            InputsType<Pin<N>*> inputs;
            inputs.reserve(m_inputs.size());

            for (const auto& in : m_inputs)
            {
                inputs.push_back(in.get());
            }
            return inputs;
        }

        Pin<N>* getOutput() 
        {
            return m_output.get();
        }

        // Getters const
        const InputsType<Pin<N>*> getInputs() const 
        {
            InputsType<Pin<N>*> inputs;
            inputs.reserve(m_inputs.size());

            for (const auto& in : m_inputs)
            {
                inputs.push_back(in.get());
            }
            return inputs;
        }

        const Pin<N>* getOutput() const
        {
            return m_output.get();
        }

        // Poor virtual function
        virtual void calcOut() = 0;
        
    public:
        Pos<N> m_pos;
        Frame<N> m_frame;
        std::shared_ptr<Pin<N>> m_output;
        InputsType<std::shared_ptr<Pin<N>>> m_inputs;
    };

    // Basic classes (AND and OR)
    template <
        Number N,
        template<typename, typename ...> 
            class InputsType = std::vector
    >
    class And : public LogicGate<N>
    {
    public:
        And (const Pos<N>& pos,
             const Frame<N>& frame,
             std::shared_ptr<Pin<N>>& output, 
             InputsType<std::shared_ptr<Pin<N>>>& inputs) :

            LogicGate<N>(pos, frame, output, inputs) 
        { 
            calcOut();
        }

        void calcOut() override 
        {
            bool out = { true };

            for (const auto& in : this->m_inputs)
            {
                if (in->state == Pin<N>::State::Undefined)
                {
                    this->m_output->state = Pin<N>::State::Undefined;
                    return;
                }
            }

            for (const auto& in : this->m_inputs)
            {
                if (in->state == Pin<N>::State::HighImpedance) 
                {
                    this->m_output->state = Pin<N>::State::HighImpedance;
                    return;
                }

                out &= static_cast<bool>(in->state);
            }

            this->m_output->state = out ? Pin<N>::State::LogicOne : Pin<N>::State::LogicZero;
        }
    };

    template <
        Number N,
        template<typename, typename ...> 
            class InputsType = std::vector
    >
    class Or : public LogicGate<N>
    {
    public:
        Or (const Pos<N>& pos,
            const Frame<N>& frame,
            std::shared_ptr<Pin<N>>& output, 
            InputsType<std::shared_ptr<Pin<N>>>& inputs) :

        LogicGate<N>(pos, frame, output, inputs) 
        {
            calcOut();
        }

        void calcOut() override
        {
            bool out { false };

            for (const auto& in : this->m_inputs)
            {
                if (in->state == Pin<N>::State::Undefined)
                {
                    this->m_output->state = Pin<N>::State::Undefined;
                    return;
                }
            }

            for (const auto& in : this->m_inputs)
            {
                if (in->state == Pin<N>::State::HighImpedance) 
                {
                    this->m_output->state = Pin<N>::State::HighImpedance;
                    return;
                }

                out |= static_cast<bool>(in->state);
            }

            this->m_output->state = out ? Pin<N>::State::LogicOne : Pin<N>::State::LogicZero;
        }
    };

    // Decorator 
    template<Number N>
    class Decorator : public LogicGate<N>
    {
    protected:
        Decorator(std::unique_ptr<LogicGate<N>> gate) :
            LogicGate<N>(gate->m_pos, gate->m_frame, gate->m_output, gate->m_inputs), 
            m_gate(std::move(gate)) { }

    public:
        std::unique_ptr<LogicGate<N>> m_gate;
    };

    // Concrete Decorator (Not)
    template<Number N>
    class Not : public Decorator<N>
    {
    public:
        Not(std::unique_ptr<LogicGate<N>> gate) : Decorator<N>(std::move(gate))
        {
            calcOut();
        }

        void calcOut() override
        {
            this->m_gate->calcOut();

            switch (this->m_output->state)
            {
                case Pin<N>::State::LogicZero: {
                    this->m_output->state = Pin<N>::State::LogicOne;
                    break;
                }
                case Pin<N>::State::LogicOne: {
                    this->m_output->state = Pin<N>::State::LogicZero;
                    break;
                }
                case Pin<N>::State::HighImpedance: {
                    this->m_output->state = Pin<N>::State::HighImpedance;
                    break;
                }
                default: {
                    this->m_output->state = Pin<N>::State::Undefined;
                    break;   
                }
            }
        }
    };
}

#endif // CIRCUITS_HPP