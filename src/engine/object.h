#pragma once

#include "math/vector.h"
#include "math/matrix.h"

class Object {
public:
    Object();

    void SetPosition(const Vector3 &position);

    const Vector3 &GetPosition() const;

    void SetRotationAngles(const Vector2 &rotation_angles);

    const Vector2 &GetRotationAngles() const;

    Vector3 GetDirectionForward() const;

private:
    void UpdateTransformationMatrix();

protected:
    Vector3 position_;

    Vector2 rotation_angles_;

    Matrix4 transformation_matrix_;

    Vector3 direction_;
};