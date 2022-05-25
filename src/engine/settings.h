#pragma once

#include "math/color.h"

struct DebugSettings {
    struct TriangleOutlines {
        bool show;
        Color color;
    };

    struct TriangleNormals {
        bool show;
        Color color;
        float length;
    };

    struct TriangleSettings {
        TriangleOutlines outlines;
        TriangleNormals normals;
    };

    TriangleSettings triangle;
    TriangleSettings clipped_triangle;
};

struct Settings {
    DebugSettings debug;
};