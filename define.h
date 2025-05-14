#pragma once

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

constexpr int INVALID_INDEX = -1;

#define DISALLOW_COPY_AND_ASSIGN(TypeName)          \
	TypeName(const TypeName&) = delete;             \
	TypeName& operator=(const TypeName&) = delete;

#define SAFE_DELETE(p) do { if(p) { delete (p); (p) = nullptr; } } while(0)

#define CLEAR_MAP(MAP)                                          \
	for (auto it = MAP.begin(); it != MAP.end(); ++it)          \
	{                                                           \
		SAFE_DELETE((*it).second);                              \
	}                                                           \
	MAP.clear()

#define CLEAR_VECTOR(VECTOR)                                    \
	for (auto it = VECTOR.begin(); it != VECTOR.end(); ++it)    \
	{                                                           \
		SAFE_DELETE(*it);                                       \
	}                                                           \
	VECTOR.clear()
