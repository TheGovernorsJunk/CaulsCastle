#ifndef TE_INPUT_HPP
#define TE_INPUT_HPP

namespace te
{
	enum InputSwitch
	{
		LIGHT_ATTACK = 1,
		PARRY = 2,
	};

	struct Input
	{
		int switches;
		float lock;
		float heavyAttack;
		float x;
		float y;
	};

	enum class XBoxInput
	{
		A = 0,
		B = 1,
		X = 2,
		Y = 3,
		LB = 4,
		RB = 5,
		Back = 6,
		Start = 7,
		L3 = 8,
		R3 = 9,

		LT = 100,
		RT = 200
	};

	template <typename ButtonType, typename AxisType>
	struct Keymap
	{
		ButtonType lightAttack;
		ButtonType parry;

		AxisType lock;
		AxisType heavyAttack;
		AxisType X;
		AxisType Y;
	};
}

#endif
