#include <cmath>
#include <cassert>

#include "math/graphics_utils.h"
#include "constants.h"

Matrix4 CreateProjectionMatrix(float aspect_ratio, float fov, float near_z, float far_z) {
    const float half_fov_tan = std::tan(fov / 2);

    Matrix4 res;
    res.SetZero();

    res[0][0] = 1 / (aspect_ratio * half_fov_tan);
    res[1][1] = 1 / half_fov_tan;
    res[2][3] = (far_z * near_z) / (near_z - far_z);


    if (kHandedness == Handedness::kLeftHanded) {
        res[2][2] = far_z / (far_z - near_z);
        res[3][2] = 1;
    } else {
        res[2][2] = far_z / (near_z - far_z);
        res[3][2] = -1;
    }

    return res;
}

Matrix4 CreateViewMatrix(const Vector3 &position, const Vector3 &right, const Vector3 &up, const Vector3 &forward) {
    assert(right.IsNormalized());
    assert(up.IsNormalized());
    assert(forward.IsNormalized());

    Matrix4 res;
    res.SetZero();

    res.SetRow(0, right);
    res.SetRow(1, up);

    if (kHandedness == Handedness::kLeftHanded)
        res.SetRow(2, forward);
    else
        res.SetRow(2, -forward);

    res.SetColumn(3, Vector3(-position.Dot(right), -position.Dot(up), -position.Dot(forward)));
    res.at(3, 3) = 1;

    return res;
}

Matrix4 CreateScreenSpaceMatrix(const Vector2 &size) {
    const float half_width = size[0] / 2;
    const float half_height = size[1] / 2;

    Matrix4 res;
    res.SetIdentity();

    res[0][0] = res[0][3] = half_width;
    res[1][1] = -half_height;
    res[1][3] = half_height;

    return res;
}