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
		OGG, MP3,
		WAV
	};
	enum class Light {
		POINT, DIRECTIONAL, SPOT
	};
	enum class Rotate {
		XYZ, YXZ, XZY,
		ZYX, ZXY, YZX
	};
}