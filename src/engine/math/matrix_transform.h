#pragma once

#include "matrix.h"
#include "vector.h"

namespace matrix {

    Matrix4 Translate(const Vector3 &translate);

    Matrix4 RotateAroundX(float angle);

    Matrix4 RotateAroundY(float angle);

    Matrix4 RotateAroundZ(float angle);

    Matrix4 Scale(const Vector3 &scale);

}