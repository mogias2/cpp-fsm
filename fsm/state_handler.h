#pragma once

#include "state_action.h"
#include "finite_state_machine.h"

namespace fsm {

class StateActionBase;

class StateHandler
{
public:
    template <class T, typename PARAM = StateNoneParam>
    using TStateAction = StateAction<T, PARAM>;

    template <class T>
    using TDelegate = void(T::*)();

    template <class T, typename PARAM>
    using TDelegateWithParam = void(T::*)(const PARAM&);

    using StateActionBasePtr = std::shared_ptr<StateActionBase>;
    using StateActionVector = std::vector<StateActionBasePtr>;

public:
    StateHandler(const uint32 state_count, const char* name = nullptr);
    virtual ~StateHandler();

    template <class T>
    void AddState(const uint32 state_id, T& container, TDelegate<T> on_enter, TDelegate<T> on_update, TDelegate<T> on_exit)
    {
        if (!IsValidStateId(state_id))
        {
            std::cout << "invalid state id: " << state_id << std::endl;
            return;
        }

        if (action_vector_[state_id] != nullptr)
        {
            std::cout << "already state id: " << state_id << std::endl;
            return;
        }

        auto action = std::make_shared<TStateAction<T>>(container, on_enter, on_update, on_exit);
        action_vector_[state_id] = action;
        fsm().AddState(state_id);
    }

    template <class T>
    void AddState(const uint32 state_id, T& container)
    {
        AddState<T>(state_id, container, nullptr, nullptr, nullptr);
    }

    template <class T, typename PARAM = StateNoneParam>
    void AddState(const uint32 state_id, T& container, TDelegateWithParam<T, PARAM> on_enter,
        TDelegate<T> on_update, TDelegate<T> on_exit)
    {
        if (!IsValidStateId(state_id))
        {
            std::cout << "invalid state id: " << state_id << std::endl;
            return;
        }

        if (action_vector_[state_id] != nullptr)
        {
            std::cout << "already state id: " << state_id << std::endl;
            return;
        }

        auto action = std::make_shared<TStateAction<T, PARAM>>(container, on_enter, on_update, on_exit);
        action_vector_[state_id] = action;
        fsm().AddState(state_id);
    }

    StateActionBasePtr GetStateAction(const uint32 state_id) const;
    bool HasState(const uint32 state_id) const;
    bool AddTransition(const uint32 state_id, const uint32 input_event, const uint32 output_state_id);
    void SetDefaultState(const uint32 state_id);
    bool TransitState(const uint32 input_event);
    void UpdateState();
    size_t GetStateCount() const;
    uint32 GetCurrentStateId() const;
    StateActionBasePtr GetCurrentStateAction() const;
    void SetStateDuration(const uint32 state_id, const uint32 duration, const uint32 input_event);
    StateActionBasePtr FindStateAction(const uint32 input_event) const;
    void EndState();

protected:
    FiniteStateMachine& fsm() { return fsm_; }
    const FiniteStateMachine& fsm() const { return fsm_; }

private:
    bool SetCurrentStateAction(const uint32 state_id);
    bool IsValidStateId(const uint32 state_id) const;

private:
    FiniteStateMachine fsm_;
    StateActionBasePtr current_action_;
    StateActionVector action_vector_;
};

} // namespace fsm
