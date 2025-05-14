#include "pch.h"
#include "finite_state_machine.h"
#include "finite_state.h"

namespace fsm {

FiniteStateMachine::FiniteStateMachine(const char* name/* = nullptr*/)
	: current_state_time_(0)
	, current_state_(nullptr)
{
	if (name != nullptr)
	{
		name_ = name;
	}
}

FiniteStateMachine::~FiniteStateMachine()
{
	state_map_.clear();
}

uint32 FiniteStateMachine::GetCurrentStateId() const
{
	if (current_state_ == nullptr)
	{
		std::cout << "current state is null" << std::endl;
		_ASSERT(0);
		return InvalidStateId;
	}

	return current_state_->GetStateId();
}

FiniteStateMachine::FiniteStatePtr FiniteStateMachine::GetState(const uint32 state_id) const
{
	const auto iter = state_map_.find(state_id);
	return (iter != state_map_.end()) ? (*iter).second : nullptr;
}

FiniteStateMachine::FiniteStatePtr FiniteStateMachine::AddState(const uint32 state_id)
{
	auto state = GetState(state_id);
	if (state == nullptr)
	{
		auto tmpState = std::make_shared<FiniteState>(state_id);
		const auto result = state_map_.emplace(state_id, tmpState);
		if (!result.second)
		{
			tmpState.reset();
			std::cout << "failed to insert state: " << state_id << std::endl;
			_ASSERT(0);
			return nullptr;
		}
		state = tmpState;
	}

	return state;
}

bool FiniteStateMachine::AddStateTransition(const uint32 state_id, const uint32 input_event, const uint32 output_state_id)
{
	auto state = AddState(state_id);
	if (!state)
	{
		return false;
	}

	return state->AddTransition(input_event, output_state_id);
}

void FiniteStateMachine::DeleteStateTransition(const uint32 state_id, const uint32 input_event)
{
	auto state = GetState(state_id);
	if (!state)
	{
		std::cout << GetName() << " cannot find state(" << state_id << ")" << std::endl;
		return;
	}

	state->DeleteTransition(input_event);
	if (state->GetCount() == 0)
	{
		state_map_.erase(state->GetStateId());
	}
}

bool FiniteStateMachine::SetCurrentState(const uint32 state_id)
{
	auto state = GetState(state_id);
	if (!state)
	{
		std::cout << GetName() << " cannot find state(" << state_id << ")" << std::endl;
		return false;
	}

	current_state_ = state;
	current_state_time_ = ::time(nullptr);
	return true;
}

bool FiniteStateMachine::TransitState(const uint32 input_event)
{
	if (current_state_ == nullptr)
	{
		std::cout << GetName() << " current state is null" << std::endl;
		return false;
	}

	const auto [ouput_state_id, ok] = current_state_->GetOutputState(input_event);
	if (!ok)
	{
		std::cout << GetName() << " cannot find output state" << current_state_->GetStateId() << " " << input_event << std::endl;
		return false;
	}

	return SetCurrentState(ouput_state_id);
}

FiniteStateMachine::FiniteStatePtr FiniteStateMachine::GetTransitState(const uint32 state_id, const uint32 input_event) const
{
	const auto state = GetState(state_id);
	if (!state)
	{
		std::cout << GetName() << " cannot find state(" << state_id << ")" << std::endl;
		return nullptr;
	}

	const auto [ouput_state_id, ok] = state->GetOutputState(input_event);
	if (!ok)
	{
		std::cout << GetName() << " cannot find output state" << state->GetStateId() << " " << input_event << std::endl;
		return nullptr;
	}

	return GetState(ouput_state_id);
}

bool FiniteStateMachine::CanTransitState(const uint32 state_id, const uint32 input_event) const
{
	const auto state = GetState(state_id);
	if (!state)
	{
		std::cout << GetName() << " cannot find state(" << state_id << ")" << std::endl;
		return false;
	}

	return state->FindOutputState(input_event);
}

uint32 FiniteStateMachine::FindOutputStateId(const uint32 input_event) const
{
	const auto state = GetState(GetCurrentStateId());
	if (!state)
	{
		std::cout << GetName() << " cannot find state(" << GetCurrentStateId() << ")" << std::endl;
		return InvalidStateId;
	}

	const auto [output_state_id, _] = state->GetOutputState(input_event);
	return output_state_id;
}

} // namespace fsm
