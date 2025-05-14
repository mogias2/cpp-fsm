#include "pch.h"
#include "contents_manager.h"

#define CONTENTS_PATH                   "json/"
#define CONTENTS_FILE_PATH(FILE_NAME)   CONTENTS_PATH##FILE_NAME

bool ContentsManager::LoadContents()
{
	if (!transition_.Load(CONTENTS_FILE_PATH("contents.json")))
	{
		return false;
	}

	return true;
}

const ContentsTransition* ContentsManager::GetTransition() const
{
	return transition_.Get(0);
}
