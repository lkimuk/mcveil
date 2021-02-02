/*
 *
MIT License

Copyright (c) 2021 gxkey(Gaoxing Li)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef MC_ASPECT_HPP
#define MC_ASPECT_HPP

#include <concepts>
#include <utility>
#include "../noncopyable.hpp"


namespace mc
{

template<typename Advice, typename... Args>
concept HasMemberBefore = requires (Advice advice) { advice.before(std::declval<Args>()...); };

template<typename Advice, typename... Args>
concept HasMemberAfter = requires (Advice advice) { advice.after(std::declval<Args>()...); };

template<typename Func, typename... Args>
struct aspect final : noncopyable
{
	aspect(Func&& func) : func_(std::forward<Func>(func))
	{}

	template<typename... Advice>
	void invoke(Args&&... args, Advice&&... advice) const
	{
		auto invoke_before = [&](auto&& ad) {
			if constexpr (HasMemberBefore<decltype(ad), Args...>) 
				ad.before(std::forward<Args>(args)...);
		};

		auto invoke_after = [&](auto&& ad) {
			if constexpr (HasMemberAfter<decltype(ad), Args...>) 
				ad.after(std::forward<Args>(args)...);
		};

		int dummy;
		(invoke_before(advice), ...);
		func_(std::forward<Args>(args)...);
		(dummy = ... = (invoke_after(advice), 0));
	}

private:
	Func func_;
};



template<typename... Advice, typename... Args, typename Func>
void make_aspect(Func&& func, Args&&... args)
{
	mc::aspect<Func, Args...> ap(std::forward<Func>(func));
	ap.invoke(std::forward<Args>(args)..., Advice()...);
}

} // namespace mc

#endif // MC_ASPECT_HPP
