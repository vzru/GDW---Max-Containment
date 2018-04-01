#pragma once
#include "FMOD/fmod.hpp"
#include <vector>

namespace Type {
	enum class Extension {
		OBJ, HTR, XML,
		PNG, BMP,
		OGG, MP3, WAV,
	};
	enum class Object {
		MESH, MATERIAL
	};
	enum Texture {
		DIFFUSE, SPECULAR,
		NORMAL, BUMP
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
		const glm::vec3 up =		{ 0.f, 1.f, 0.f };
		const glm::vec3 down =		{ 0.f, -1.f, 0.f };
		const glm::vec3 left =		{ -1.f, 0.f, 0.f };
		const glm::vec3 right =		{ 1.f, 0.f, 0.f };
		const glm::vec3 forward =	{ 0.f, 0.f, 1.f };
		const glm::vec3 backward =	{ 0.f, 0.f, -1.f };
	} const direction;
	const struct Color {
		const glm::vec3 black =		{ 0.f, 0.f, 0.f };
		const glm::vec3 white =		{ 1.f, 1.f, 1.f };
		const glm::vec3 red =		{ 1.f, 0.f, 0.f };
		const glm::vec3 blue =		{ 0.f, 1.f, 0.f };
		const glm::vec3 green =		{ 0.f, 0.f, 1.f };
		const glm::vec3 yellow =	{ 1.f, 1.f, 0.f };
		const glm::vec3 cyan =		{ 0.f, 1.f, 1.f };
		const glm::vec3 magenta =	{ 1.f, 0.f, 1.f };
	} const color;
} const consts;

struct Helper {
	static bool inside(glm::vec2 pos, glm::vec4 box, glm::vec2 scale = glm::vec2(1.f)) {
		return pos.x > box.x * scale.x && pos.x < box.y * scale.x && pos.y > box.z * scale.y && pos.y < box.w * scale.y;
	}
	template<class T>
	static glm::vec4 castBox(T box) {
		return { box.min.x, box.max.x, box.min.y, box.max.y };
	}
	template<class T>
	static void bitOn(unsigned int &bits, T bit) {
		bits &= ~(1 << (unsigned int)bit);
	}
	template<class T>
	static void bitOff(unsigned int &bits, T bit) {
		bits |= (1 << (unsigned int)bit);
	}
	template<class T>
	static bool bitState(unsigned int &bits, T bit) {
		return bits & (1 << (unsigned int)bit);
	}
	template<class T>
	static bool bitState(unsigned int &bits, T onBit, T offBit) {
		return bits & (1 << (unsigned int)onBit) && bits & ~(1 << (unsigned int)offBit);
	}
	static float getAngle(glm::vec3 vec) {
		return atan2(-vec.z, vec.x);
	}
	static glm::vec2 getDist(float angle, float length) {
		return { sin(angle) * length, cos(angle) * length };
	}
	template<class T>
	static glm::vec2 getDist(float angle, T length) {
		return getDist(angle, glm::length(length));
	}
	static glm::vec3 castVec(FMOD_VECTOR vec) {
		return { vec.x, vec.y, vec.z };
	}
	static FMOD_VECTOR castVec(glm::vec3 vec) {
		return { vec.x, vec.y, vec.z };
	}
	template<class T>
	static int convArray(int index, T tuple[3]) {
		int total = 0;
		for (int i = 0; i < 3; i++) {
			total += tuple[0].positions.size();
			if (index < total)
				return i;
		}
	}
	template<class T>
	static int offsetIndex(int type, T tuple[3]) {
		int result = 0;
		for (int i = 0; i < type - 1; i++)
			result -= tuple[i].positions.size();
		return result;
	}
};