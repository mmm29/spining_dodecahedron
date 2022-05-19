#pragma once

#include "matrix.h"
#include "vector.h"

Matrix4 CreateProjectionMatrix(float aspect_ratio, float fov, float near_z, float far_z);

Matrix4 CreateViewMatrix(const Vector3 &position, const Vector3 &right, const Vector3 &up, const Vector3 &forward);

Matrix4 CreateScreenSpaceMatrix(const Vector2 &size);