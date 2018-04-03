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
#define MAX_NUM_CONTROLLERS 4
	enum State {
		Off, Down, Hold, Up
	};
	enum Button	{
		DPadUp, DPadDown, DPadLeft, DPadRight,
		Start,	Select,	L3,	R3,
		LB,	RB,	A,	B,	X,	Y,
		Size
	};
	const int button[] = {
		XINPUT_GAMEPAD_DPAD_UP, XINPUT_GAMEPAD_DPAD_DOWN, XINPUT_GAMEPAD_DPAD_LEFT, XINPUT_GAMEPAD_DPAD_RIGHT,
		XINPUT_GAMEPAD_START, XINPUT_GAMEPAD_BACK, XINPUT_GAMEPAD_LEFT_THUMB, XINPUT_GAMEPAD_RIGHT_THUMB,
		XINPUT_GAMEPAD_LEFT_SHOULDER, XINPUT_GAMEPAD_RIGHT_SHOULDER,
		XINPUT_GAMEPAD_A, XINPUT_GAMEPAD_B, XINPUT_GAMEPAD_X, XINPUT_GAMEPAD_Y
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
		void(*callback)(unsigned short index, Input::Button button, Input::State state);
		void(*special)(unsigned short index, Input::Triggers _triggers, Input::Sticks _sticks);
	private:

		XINPUT_STATE state;

		int packetNumber[MAX_NUM_CONTROLLERS];
		bool connected[MAX_NUM_CONTROLLERS];

		State buttonStates[MAX_NUM_CONTROLLERS][Button::Size];
		Triggers triggers[MAX_NUM_CONTROLLERS];
		Sticks sticks[MAX_NUM_CONTROLLERS];
	};
}