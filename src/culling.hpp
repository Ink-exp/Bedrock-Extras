#ifndef CULLING_HPP
#define CULLING_HPP

// 3D vector coordinates for entities and camera position
struct Vector3 {
    float x;
    float y;
    float z;
};

// 3D Bounding box structure for frustum and distance checks
struct AABB {
    Vector3 min;
    Vector3 max;
};

#endif // CULLING_HPP
