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

Vector3 Vector3::operator* (const FLOAT& fl)
{
    return Vector3(this->X * fl, this->Y * fl, this->Z * fl);
}

Vector3 Vector3::operator+ (const Vector3& vec)
{
    return Vector3(this->X + vec.X, this->Y + vec.Y, this->Z + vec.Z);
}

