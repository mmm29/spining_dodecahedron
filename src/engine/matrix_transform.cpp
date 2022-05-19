#include "math/matrix_transform.h"

Matrix4 matrix::Translate(const Vector3 &translate) {
    return Matrix4{
            {1, 0, 0, translate[0]},
            {0, 1, 0, translate[1]},
            {0, 0, 1, translate[2]},
            {0, 0, 0, 1}
    };
}

Matrix4 matrix::RotateAroundX(float angle) {
    const float s = std::sin(angle);
    const float c = std::cos(angle);

    return Matrix4{
            {1, 0, 0,  0},
            {0, c, -s, 0},
            {0, s, c,  0},
            {0, 0, 0,  1}
    };
}

Matrix4 matrix::RotateAroundY(float angle) {
    const float s = std::sin(angle);
    const float c = std::cos(angle);

    return Matrix4{
            {c,  0, s, 0},
            {0,  1, 0, 0},
            {-s, 0, c, 0},
            {0,  0, 0, 1}
    };
}

Matrix4 matrix::RotateAroundZ(float angle) {
    const float s = std::sin(angle);
    const float c = std::cos(angle);

    return Matrix4{
            {c, -s, 0, 0},
            {s, c,  0, 0},
            {0, 0,  1, 0},
            {0, 0,  0, 1}
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