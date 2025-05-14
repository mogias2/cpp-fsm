#pragma once

#include "rapidjson/document.h"

#pragma warning(suppress : 4996)

class JsonTool
{
public:
	static bool LoadFile(const char* file_name, std::string& buf, bool binary);
	static bool LoadJsonFile(const char* file_name, rapidjson::Document& doc, bool binary = false);
	static uint32 GetUint32(const rapidjson::Value& value);
	static uint8 GetUint8(const rapidjson::Value& value);
	static uint16 GetUint16(const rapidjson::Value& value);
	static uint64 GetUint64(const rapidjson::Value& value);
	static const char* GetString(const rapidjson::Value& value);
	static double GetDouble(const rapidjson::Value& value);
	static float GetFloat(const rapidjson::Value& value);

private:
	JsonTool() = delete;
	~JsonTool() = delete;
};

#define JSON_GET_UINT32(VALUE) JsonTool::GetUint32(VALUE)
#define JSON_GET_UINT8(VALUE) JsonTool::GetUint8(VALUE)
#define JSON_GET_UINT16(VALUE) JsonTool::GetUint16(VALUE)
#define JSON_GET_UINT64(VALUE) JsonTool::GetUint64(VALUE)
#define JSON_GET_STRING(VALUE) JsonTool::GetString(VALUE)
#define JSON_GET_DOUBLE(VALUE) JsonTool::GetDouble(VALUE)
#define JSON_GET_FLOAT(VALUE) JsonTool::GetFloat(VALUE)

#define FLOAT_ROUND(x, digit) \
	(::floor((x) * ::pow(float(10), digit) + 0.5f) / ::pow(float(10), digit))
