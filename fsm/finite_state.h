#pragma once

namespace fsm {

const uint32 InvalidStateId = UINT32_MAX;

class FiniteStateMachine;

class FiniteState
{
	friend class FiniteStateMachine;
	typedef std::unordered_map<uint32, uint32> TransitionMap;

public:
	explicit FiniteState(const uint32 state_id);
	virtual ~FiniteState();

private:
	uint32 GetStateId() const { return state_id_; }
	bool AddTransition(const uint32 input_event, const uint32 output_state_id);
	void DeleteTransition(const uint32 input_event);
	bool FindOutputState(const uint32 input_event) const;
	std::tuple<uint32, bool> GetOutputState(const uint32 input_event) const;
	size_t GetCount() const;

private:
	uint32 state_id_;
	TransitionMap transition_map_;
};

} // namespace fsm
