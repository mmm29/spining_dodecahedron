#pragma once

#include <memory>

#include "math/vector.h"
#include "math/matrix.h"

class Object {
public:
    Object();

    void SetWorldPosition(const Vector3 &position);

    Vector3 GetWorldPosition() const;

    void Move(const Vector3 &offset);

    void SetRelativePosition(const Vector3 &position);

    Vector3 GetRelativePosition() const;

    void SetRotationAngles(const Vector2 &rotation_angles);

    Vector2 GetRotationAngles() const;

    Vector3 GetDirectionForward() const;

    Matrix4 GetModelMatrix() const;

public:
    void AttachTo(const std::shared_ptr<Object> &object);

    void Detach();

    bool IsAttached() const;

private:
    std::shared_ptr<Object> attached_to_;

private:
    void UpdateRotationMatrix();

protected:
    Vector3 position_;

    Vector2 rotation_angles_;

    Matrix4 rotation_matrix_;

    Vector3 direction_;
};