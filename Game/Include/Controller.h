#pragma once

#include <utility>

#include <Xinput.h>
#include <glm/vec2.hpp>
#include <glm/glm.hpp>

// for some reason this gets hidden when including some stuff so rewriting it
#define min(a,b) (((a) < (b)) ? (a) : (b))

namespace Input {
	using Triggers = std::pair<float, float>;
	using Sticks = std::pair<glm::vec2, glm::vec2>;
	enum Button	{
		A = 0x0, B = 0x1, X = 0x2, Y = 0x3,
		RB = 0x4, LB = 0x5, R3 = 0x6, L3 = 0x7,
		Start = 0x8, Select = 0x9,
		DPadLeft = 0xA, DPadRight = 0xB, DPadUp = 0xC, DPadDown = 0xD
	};
	
	class XBox {
	public:
		// These are basically just to set the vibration to zero
		XBox();
		~XBox();

		// Call this function ONCE in an update
		bool update(unsigned short _numControllers = 1);

		// Call this function to get the sticks. The function errors(WIP) if there is no controller for this index
		Sticks getSticks(unsigned short index);
		// This function returns false if there is no controller plugged in to this index
		bool getConnected(unsigned short index);
		// Call this function to get the Button. The function errors(WIP) if there is no controller, returns false if the button isn't pressed and returns true if the button is
		bool getButton(unsigned short index, Button _buttons);
		// Call this function to get the triggers on the controller. It errors(WIP) if there is no controller connected
		Triggers getTriggers(unsigned short index);
		// Call this function to set the vibration. It errors(WIP) if there is no controller connected
		bool setVibration(unsigned short index, Triggers _powers);
		
		// these are the callbacks
		void setCallbacks(
			void (*_callback)(unsigned short index, Input::Button button),
			void (*_special)(unsigned short index, Input::Triggers triggers, Input::Sticks sticks)
		);
		void (*callback)(unsigned short index, Input::Button button);
		void (*special)(unsigned short index, Input::Triggers _triggers, Input::Sticks _sticks);
	private:

		XINPUT_STATE state;

		int packetNumber[4];
		bool connected[4];

		bool buttonStates[4][14];
		Triggers triggers[4];
		Sticks sticks[4];
	};
}