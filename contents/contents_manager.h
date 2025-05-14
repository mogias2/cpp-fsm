#include "json_tool.h"
#include "contents.h"

template <typename CONTENT>
class ContentsVector final
{
public:
	typedef typename std::vector<CONTENT*>              TContentsVector;
	typedef typename TContentsVector::const_iterator    TContentsVectorConstIt;

	ContentsVector() {}
	~ContentsVector() { Release(); }

	const CONTENT* Get(const uint32 id) const;
	const std::vector<CONTENT*>& GetList() const;
	bool AddData(CONTENT* content);
	bool Load(const char* file_name);
	uint32 Size() const { return data_.size(); }

private:
	void Release();

private:
	TContentsVector data_;
};

template <typename CONTENT>
void ContentsVector<CONTENT>::Release()
{
	CLEAR_VECTOR(data_);
}

template <typename CONTENT>
const CONTENT* ContentsVector<CONTENT>::Get(const uint32 id) const
{
	if (id >= 0 && id < data_.size())
	{
		return data_[id];
	}

	std::cout << "invalid content id: " << id << std::endl;
	_ASSERT(0);
	return nullptr;
}

template <typename CONTENT>
const std::vector<CONTENT*>& ContentsVector<CONTENT>::GetList() const
{
	return data_;
}

template <typename CONTENT>
bool ContentsVector<CONTENT>::AddData(CONTENT* content)
{
	data_.push_back(content);
	return true;
}

template <typename CONTENT>
bool ContentsVector<CONTENT>::Load(const char* file_name)
{
	_ASSERT(file_name != nullptr);
	rapidjson::Document doc;
	if (!JsonTool::LoadJsonFile(file_name, doc))
	{
		return false;
	}

	if (!CONTENT::Load(doc, data_))
	{
		std::cout << "failed to load content: " << file_name << std::endl;
		_ASSERT(false);
		return false;
	}

	return true;
}

class ContentsManager : public Singleton<ContentsManager>
{
public:
	ContentsManager() {}
	~ContentsManager() {}

	bool LoadContents();
	const ContentsTransition* GetTransition() const;

public:
	ContentsVector<ContentsTransition> transition_;
};

#define CONTENTS_MANAGER ContentsManager::GetInstance()
