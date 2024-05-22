#pragma once
#include "pch.h"

struct Vector2
{
	FLOAT X, Y;

	static Vector2 Zero();

	bool operator== (const Vector2&) const;
};

