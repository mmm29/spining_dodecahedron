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
    mutable Vector3 position_; // TODO: remove mutable

    mutable Vector2 rotation_angles_; // TODO: remove mutable

    mutable Matrix4 rotation_matrix_; // TODO: remove mutable

    Vector3 direction_;
};