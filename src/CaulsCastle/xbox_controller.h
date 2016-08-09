#ifndef TE_XBOX_CONTROLLER_H
#define TE_XBOX_CONTROLLER_H

namespace te
{
	enum Xbox_button {
		A_button = 10,
		B_button = 11,
		X_button = 12,
		Y_button = 13,
		LB_button = 8,
		RB_button = 9,
		Back_button = 5,
		Start_button = 4,
		L3_button = 6,
		R3_button = 7,
		D_up_button = 0,
		D_down_button = 1,
		D_left_button = 2,
		D_right_button = 3
	};

	enum Xbox_axis {
		X_axis = 0,
		Y_axis = 1,
		U_axis = 2,
		V_axis = 3,
		LT_axis = 4,
		RT_axis = 5
	};
}

#endif
