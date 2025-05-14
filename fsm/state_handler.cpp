#include "pch.h"
#include "state_handler.h"

namespace fsm {

StateHandler::StateHandler(const uint32 state_count, const char* name/* = nullptr*/)
	: fsm_(name)
	, current_action_(nullptr)
{
	action_vector_.resize(state_count);
}

StateHandler::~StateHandler()
{
	action_vector_.clear();
}

bool StateHandler::IsValidStateId(const uint32 state_id) const
{
	return (state_id >= 0) && (state_id < action_vector_.size());
}

StateHandler::StateActionBasePtr StateHandler::GetStateAction(const uint32 state_id) const
{
	if (!IsValidStateId(state_id))
	{
		std::cout << "invalid state id: " << state_id << std::endl;
		_ASSERT(0);
		return nullptr;
	}

	return action_vector_[state_id];
}

bool StateHandler::HasState(const uint32 state_id) const
{
	if (!IsValidStateId(state_id))
	{
		return false;
	}

	return action_vector_[state_id] != nullptr;
}

bool StateHandler::AddTransition(const uint32 state_id, const uint32 input_event, const uint32 output_state_id)
{
	return fsm().AddStateTransition(state_id, input_event, output_state_id);
}

bool StateHandler::SetCurrentStateAction(const uint32 state_id)
{
	if (!IsValidStateId(state_id) || action_vector_[state_id] == nullptr)
	{
		std::cout << "invalid state id: " << state_id << std::endl;
		_ASSERT(0);
		return false;
	}

	current_action_ = action_vector_[state_id];
	return true;
}

void StateHandler::SetDefaultState(const uint32 state_id)
{
	if (!SetCurrentStateAction(state_id))
	{
		return;
	}

	fsm().SetCurrentState(state_id);
	current_action_->OnEnter();
}

bool StateHandler::TransitState(const uint32 input_event)
{
	if (current_action_ == nullptr)
	{
		std::cout << "invalid current state: " << fsm().GetName() << " " << GetCurrentStateId() << std::endl;
		return false;
	}

	const auto current_state_id = GetCurrentStateId();
	if (!fsm().TransitState(input_event))
	{
		return false;
	}

	if (current_state_id != GetCurrentStateId())
	{
		current_action_->OnExit();
		SetCurrentStateAction(GetCurrentStateId());
		current_action_->OnEnter();
	}
	return true;
}

void StateHandler::EndState()
{
	current_action_->OnExit();
}

void StateHandler::UpdateState()
{
	if (current_action_ == nullptr)
	{
		std::cout << "invalid current state: " << fsm().GetName() << " " << GetCurrentStateId() << std::endl;
		return;
	}

	if (current_action_->IsExpire())
	{
		TransitState(current_action_->input_event());
		return;
	}

	current_action_->OnUpdate();
}

size_t StateHandler::GetStateCount() const
{
	return action_vector_.size();
}

uint32 StateHandler::GetCurrentStateId() const
{
	return fsm().GetCurrentStateId();
}

StateHandler::StateActionBasePtr StateHandler::GetCurrentStateAction() const
{
	return GetStateAction(GetCurrentStateId());
}

void StateHandler::SetStateDuration(const uint32 state_id, const uint32 duration, const uint32 input_event)
{
	auto state = GetStateAction(state_id);
	if (state && fsm().CanTransitState(state_id, input_event))
	{
		state->SetDuration(duration, input_event);
	}
}

StateHandler::StateActionBasePtr StateHandler::FindStateAction(const uint32 input_event) const
{
	const auto output_state_id = fsm().FindOutputStateId(input_event);
	return GetStateAction(output_state_id);
}

} // namespace fsm
