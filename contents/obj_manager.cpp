#include "pch.h"
#include "obj_manager.h"
#include "contents_manager.h"
#include "obj.h"

ObjManager::ObjManager()
{
}

ObjManager::~ObjManager()
{
}

bool ObjManager::Init()
{
	if (!CONTENTS_MANAGER.LoadContents())
	{
		return false;
	}
	return true;
}

bool ObjManager::Run()
{
	if (!Init())
	{
		return false;
	}

	auto obj = std::make_unique<Obj>(1);
	obj->Init();
	obj->Update();
	return true;
}
