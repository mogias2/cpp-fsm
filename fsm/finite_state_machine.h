#pragma once

namespace fsm {

class FiniteState;

class FiniteStateMachine
{
public:
	using FiniteStatePtr = std::shared_ptr<FiniteState>;
	typedef std::unordered_map<uint32, FiniteStatePtr> StateMap;

public:
	explicit FiniteStateMachine(const char* name = nullptr);
	virtual ~FiniteStateMachine();

public:
	uint32 GetCurrentStateId() const;
	bool AddStateTransition(const uint32 state_id, const uint32 input_event, const uint32 output_state_id);
	void DeleteStateTransition(const uint32 state_id, const uint32 input_event);
	bool SetCurrentState(const uint32 state_id);
	bool TransitState(const uint32 input_event);
	time_t GetCurrentStateTime() const { return current_state_time_; }
	FiniteStatePtr GetState(const uint32 state_id) const;
	FiniteStatePtr AddState(const uint32 state_id);
	FiniteStatePtr GetTransitState(const uint32 state_id, const uint32 input_event) const;
	bool CanTransitState(const uint32 state_id, const uint32 input_event) const;
	uint32 FindOutputStateId(const uint32 input_event) const;
	std::string_view GetName() const { return name_; }

private:
	std::string name_;
	time_t current_state_time_;
	StateMap state_map_;
	FiniteStatePtr current_state_;
};

} // namespace fsm
