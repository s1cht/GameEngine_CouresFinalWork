#include "pch.h"
#include "Vector2.h"

Vector2 Vector2::Zero()
{
    return Vector2{ 0, 0 };
}

bool Vector2::operator==(const Vector2& vec) const
{
    return (vec.X == this->X && vec.Y == this->Y);
}
