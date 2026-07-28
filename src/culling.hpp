#pragma once

#include <cmath>

// 3D Vector for Entity, Player, and Camera positions
struct Vector3 {
    float x;
    float y;
    float z;

    // Squared distance function (no square root = better performance on mobile)
    float distance_squared_to(const Vector3& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        float dz = z - other.z;
        return (dx * dx) + (dy * dy) + (dz * dz);
    }
};

// Simple configuration structure for culling tweaks
struct CullingConfig {
    float max_distance = 48.0f; // Block distance
    bool enabled = true;
};
