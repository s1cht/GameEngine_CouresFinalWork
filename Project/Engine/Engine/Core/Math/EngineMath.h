#pragma once
#include "pch.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Color4.h"

const FLOAT DEGREE_IN_RAD = 0.0174532925f;

namespace math {
	inline FLOAT ToRadians(FLOAT degrees)
	{
		return degrees * DEGREE_IN_RAD;
	}
};
