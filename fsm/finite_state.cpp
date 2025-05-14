#include "pch.h"
#include "finite_state.h"

namespace fsm {

FiniteState::FiniteState(const uint32 state_id)
	: state_id_(state_id)
{
}

FiniteState::~FiniteState()
{
	transition_map_.clear();
}

bool FiniteState::AddTransition(const uint32 input_event, const uint32 output_state_id)
{
	const auto result = transition_map_.emplace(input_event, output_state_id);
	if (!result.second)
	{
		std::cout << "already added transition: " << input_event;
		_ASSERT(0);
		return false;
	}

	return true;
}

void FiniteState::DeleteTransition(const uint32 input_event)
{
	transition_map_.erase(input_event);
}

bool FiniteState::FindOutputState(const uint32 input_event) const
{
	const auto iter = transition_map_.find(input_event);
	return (iter != transition_map_.end());
}

std::tuple<uint32, bool> FiniteState::GetOutputState(const uint32 input_event) const
{
	const auto iter = transition_map_.find(input_event);
	if (iter == transition_map_.end())
	{
		std::cout << "cannot find output state for input: " << input_event << std::endl;
		return { InvalidStateId, false };
	}

	return { (*iter).second, true };
}

size_t FiniteState::GetCount() const
{
	return transition_map_.size();
}

} // namespace fsm
