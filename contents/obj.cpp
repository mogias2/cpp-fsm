#include "pch.h"
#include "obj.h"
#include "contents/contents_manager.h"

//////////////////////////////////////////////////////////////////////////////////////
// FSMObject
FSMObject::FSMObject(const uint32 state_count, const char* object_name/* = nullptr*/)
	: BaseObject()
	, state_handler_(new TStateHandler(state_count, object_name))
{
}

FSMObject::FSMObject(const uint64 object_id, const uint32 state_count,
	const char* object_name/* = nullptr*/)
	: BaseObject(object_id)
	, state_handler_(new TStateHandler(state_count, object_name))
{
}

FSMObject::~FSMObject()
{
	SAFE_DELETE(state_handler_);
}


//////////////////////////////////////////////////////////////////////////////////////
// Obj
Obj::Obj(const uint64 object_id)
	: FSMObject(object_id, eObjState::COUNT_, "obj")
	, input_index_(0)
{
}

Obj::~Obj()
{
}

void Obj::RegisterStateAction()
{
	state_handler()->AddState<Obj>(eObjState::ACTIVE, *this, &Obj::EnterActive, nullptr, &Obj::LeaveActive);
	state_handler()->AddState<Obj, TObjStateParam>(eObjState::IDLE, *this, &Obj::EnterIdle, nullptr, &Obj::LeaveIdle);
	state_handler()->AddState<Obj>(eObjState::PLAY, *this, &Obj::EnterPlay, nullptr, &Obj::LeavePlay);
	state_handler()->AddState<Obj>(eObjState::END, *this, &Obj::EnterEnd, nullptr, &Obj::LeaveEnd);
	//state_handler()->AddState(eObjState::END, *this);
}

void Obj::InitFSM()
{
	const auto content = CONTENTS_MANAGER.GetTransition();
	if (content == nullptr)
	{
		return;
	}

	for (const auto& transition : content->transitions)
	{
		if (!VALID_STATE_ID(transition.current) || !VALID_INPUT_ID(transition.input) || !VALID_STATE_ID(transition.next))
		{
			std::cout << "invalid content " << transition.current << " " << transition.input << " " << transition.next << std::endl;
			continue;
		}
		state_handler()->AddTransition(transition.current, transition.input, transition.next);
	}

	state_handler()->SetDefaultState(0);
	ChangeStateAuto(0);
}

uint32 Obj::GetCurrentStateId() const
{
	return state_handler()->GetCurrentStateId();
}

bool Obj::Update()
{
	const auto content = CONTENTS_MANAGER.GetTransition();
	if (content == nullptr)
	{
		return false;
	}

	for (auto i = 1; i < content->actions.size(); ++i)
	{
		state_handler()->UpdateState();
		ChangeStateAuto(i);
	}

	state_handler()->EndState();
	return true;
}

void Obj::ChangeState(const uint32 input)
{
	std::cout << "OBJ(" << object_id_ << ") " << "state(" << GetCurrentStateId() << ") " << "input(" << input << ")" << std::endl;
	if (!state_handler()->TransitState(input))
	{
		//std::cout << "OBJ(" << object_id_ << ") " << "cannot transit state(" << GetCurrentStateId() << ") " << "input(" << input << ")" << std::endl;
		return;
	}
}

void Obj::ChangeStateAuto(const uint32 index)
{
	const auto content = CONTENTS_MANAGER.GetTransition();
	if (content == nullptr)
	{
		return;
	}

	const auto& actions = content->actions;
	auto input = actions[index];

	auto action = state_handler()->FindStateAction(input);
	if (action && action->HasParam())
	{
		auto param_action = reinterpret_cast<TParamAction*>(action.get());
		param_action->param_.Replace<0>(GetCurrentStateId());
		param_action->param_.Replace<1>(input);
	}

	ChangeState(input);
	::Sleep(1000);
}

void Obj::EnterActive()
{
	std::cout << "OBJ(" << object_id_ << ") " << "Enter Active" << std::endl;
}

void Obj::LeaveActive()
{
	std::cout << "OBJ(" << object_id_ << ") " << "Leave Active" << std::endl;
}

void Obj::EnterIdle(const TObjStateParam& param)
{
	std::cout << "OBJ(" << object_id_ << ") " << "Enter Idle param:" << param.Get<0>() << " " << param.Get<1>() << std::endl;
}

void Obj::LeaveIdle()
{
	std::cout << "OBJ(" << object_id_ << ") " << "Leave Idle" << std::endl;
}

void Obj::EnterPlay()
{
	std::cout << "OBJ(" << object_id_ << ") " << "Enter Play" << std::endl;
}

void Obj::LeavePlay()
{
	std::cout << "OBJ(" << object_id_ << ") " << "Leave Play" << std::endl;
}

void Obj::EnterEnd()
{
	std::cout << "OBJ(" << object_id_ << ") " << "Enter End" << std::endl;
}

void Obj::LeaveEnd()
{
	std::cout << "OBJ(" << object_id_ << ") " << "Leave End" << std::endl;
}
