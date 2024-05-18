#pragma once
#include "pch.h"

struct Vector3
{
	FLOAT X, Y, Z;

	static Vector3 Zero();
	bool operator==(const Vector3&);
};

