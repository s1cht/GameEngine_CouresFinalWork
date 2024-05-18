#include "pch.h"
#include "Vector3.h"

Vector3 Vector3::Zero()
{
    return Vector3( 0, 0, 0 );
}

bool Vector3::operator==(const Vector3& vec)
{
    return vec.X == this->X && vec.Y == this->Y && vec.Z == this->Z;
}
