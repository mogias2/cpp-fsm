#pragma once

#include <tuple>
#include <type_traits>

namespace fsm {

template <typename T, typename Tuple>
struct has_type;

template <typename T>
struct has_type<T, std::tuple<>> : std::false_type{};

template <typename T, typename U, typename ... ARGS>
struct has_type<T, std::tuple<U, ARGS...>> : has_type<T, std::tuple<ARGS...>>{};

template <typename T, typename ... ARGS>
struct has_type<T, std::tuple<T, ARGS...>> : std::true_type{};

struct StateNoneParam
{
	virtual bool HasParam() const { return false; }
};

template <typename ... ARGS>
struct StateParam : public StateNoneParam
{
	typedef typename std::tuple<ARGS...> Tuple;

	template <std::size_t N>
	using tuple_type = typename std::tuple_element<N, Tuple>::type;

	template <typename T, typename Tuple>
	using tuple_contains_type = typename has_type<T, Tuple>::type;

	Tuple tuple_;

	void Set(ARGS...args)
	{
		tuple_ = std::make_tuple(args...);
	}

	template <uint32 INDEX>
	const tuple_type<INDEX> Get() const
	{
		return std::get<INDEX>(tuple_);
	}

	template <uint32 INDEX>
	void Replace(tuple_type<INDEX> val)
	{
		_ASSERT(HasParam());
		if (tuple_contains_type<tuple_type<INDEX>, Tuple>())
		{
			std::get<INDEX>(tuple_) = val;
		}
		else
		{
			_ASSERT(false);
		}
	}

	const size_t Size() const
	{
		return std::tuple_size<Tuple>::value;
	}

	virtual bool HasParam() const override
	{
		return Size() > 0;
	}
};

} // namespace fsm
