#include "pch.h"
#include "json_tool.h"
#include <fstream>
#include <sstream>
#include <string>

bool JsonTool::LoadJsonFile(const char* file_name, rapidjson::Document& doc, bool binary/* = false*/)
{
	std::string buf;
	if (!JsonTool::LoadFile(file_name, buf, binary))
	{
		std::cout << "failed to load json file: " << file_name << std::endl;
		_ASSERT(false);
		return false;
	}

	if (doc.Parse<0>(buf.c_str()).HasParseError())
	{
		std::cout << "failed to parse json file: " << file_name << std::endl;
		_ASSERT(false);
		return false;
	}
	return true;
}

bool JsonTool::LoadFile(const char* file_name, std::string& buf, bool binary)
{
	std::ifstream ifs(file_name, binary ? std::ifstream::binary : std::ifstream::in);
	if (!ifs.is_open()) return false;
	if (binary)
	{
		// The fastest way to read a file into a string.
		ifs.seekg(0, std::ios::end);
		buf.resize(static_cast<size_t>(ifs.tellg()));
		ifs.seekg(0, std::ios::beg);
		ifs.read(&buf[0], buf.size());
	}
	else
	{
		// This is slower, but works correctly on all platforms for text files.
		std::ostringstream oss;
		oss << ifs.rdbuf();
		buf = oss.str();
	}
	return !ifs.bad();
}

uint32 JsonTool::GetUint32(const rapidjson::Value& value)
{
	if (value.IsUint())
	{
		return value.GetUint();
	}
	else if (value.IsString())
	{
		char** end_ptr = nullptr;
		auto val = static_cast<uint32>(::strtoul(value.GetString(), end_ptr, 10));
		_ASSERT(end_ptr == nullptr);
		return val;
	}
	else
	{
		_ASSERT(false);
	}
	return 0;
}

uint8 JsonTool::GetUint8(const rapidjson::Value& value)
{
	auto val = GetUint32(value);
	_ASSERT(val <= UCHAR_MAX);
	return static_cast<uint8>(val);
}

uint16 JsonTool::GetUint16(const rapidjson::Value& value)
{
	auto val = GetUint32(value);
	_ASSERT(val <= USHRT_MAX);
	return static_cast<uint16>(val);
}

uint64 JsonTool::GetUint64(const rapidjson::Value& value)
{
	if (value.IsUint64())
	{
		return value.GetUint64();
	}
	else if (value.IsString())
	{
		char** end_ptr = nullptr;
		auto val = ::strtoull(value.GetString(), end_ptr, 10);
		_ASSERT(end_ptr == nullptr);
		return val;
	}
	else
	{
		_ASSERT(false);
	}
	return 0ULL;
}

const char* JsonTool::GetString(const rapidjson::Value& value)
{
	_ASSERT(value.IsString());
	return value.GetString();
}

double JsonTool::GetDouble(const rapidjson::Value& value)
{
	if (value.IsDouble())
	{
		return value.GetDouble();
	}
	else if (value.IsString())
	{
		return std::stod(value.GetString());
	}
	else
	{
		_ASSERT(false);
	}
	return static_cast<double>(0);
}

float JsonTool::GetFloat(const rapidjson::Value& value)
{
	auto val = GetDouble(value);
	_ASSERT(::fabs(val) >= FLT_MIN && ::fabs(val) <= FLT_MAX);
	return static_cast<float>(val);
}
