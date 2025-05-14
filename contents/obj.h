#pragma once

#include "fsm/state_handler.h"

namespace fsm {
	struct StateNoneParam;
}

class IObject
{
public:
	IObject() {}
	virtual ~IObject() {}
	virtual bool Init() = 0;
	virtual void Destroy() = 0;
};

class BaseObject : public IObject
{
public:
	BaseObject() : object_id_(0ULL) {}
	explicit BaseObject(const uint64 object_id) : object_id_(object_id) {}
	virtual ~BaseObject() {}

	// from IObject
	virtual bool Init() override { return true; }
	virtual void Destroy() override {}

	const uint64 object_id() const { return object_id_; }

protected:
	uint64 object_id_;
};

class FSMObject : public BaseObject
{
public:
	typedef fsm::StateHandler TStateHandler;

public:
	FSMObject(const uint32 state_count, const char* object_name = nullptr);
	FSMObject(const uint64 object_id, const uint32 state_count, const char* object_name = nullptr);
	virtual ~FSMObject();

	// from IObject
	virtual bool Init() override
	{
		RegisterStateAction();
		InitFSM();
		return true;
	}

	virtual void Destroy() override
	{
	}

protected:
	virtual void RegisterStateAction() = 0;
	virtual void InitFSM() = 0;
	TStateHandler* state_handler() { return state_handler_; }
	const TStateHandler* state_handler() const { return state_handler_; }

protected:
	TStateHandler* state_handler_;
};

struct eObjState
{
	enum : uint32
	{
		ACTIVE,         // 0
		IDLE,           // 1
		PLAY,           // 2
		END,
		COUNT_,
	};
};

#define VALID_STATE_ID(ID)  (ID >= eObjState::ACTIVE && ID < eObjState::COUNT_)

struct eObjInput
{
	enum : uint32
	{
		DO_IDLE,            // 0
		DO_PLAY,            // 1
		DO_END,
		COUNT_,
	};
};

#define VALID_INPUT_ID(ID)  (ID >= eObjInput::DO_IDLE && ID < eObjInput::COUNT_)

class Obj : public FSMObject
{
public:
	typedef fsm::StateParam<uint32, uint32>			TObjStateParam;
	typedef fsm::StateAction<Obj, TObjStateParam>	TParamAction;

public:
	explicit Obj(const uint64 object_id);
	~Obj();

	uint32 GetCurrentStateId() const;
	bool Update();
	void ChangeState(const uint32 input);
	void ChangeStateAuto(const uint32 index);
	void EnterActive();
	void LeaveActive();
	void EnterIdle(const TObjStateParam& param);
	void LeaveIdle();
	void EnterPlay();
	void LeavePlay();
	void EnterEnd();
	void LeaveEnd();

private:
	//--------------------------------------------------------------------------
	// from FSMObject
	virtual void RegisterStateAction() override;
	virtual void InitFSM() override;
	//==========================================================================

private:
	uint32 input_index_;

};

//typedef std::shared_ptr<bot_client::Bot> TSharedPtrBot;
