#ifndef MC_ASPECT_HPP
#define MC_ASPECT_HPP

namespace mc
{

template<typename Aspect, typename... Args>
concept HasMemberBefore = requires (Aspect ap) { ap.before(std::declval<Args>()...); };

template<typename Aspect, typename... Args>
concept HasMemberAfter = requires (Aspect ap) { ap.after(std::declval<Args>()...); };

template<typename Func>
struct aspect
{
	aspect(Func&& func) : func_(std::forward<Func>(func))
	{}

	template<typename... Aspects, typename... Args>
	void invoke(Args&&... args)
	{
		auto invoke_before = [&](auto&& ap) {
			if constexpr (HasMemberBefore<decltype(ap), Args...>) 
				ap.before(std::forward<Args>(args)...);
		};

		auto invoke_after = [&](auto&& ap) {
			if constexpr (HasMemberAfter<decltype(ap), Args...>) 
				ap.after(std::forward<Args>(args)...);
		};

		int dummy;
		(invoke_before(Aspects()), ...);
		func_(std::forward<Args>(args)...);
		(dummy = ... = (invoke_after(Aspects()), 0));
	}

private:
	Func func_;
};


template<typename... Aspects, typename... Args, typename Func>
void make_aspect(Func&& func, Args&&... args)
{
	mc::aspect<Func> ap(std::forward<Func>(func));
	ap.invoke<Aspects...>(std::forward<Args>(args)...);
}

} // namespace mc

#endif // MC_ASPECT_HPP