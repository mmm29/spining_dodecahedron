#pragma once

#include "vector.h"
#include "matrix.h"

class Object {
public:
    Object();

    void SetPosition(const Vector3 &position);

    const Vector3 &GetPosition() const;

    void SetRotationAngles(const Vector2 &rotation_angles);

    const Vector2 &GetRotationAngles() const;

    Vector3 GetDirection() const;

private:
    void UpdateTransformationMatrix();

protected:
    Vector3 position_;

    Vector2 rotation_angles_;

    Matrix4 transformation_matrix_;

    Vector3 direction_;
};