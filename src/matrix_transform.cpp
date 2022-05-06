#include "matrix_transform.h"

Matrix4 matrix::Translate(const Vector3 &translate) {
    return Matrix4{
            {1, 0, 0, translate[0]},
            {0, 1, 0, translate[1]},
            {0, 0, 1, translate[2]},
            {0, 0, 0, 1}
    }
}

Matrix4 matrix::RotateX(float angle) {
    const float sin = sinf(angle);
    const float cos = cosf(angle);

    return Matrix4{
            {1, 0,   0,    0},
            {0, cos, -sin, 0},
            {0, sin, cos,  0},
            {0, 0,   0,    1}
    };
}

Matrix4 matrix::RotateY(float angle) {
    const float sin = sinf(angle);
    const float cos = cosf(angle);

    return Matrix4{
            {cos,  0, sin, 0},
            {0,    1, 0,   0},
            {-sin, 0, cos, 0},
            {0,    0, 0,   1}
    };
}

Matrix4 matrix::RotateZ(float angle) {
    const float sin = sinf(angle);
    const float cos = cosf(angle);

    return Matrix4{
            {cos, -sin, 0, 0},
            {sin, cos,  0, 0},
            {0,   0,    1, 0},
            {0,   0,    0, 1}
    };
}

Matrix4 matrix::Scale(const Vector3 &scale) {
    return Matrix4{
            {scale[0], 0,        0,        0},
            {0,        scale[1], 0,        0},
            {0,        0,        scale[2], 0},
            {0,        0,        0,        1}
    };
}