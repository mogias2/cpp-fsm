#pragma once

class ObjManager : public Singleton<ObjManager>
{
public:
	ObjManager();
	~ObjManager();

	bool Run();

private:
	bool Init();
};

#define OBJ_MANAGER ObjManager::GetInstance()
