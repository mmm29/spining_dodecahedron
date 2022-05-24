#include "vector.h"

Vector<3> Vector<2>::AsVec3(float z) const {
    return Vector<3>(x, y, z);
}

Vector<4> Vector<2>::AsVec4(float z, float w) const {
    return Vector<4>(x, y, z, w);
}

Vector<2> Vector<3>::AsVec2() const {
    return Vector<2>(x, y);
}

Vector<4> Vector<3>::AsVec4(float w) const {
    return Vector<4>(x, y, z, w);
}

Vector<2> Vector<4>::AsVec2() const {
    return Vector<2>(x, y);
}

Vector<3> Vector<4>::AsVec3() const {
    return Vector<3>(x, y, z);
}