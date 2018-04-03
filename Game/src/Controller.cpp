#include <memory>

#include <Windows.h>

#include "Controller.h"

namespace Input
{
	XBox::XBox() {
		for (unsigned int i = 0; i < XUSER_MAX_COUNT; i++) {
			XINPUT_VIBRATION vibration;
			memset(&vibration, 0, sizeof(XINPUT_VIBRATION));
			vibration.wLeftMotorSpeed = 0;
			vibration.wRightMotorSpeed = 0;
			XInputSetState(i, &vibration);
		}
	}
	XBox::~XBox() {
		for (unsigned int i = 0; i < XUSER_MAX_COUNT; i++) {
			XINPUT_VIBRATION vibration;
			memset(&vibration, 0, sizeof(XINPUT_VIBRATION));
			vibration.wLeftMotorSpeed = 0;
			vibration.wRightMotorSpeed = 0;
			XInputSetState(i, &vibration);
		}
	}

	bool XBox::update(unsigned short _numControllers) {
		// download packets
		unsigned int result;
		for (unsigned int i = 0; i < min(min(_numControllers, 4), XUSER_MAX_COUNT); i++) {
			// Set the state to zero
			memset(&state, 0, sizeof(XINPUT_STATE));

			// Get the first state (used mainly to determine if the controller is connected or not)
			result = XInputGetState(i, &state);

			// If the result is null, then the controller IS connected
			if (result == NULL) {
				connected[i] = true;
				// If the number of packets has changed, we know something has happened in the controller
				// and we can get the state here. A simple speed optimization
				if (state.dwPacketNumber != packetNumber[i]) {
					// Reset everything to Zero
					std::fill(std::begin(buttonStates[i]), std::end(buttonStates[i]), false);
					packetNumber[i] = state.dwPacketNumber;

#pragma region Update Sticks
					// UPDATE THE STICKS
					sticks[i] = { { state.Gamepad.sThumbLX, state.Gamepad.sThumbLY },
								{ state.Gamepad.sThumbRX, state.Gamepad.sThumbRY } };

					if (glm::length(sticks[i].first) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
						sticks[i].first /= 32767.0f;
					else sticks[i].first = { 0.0f, 0.0f };

					if (glm::length(sticks[i].second) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
						sticks[i].second /= 32767.0f;
					else sticks[i].second = { 0.0f, 0.0f };
#pragma endregion

#pragma region Update Triggers
					// UPDATE THE TRIGGERS
					if (state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
						triggers[i].first = state.Gamepad.bLeftTrigger / 255.0f;
					else triggers[i].first = 0.0f;
					if (state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
						triggers[i].second = state.Gamepad.bRightTrigger / 255.0f;
					else triggers[i].second = 0.0f;
#pragma endregion

#pragma region Update Buttons
					// UPDATE ALL OF THE BUTTONS OF THE GAMEPAD
					for (int j = 0; j < Button::Size; j++)
						if (state.Gamepad.wButtons & button[j])
							if (buttonStates[i][j])						buttonStates[i][j] = State::Hold;
							else										buttonStates[i][j] = State::Down;
						else if (buttonStates[i][j] == State::Hold)		buttonStates[i][j] = State::Up;
						else if (buttonStates[i][j] == State::Up)		buttonStates[i][j] = State::Off;
#pragma endregion
				}
				for (int b = 0; b < 14; b++)
					if (buttonStates[i][b])
						(*callback)(i, (Input::Button)b, buttonStates[i][b]);
				(*special)(i, triggers[i], sticks[i]);
			} else connected[i] = false;
		}

		return true;
	}
	// getters
	bool XBox::getConnected(unsigned short _index) {
		if (!connected[_index]) return false;
		return true;
	}
	bool XBox::getButton(unsigned short _index, Button _buttons) {
		if (!connected[_index]) { throw "Controller not connected"; } // WIP improve error message
		return buttonStates[_index][_buttons];
	}
	Sticks XBox::getSticks(unsigned short _index) {
		if (!connected[_index]) { throw "Controller not connected"; } // WIP improve error message
		return sticks[_index];
	}
	Triggers XBox::getTriggers(unsigned short _index) {
		if (!connected[_index]) { throw "Controller not connected"; } // WIP improve error message
		return triggers[_index];
	}
	// setters
	bool XBox::setVibration(unsigned short _index, Triggers _powers) {
		if (!connected[_index]) { throw "Controller not connected"; } // WIP improve error message
		XINPUT_VIBRATION vibration;
		memset(&vibration, 0, sizeof(XINPUT_VIBRATION));
		vibration.wLeftMotorSpeed = _powers.first * 65535;
		vibration.wRightMotorSpeed = _powers.second * 65535;
		XInputSetState(_index, &vibration);
		return true;
	}
}