#ifndef MC_NONCOPYABLE_HPP
#define MC_NONCOPYABLE_HPP

namespace mc
{

class noncopyable
{
protected:
	noncopyable() = default;
	~noncopyable() = default;

	noncopyable(const noncopyable&) = delete;
	const noncopyable& operator=(const noncopyable&) = delete;
};

} // namespace mc

#endif // MC_NONCOPYABLE_HPP
