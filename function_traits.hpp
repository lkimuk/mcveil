#ifndef MC_FUNCTION_TRAITS_HPP
#define MC_FUNCTION_TRAITS_HPP

#include <tuple>
#include <functional>
#include <type_traits>


namespace mc
{

template<typename T>
struct function_traits;

template<typename R, typename... Args>
struct function_traits<std::function<R(Args...)>>
{
	static constexpr std::size_t value = sizeof...(Args);
	using result_type = R;

	template<size_t I>
	struct get {
		using type = typename std::tuple_element<I, std::tuple<Args...>>::type;
	};
};

using PlaceholdersList = std::tuple<decltype(std::placeholders::_1),
									decltype(std::placeholders::_2),
									decltype(std::placeholders::_3),
									decltype(std::placeholders::_4),
									decltype(std::placeholders::_5),
									decltype(std::placeholders::_6),
									decltype(std::placeholders::_7),
									decltype(std::placeholders::_8),
									decltype(std::placeholders::_9),
									decltype(std::placeholders::_10),
									decltype(std::placeholders::_11),
									decltype(std::placeholders::_12),
									decltype(std::placeholders::_13),
									decltype(std::placeholders::_14),
									decltype(std::placeholders::_15),
									decltype(std::placeholders::_16),
									decltype(std::placeholders::_17),
									decltype(std::placeholders::_18),
									decltype(std::placeholders::_19),
									decltype(std::placeholders::_20)>;

template<std::size_t... Is, typename F, typename... Args>
auto bind_helper(std::index_sequence<Is...>, const F& f, Args&&... args)
{
	return std::bind(f, std::forward<Args>(args)..., 
		typename std::tuple_element<Is, PlaceholdersList>::type{}...);
}


template<typename FunctionType, typename F, typename... Args>
auto binder(const F& f, Args&&... args)
{
	return bind_helper(std::make_index_sequence<
		function_traits<FunctionType>::value>{}, f, std::forward<Args>(args)...);
}

} // namespace mc

#endif // MC_FUNCTION_TRAITS_HPP
