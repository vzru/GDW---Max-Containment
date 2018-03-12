#pragma once

//#define ON(x,X) case x: case X: Helper::bitOn(input.keys, Input::Keys::Key X)

namespace Type {
	enum class Object {
		MESH, MATERIAL
	};
	enum Texture {
		DIFFUSE, SPECULAR,
		NORMAL, BUMP
	};
	enum class Sound {
		OGG, MP3, WAV
	};
	enum class Light {
		POINT, DIRECTIONAL, SPOT
	};
	enum class Rotate {
		XYZ, YXZ, XZY,
		ZYX, ZXY, YZX
	};
}
const struct Consts {
	const struct Direction {
		const glm::vec3 up = { 0.f, 1.f, 0.f };
		const glm::vec3 down = { 0.f, -1.f, 0.f };
		const glm::vec3 left = { -1.f, 0.f, 0.f };
		const glm::vec3 right = { 1.f, 0.f, 0.f };
		const glm::vec3 forward = { 0.f, 0.f, 1.f };
		const glm::vec3 backward = { 0.f, 0.f, -1.f };
	} const direction;
	const struct Color {
		const glm::vec3 black = { 0.f, 0.f, 0.f };
		const glm::vec3 white = { 1.f, 1.f, 1.f };
		const glm::vec3 red = { 1.f, 0.f, 0.f };
		const glm::vec3 blue = { 0.f, 1.f, 0.f };
		const glm::vec3 green = { 0.f, 0.f, 1.f };
		const glm::vec3 yellow = { 1.f, 1.f, 0.f };
		const glm::vec3 cyan = { 0.f, 1.f, 1.f };
		const glm::vec3 magenta = { 1.f, 0.f, 1.f };
	} const color;
} const consts;

class Helper {
public:
	static bool inside(glm::vec2 pos, glm::vec4 box, glm::vec2 scale = glm::vec2(1.f)) {
		return pos.x > box.x * scale.x && pos.x < box.y * scale.x && pos.y > box.z * scale.y && pos.y < box.w * scale.y;
	}
	static float getAngle(glm::vec2 diff) {
		return glm::degrees(atan2f(diff.y, -diff.x));
	}
	static float getAngle(glm::vec3 diff) {
		return glm::degrees(atan2f(-diff.z, diff.x));
	}
	static bool incLife(float& currLife, const float& maxLife, float& incLife) {
		// increase curr by inc up to max
		// decrase inc by how much curr increased
		float diffLife = maxLife - currLife;	// diff of curr and max
		if (incLife > diffLife) {				// if inc is more than diff
			currLife = maxLife;					// increase curr to max
			incLife -= diffLife;				// decrease inc by diff
			return !incLife;					// is inc empty?
		}
		currLife += incLife;					// increase curr by inc
		incLife = 0.f;							// clear inc
		return true;							// inc is empty
	}
	template<class Bit>
	static void bitOn(unsigned int& bits, Bit index) {
		bits |= (1 << index);
	}
	template<class Bit>
	static void bitOff(unsigned int& bits, Bit index) {
		bits &= ~(1 << index);
	}
	template<class Bit>
	static bool bitState(unsigned int& bits, Bit index) {
		return bits & (1 << index);
	}
};