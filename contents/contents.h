#pragma once

#include "json_tool.h"

//struct ActionProtocol
//{
//    std::string schema_file;
//    std::string json_file;
//};

struct ContentsTransition final
{
	struct Transition final
	{
		uint32 current;
		uint32 input;
		uint32 next;

		Transition(const uint32 _current, const uint32 _input, const uint32 _next)
			: current(_current), input(_input), next(_next)
		{
		}
	};

	std::vector<Transition> transitions;
	std::vector<uint32> actions;

	static bool Load(const rapidjson::Document& doc, std::vector<ContentsTransition*>& transitions)
	{
		auto content = new ContentsTransition();

		if (doc.HasMember("transitions"))
		{
			const auto& transition = doc["transitions"];
			_ASSERT(transition.IsArray());

			for (rapidjson::SizeType i = 0; i < transition.Size(); ++i)
			{
				if (!transition[i].HasMember("current") ||
					!transition[i].HasMember("input") ||
					!transition[i].HasMember("next"))
				{
					_ASSERT(0);
					SAFE_DELETE(content);
					return false;
				}

				const auto& value = transition[i];
				content->transitions.emplace_back(
					Transition(JSON_GET_UINT32(value["current"]),
						JSON_GET_UINT32(value["input"]),
						JSON_GET_UINT32(value["next"])));
			}
		}

		if (doc.HasMember("patterns"))
		{
			const auto& pattern = doc["patterns"];
			_ASSERT(pattern.IsArray());
			for (rapidjson::SizeType i = 0; i < pattern.Size(); ++i)
			{
				if (pattern[i].HasMember("actions"))
				{
					const auto& action = pattern[i]["actions"];
					_ASSERT(action.IsArray());
					for (rapidjson::SizeType j = 0; j < action.Size(); ++j)
					{
						if (action[j].HasMember("input"))
						{
							content->actions.push_back(JSON_GET_UINT32(action[j]["input"]));
						}
					}
				}
			}
		}

		transitions.push_back(content);
		return true;
	}
};
