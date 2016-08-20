#ifndef TE_TRI_BOOL_H
#define TE_TRI_BOOL_H

namespace te {

class Tri_bool {
public:
	constexpr Tri_bool() noexcept
		: m_state(State::Null)
	{}
	constexpr Tri_bool(bool state) noexcept
		: m_state{ state ? State::True : State::False }
	{}
private:
	enum class State {
		True,
		False,
		Null
	};
	const State m_state;

	constexpr operator bool() const noexcept
	{
		return m_state == State::True;
	}

	friend constexpr bool operator==(Tri_bool, Tri_bool) noexcept;
};

constexpr bool operator==(Tri_bool lhs, Tri_bool rhs) noexcept
{
	return (lhs.m_state == Tri_bool::State::Null || rhs.m_state == Tri_bool::State::Null)
		|| (bool{ lhs } == bool{ rhs });
}

} // namespace te

#endif
