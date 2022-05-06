#pragma once

#include "matrix.h"
#include "vector.h"

namespace matrix {

    Matrix4 Translate(const Vector3 &translate);

    Matrix4 RotateX(float angle);

    Matrix4 RotateY(float angle);

    Matrix4 RotateZ(float angle);

    Matrix4 Scale(const Vector3 &scale);

}