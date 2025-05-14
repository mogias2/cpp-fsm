#pragma once

#include "state_param.h"

namespace fsm {

class StateActionBase
{
public:
	StateActionBase() : input_event_(0), expiration_(0) {}
	virtual ~StateActionBase() {}

	virtual void OnEnter() {}
	virtual void OnUpdate() {}
	virtual void OnExit() {}

	void SetDuration(const uint32 duration, const uint32 input_event)
	{
		expiration_ = ::_time32(nullptr) + duration;
		input_event_ = input_event;
	}

	inline bool IsExpire() const
	{
		if (expiration_ == 0)
		{
			return false;
		}

		auto now = ::_time32(nullptr);
		return (expiration_ <= now);
	}

	uint32 input_event() const { return input_event_; }
	virtual bool HasParam() const { return false; }

protected:
	uint32 input_event_;
	__time32_t expiration_;
};

template <class T, typename PARAM = StateNoneParam>
class StateAction : public Container<T, StateActionBase>
{
public:
	typedef void(T::*Delegate)();
	typedef void(T::*DelegateWithParam)(const PARAM& param);

public:
	StateAction(T& container, DelegateWithParam on_enter_with_param, Delegate on_update, Delegate on_exit)
		: Container<T, StateActionBase>(container), on_enter_with_param_(on_enter_with_param)
		, on_enter_(nullptr), on_update_(on_update), on_exit_(on_exit) {}
	StateAction(T& container, Delegate on_enter, Delegate on_update, Delegate on_exit)
		: Container<T, StateActionBase>(container), on_enter_with_param_(nullptr)
		, on_enter_(on_enter), on_update_(on_update), on_exit_(on_exit) {}

	virtual ~StateAction() {}

	virtual void OnEnter() override
	{
		if (on_enter_with_param_)
		{
			(this->GetContainer().*on_enter_with_param_)(param_);
		}

		if (on_enter_)
		{
			(this->GetContainer().*on_enter_)();
		}
	}

	virtual void OnUpdate() override
	{
		if (on_update_)
		{
			(this->GetContainer().*on_update_)();
		}
	}

	virtual void OnExit() override
	{
		if (on_exit_)
		{
			(this->GetContainer().*on_exit_)();
		}
	}

	virtual bool HasParam() const override { return param_.HasParam(); }

public:
	PARAM param_;

protected:
	DelegateWithParam on_enter_with_param_;
	Delegate on_enter_;
	Delegate on_update_;
	Delegate on_exit_;
};

} // namespace fsm
