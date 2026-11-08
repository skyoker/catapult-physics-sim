#pragma once
#include <cmath>
#include <vector>
#include "utl/utils.hpp"
#include "utl/constants.hpp"


inline vec3 universal_gravity_3d(float m1, float m2, vec3 p1, vec3 p2, float G) {
    vec3 r_vec = p2 - p1;
    long double distance_sq = r_vec.x * r_vec.x + r_vec.y * r_vec.y + r_vec.z * r_vec.z;
    if (distance_sq < 1e-10L) return {0.0L, 0.0L, 0.0L};
    long double distance = sqrtl(distance_sq);
    long double force_mag = (long double)G * m1 * m2 / distance_sq;
    vec3 F = {
        (r_vec.x / distance) * force_mag,
        (r_vec.y / distance) * force_mag,
        (r_vec.z / distance) * force_mag
    };
    return F;
}




 