#include <memory>

#include <Windows.h>

#include "controller.h"

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

	bool XBox::Update(unsigned short _numControllers) {
		// download packets
		unsigned int result;
		for (unsigned int i = 0; i < min(min(_numControllers, 4), XUSER_MAX_COUNT); i++) {
			// Set the state to zero
			memset(&m_state, 0, sizeof(XINPUT_STATE));

			// Get the first state (used mainly to determine if the controller is connected or not)
			result = XInputGetState(i, &m_state);

			// If the result is null, then the controller IS connected
			if (result == NULL) {
				m_connected[i] = true;
				// If the number of packets has changed, we know something has happened in the controller
				// and we can get the state here. A simple speed optimization
				if (m_state.dwPacketNumber != m_packetNumber[i]) {
					// Reset everything to Zero
					std::fill(std::begin(m_buttonStates[i]), std::end(m_buttonStates[i]), false);
					m_packetNumber[i] = m_state.dwPacketNumber;

#pragma region Update Sticks
					// UPDATE THE STICKS
					m_sticks[i] = { { m_state.Gamepad.sThumbLX, m_state.Gamepad.sThumbLY },
									{ m_state.Gamepad.sThumbRX, m_state.Gamepad.sThumbRY } };

					if (glm::length(m_sticks[i].first) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
						m_sticks[i].first /= 32767.0f;
					else m_sticks[i].first = { 0.0f, 0.0f };
					
					if (glm::length(m_sticks[i].second) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
						m_sticks[i].second /= 32767.0f;
					else m_sticks[i].second = { 0.0f, 0.0f };
#pragma endregion

#pragma region Update Triggers
					// UPDATE THE TRIGGERS
					if (m_state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
						m_triggers[i].first = m_state.Gamepad.bLeftTrigger / 255.0f;
					else
						m_triggers[i].first = 0.0f;
					if (m_state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
						m_triggers[i].second = m_state.Gamepad.bRightTrigger / 255.0f;
					else
						m_triggers[i].second = 0.0f;
#pragma endregion

#pragma region Update Buttons
					// UPDATE ALL OF THE BUTTONS OF THE GAMEPAD
					if (m_state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
						m_buttonStates[i][Button::A] = true;
					if (m_state.Gamepad.wButtons & XINPUT_GAMEPAD_B)
						m_buttonStates[i][Button::B] = true;
					if (m_state.Gamepad.wButtons & XINPUT_GAMEPAD_X)
						m_buttonStates[i][Button::X] = true;
					if (m_state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
						m_buttonStates[i][Button::Y] = true;
					if (m_state.Gamepad.wButtons & XINPUT_GAMEPAD_START)
						m_buttonStates[i][Button::Start] = true;
					if (m_state.Gamepad.wButtons &  XINPUT_GAMEPAD_BACK)
						m_buttonStates[i][Button::Select] = true;
					if (m_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
						m_buttonStates[i][Button::LB] = true;
					if (m_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
						m_buttonStates[i][Button::RB] = true;
					if (m_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
						m_buttonStates[i][Button::L3] = true;
					if (m_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
						m_buttonStates[i][Button::R3] = true;
					if (m_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
						m_buttonStates[i][Button::DPadDown] = true;
					if (m_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
						m_buttonStates[i][Button::DPadUp] = true;
					if (m_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
						m_buttonStates[i][Button::DPadRight] = true;
					if (m_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
						m_buttonStates[i][Button::DPadLeft] = true;
#pragma endregion
				}
				for (int b = 0; b < 14; b++)
					if (GetButton(i, (Input::Button)b))
						(*callback)(i, (Input::Button)b);
				(*special)(i, GetTriggers(i), GetSticks(i));
			} else m_connected[i] = false;
		}

		return true;
	}
	// getters
	bool XBox::GetConnected(unsigned short _index) {
		if (!m_connected[_index]) return false;
		return true;
	}
	bool XBox::GetButton(unsigned short _index, Button _buttons) {
		if (!m_connected[_index]) { throw "Controller not connected"; } // WIP improve error message
		return m_buttonStates[_index][_buttons];
	}
	Sticks XBox::GetSticks(unsigned short _index) {
		if (!m_connected[_index]) { throw "Controller not connected"; } // WIP improve error message
		return m_sticks[_index];
	}
	Triggers XBox::GetTriggers(unsigned short _index) {
		if (!m_connected[_index]) { throw "Controller not connected"; } // WIP improve error message
		return m_triggers[_index];
	}
	// setters
	bool XBox::SetVibration(unsigned short _index, Triggers _powers) {
		if (!m_connected[_index]) { throw "Controller not connected"; } // WIP improve error message
		XINPUT_VIBRATION vibration;
		memset(&vibration, 0, sizeof(XINPUT_VIBRATION));
		vibration.wLeftMotorSpeed = _powers.first * 65535;
		vibration.wRightMotorSpeed = _powers.second * 65535;
		XInputSetState(_index, &vibration);
		return true;
	}
	void XBox::setCallbacks(void(*_callback)(unsigned short index, Input::Button button), void(*_special)(unsigned short index, Input::Triggers triggers, Input::Sticks sticks)) {
		callback = _callback; special = _special;
	}
}