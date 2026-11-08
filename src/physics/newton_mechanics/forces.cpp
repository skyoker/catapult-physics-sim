#include <vector>
#include <cmath>
#include <utl/utils.hpp>

vec2 net_force(std::vector<vec2> forces) {
    vec2 net_force = {0.0f, 0.0f};
    for (vec2 force : forces) {
        net_force.x += force.x;
        net_force.y += force.y;
    }

    return net_force;
}

inline bool sphere_collision(
    vec3 p1, vec3 p2,
    vec3& v1, vec3& v2,
    float m1, float m2,
    float r1, float r2,
    float e
) {
    vec3 n = p1 - p2;
    long double dist = sqrtl(n.x*n.x + n.y*n.y + n.z*n.z);
    if (dist >= r1 + r2)     return false;
    if (dist < 1e-10L)       return false;
    n = {n.x/dist, n.y/dist, n.z/dist};

    long double v_rel_n = (v1.x-v2.x)*n.x + (v1.y-v2.y)*n.y + (v1.z-v2.z)*n.z;
    if (v_rel_n > 0) return false;

    long double j = -(1.0L + e) * v_rel_n / (1.0L/m1 + 1.0L/m2);

    v1.x += n.x * j / m1; v1.y += n.y * j / m1; v1.z += n.z * j / m1;
    v2.x -= n.x * j / m2; v2.y -= n.y * j / m2; v2.z -= n.z * j / m2;

    return true;
}
