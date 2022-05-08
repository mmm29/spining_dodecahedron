#include <cmath>
#include <cassert>

#include "graphics_utils.h"

Matrix4 CreateProjectionMatrix(float aspect_ratio, float fov, float near_z, float far_z) {
    const float fov_tan = std::tan(fov / 2);
    const float far_near_distance = far_z - near_z;
    const float far_over_distance = far_z / far_near_distance;
    const float far_near_over_distance = near_z * far_over_distance;

    return Matrix4({
                           {1 / (aspect_ratio * fov_tan), 0,           0,                  0},
                           {0,                            1 / fov_tan, 0,                  0},
                           {0,                            0,           -far_over_distance, -far_near_over_distance},
                           {0,                            0,           -1,                 0}
                   });
}

Matrix4 CreateViewMatrix(const Vector3 &position, const Vector3 &right, const Vector3 &up, const Vector3 &forward) {
    assert(right.IsNormalized());
    assert(up.IsNormalized());
    assert(forward.IsNormalized());

    return Matrix4{
            {right[0],   right[1],   right[2],   -position.Dot(right)},
            {up[0],      up[1],      up[2],      -position.Dot(up)},
            {forward[0], forward[1], forward[2], -position.Dot(forward)},
            {0,          0,          0,          1},
    };
}

Matrix4 CreateScreenSpaceMatrix(const Vector2 &size) {
    const float half_width = size[0] / 2;
    const float half_height = size[1] / 2;

    return Matrix4({
                           {half_width, 0,            0, half_width},
                           {0,          -half_height, 0, half_height},
                           {0,          0,            1, 0},
                           {0,          0,            0, 1}
                   });
}