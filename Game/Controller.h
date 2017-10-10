#pragma once

#include <utility>

#include <Xinput.h>
#include <glm/vec2.hpp>
#include <glm/glm.hpp>

#define min(a,b) (((a) < (b)) ? (a) : (b))

namespace Input
{
	using Triggers = std::pair<float, float>;
	using Sticks = std::pair<glm::vec2, glm::vec2>;

	enum Button
	{
		A = 0x0, B = 0x1, X = 0x2, Y = 0x3,
		RB = 0x4, LB = 0x5, R3 = 0x6, L3 = 0x7,
		Start = 0x8, Select = 0x9,
		DPadLeft = 0xA, DPadRight = 0xB, DPadUp = 0xC, DPadDown = 0xD
	};

	class XBoxInput
	{
	public:
		// These are basically just to set the vibration to zero
		XBoxInput();
		~XBoxInput();

		// Call this function ONCE in an update
		bool Update(unsigned short _numControllers = 1);

		// Call this function to get the sticks. The function errors(WIP) if there is no controller for this index
		Sticks GetSticks(unsigned short index);
		// This function returns false if there is no controller plugged in to this index
		bool GetConnected(unsigned short index);
		// Call this function to get the Button. The function errors(WIP) if there is no controller, returns false if the button isn't pressed and returns true if the button is
		bool GetButton(unsigned short index, Button _buttons);
		// Call this function to get the triggers on the controller. It errors(WIP) if there is no controller connected
		Triggers GetTriggers(unsigned short index);
		// Call this function to set the vibration. It errors(WIP) if there is no controller connected
		bool SetVibration(unsigned short index, Triggers _powers);
		
		// these are the callbacks
		void setCallbacks(void (*_callback)(unsigned short index, Input::Button button), void(*_special)(unsigned short index, Input::Triggers triggers, Input::Sticks sticks));
		void (*callback)(unsigned short index, Input::Button button);
		void (*special)(unsigned short index, Input::Triggers triggers, Input::Sticks sticks);
	private:

		XINPUT_STATE m_state;

		int m_packetNumber[4];
		bool m_connected[4];


		bool m_buttonStates[4][14];
		Triggers m_triggers[4];
		Sticks m_sticks[4];
	};
}