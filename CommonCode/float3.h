#pragma once
#include <cmath>

struct float3
{
    float3(float _x, float _y, float _z):
        x{_x},
        y{_y},
        z{_z}{}

    float x, y, z;

    float3 operator-(const float3& other) const
    {
        return float3{ x - other.x, y - other.y, z - other.z };
    }

    bool operator==(const float3& other) const
    {
        constexpr float epsilon = 0.00001f;
        float dx = std::abs(x - other.x);
        float dy = std::abs(y - other.y);
        float dz = std::abs(z - other.z);
        return dx < epsilon && dy < epsilon && dz < epsilon;
    }
};