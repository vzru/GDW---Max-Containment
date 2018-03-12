#pragma once

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
		const glm::vec3 red = { 1.f, 0.f, 0.f };
		const glm::vec3 blue = { 0.f, 1.f, 0.f };
		const glm::vec3 green = { 0.f, 0.f, 1.f };
		const glm::vec3 yellow = { 1.f, 1.f, 0.f };
		const glm::vec3 cyan = { 0.f, 1.f, 1.f };
		const glm::vec3 magenta = { 1.f, 0.f, 1.f };
	} const color;
} const consts;