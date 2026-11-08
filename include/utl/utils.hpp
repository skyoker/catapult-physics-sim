#pragma once

#include <vector>
#include <format>
#include <cmath>
#include <format>
#include <random>

#include "glad/gl.h"
#include <GLFW/glfw3.h>

#include "utl/logger.hpp"


// --- MACROS ---

// color rgb
#define NULL_COLOR_RGBA {0.0f, 0.0f, 0.0f, 0.0f}
#define RED_RGBA {1.0f, 0.0f, 0.0f, 0.0f}
#define BLUE_RGBA {0.0f, 1.0f, 0.0f, 0.0f}
#define GREEN_RGBA {0.0f, 0.0f, 1.0f, 0.0f}

// small configs
#define RANDOM_SEED 1
#define FPS 60

// --- GENERAL UTILS ---

// vectors

struct vec2 {float x, y;};
struct vec3 {long double x, y, z;};
struct vec4 {float r, g, b, a;};

// vec3 operators
inline vec3 operator+(vec3 a, vec3 b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
inline vec3 operator-(vec3 a, vec3 b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
inline vec3 operator*(vec3 a, float s) { return {a.x * s, a.y * s, a.z * s}; }
inline vec3 operator*(float s, vec3 a) { return {a.x * s, a.y * s, a.z * s}; }
inline vec3 operator/(vec3 a, float s) { return {a.x / s, a.y / s, a.z / s}; }
inline vec3& operator+=(vec3& a, vec3 b) { a.x += b.x; a.y += b.y; a.z += b.z; return a; }
inline vec3& operator-=(vec3& a, vec3 b) { a.x -= b.x; a.y -= b.y; a.z -= b.z; return a; }

// 3D shape generators
inline std::vector<GLfloat> cube_verts(float size) {
    float h = size / 2.0f;
    return {
        // Front (+Z)
        -h, -h,  h,   h, -h,  h,   h,  h,  h,  -h,  h,  h,
        // Back (-Z)
         h, -h, -h,  -h, -h, -h,  -h,  h, -h,   h,  h, -h,
        // Top (+Y)
        -h,  h,  h,   h,  h,  h,   h,  h, -h,  -h,  h, -h,
        // Bottom (-Y)
        -h, -h, -h,   h, -h, -h,   h, -h,  h,  -h, -h,  h,
        // Right (+X)
         h, -h,  h,   h, -h, -h,   h,  h, -h,   h,  h,  h,
        // Left (-X)
        -h, -h, -h,  -h, -h,  h,  -h,  h,  h,  -h,  h, -h,
    };
}

inline std::vector<float> cube_colors() {
    return {
        // Front (+Z) - Red
        1.0f, 0.2f, 0.2f, 1.0f,  1.0f, 0.2f, 0.2f, 1.0f,
        1.0f, 0.2f, 0.2f, 1.0f,  1.0f, 0.2f, 0.2f, 1.0f,
        // Back (-Z) - Blue
        0.2f, 0.2f, 1.0f, 1.0f,  0.2f, 0.2f, 1.0f, 1.0f,
        0.2f, 0.2f, 1.0f, 1.0f,  0.2f, 0.2f, 1.0f, 1.0f,
        // Top (+Y) - Green
        0.2f, 1.0f, 0.2f, 1.0f,  0.2f, 1.0f, 0.2f, 1.0f,
        0.2f, 1.0f, 0.2f, 1.0f,  0.2f, 1.0f, 0.2f, 1.0f,
        // Bottom (-Y) - Yellow
        1.0f, 1.0f, 0.2f, 1.0f,  1.0f, 1.0f, 0.2f, 1.0f,
        1.0f, 1.0f, 0.2f, 1.0f,  1.0f, 1.0f, 0.2f, 1.0f,
        // Right (+X) - Cyan
        0.2f, 1.0f, 1.0f, 1.0f,  0.2f, 1.0f, 1.0f, 1.0f,
        0.2f, 1.0f, 1.0f, 1.0f,  0.2f, 1.0f, 1.0f, 1.0f,
        // Left (-X) - Magenta
        1.0f, 0.2f, 1.0f, 1.0f,  1.0f, 0.2f, 1.0f, 1.0f,
        1.0f, 0.2f, 1.0f, 1.0f,  1.0f, 0.2f, 1.0f, 1.0f,
    };
}

inline std::vector<int> cube_indices() {
    return {
         0, 1, 2,  0, 2, 3,       // front
         4, 5, 6,  4, 6, 7,       // back
         8, 9,10,  8,10,11,       // top
        12,13,14, 12,14,15,       // bottom
        16,17,18, 16,18,19,       // right
        20,21,22, 20,22,23,       // left
    };
}

inline std::vector<GLfloat> sphere_verts(float radius, int sectors, int stacks) {
    std::vector<GLfloat> verts;
    verts.reserve(3 * (2 + (stacks - 1) * sectors));
    float pi = (float)M_PI;

    verts.push_back(0.0f); verts.push_back(radius); verts.push_back(0.0f);

    for (int stack = 1; stack < stacks; stack++) {
        float phi = pi * (float)stack / (float)stacks;
        float sin_phi = sin(phi);
        float cos_phi = cos(phi);
        for (int sector = 0; sector < sectors; sector++) {
            float theta = 2.0f * pi * (float)sector / (float)sectors;
            verts.push_back(radius * sin_phi * cos(theta));
            verts.push_back(radius * cos_phi);
            verts.push_back(radius * sin_phi * sin(theta));
        }
    }

    verts.push_back(0.0f); verts.push_back(-radius); verts.push_back(0.0f);

    return verts;
}

inline std::vector<int> sphere_indices(int sectors, int stacks) {
    std::vector<int> idx;
    int body_first = 1;
    int body_verts_per_ring = sectors;
    int bottom_idx = body_first + (stacks - 1) * sectors;

    for (int s = 0; s < sectors; s++) {
        int a = body_first + s;
        int b = body_first + (s + 1) % sectors;
        idx.push_back(0); idx.push_back(b); idx.push_back(a);
    }

    for (int ring = 0; ring < stacks - 2; ring++) {
        for (int s = 0; s < sectors; s++) {
            int a = body_first + ring * body_verts_per_ring + s;
            int b = body_first + ring * body_verts_per_ring + (s + 1) % sectors;
            int c = body_first + (ring + 1) * body_verts_per_ring + s;
            int d = body_first + (ring + 1) * body_verts_per_ring + (s + 1) % sectors;
            idx.push_back(a); idx.push_back(c); idx.push_back(b);
            idx.push_back(b); idx.push_back(c); idx.push_back(d);
        }
    }

    int last_ring = body_first + (stacks - 2) * sectors;
    for (int s = 0; s < sectors; s++) {
        int a = last_ring + s;
        int b = last_ring + (s + 1) % sectors;
        idx.push_back(bottom_idx); idx.push_back(b); idx.push_back(a);
    }

    return idx;
}

inline std::vector<GLfloat> axis_verts(float len) {
    return {
        -len, 0.0f, 0.0f,  len, 0.0f, 0.0f,
        0.0f, -len, 0.0f,  0.0f, len, 0.0f,
        0.0f, 0.0f, -len,  0.0f, 0.0f, len,
    };
}

inline std::vector<int> axis_indices() {
    return {0, 1, 2, 3, 4, 5};
}

inline std::vector<float> axis_colors() {
    return {
        1.0f, 0.2f, 0.2f, 1.0f,  1.0f, 0.2f, 0.2f, 1.0f,
        0.2f, 1.0f, 0.2f, 1.0f,  0.2f, 1.0f, 0.2f, 1.0f,
        0.2f, 0.2f, 1.0f, 1.0f,  0.2f, 0.2f, 1.0f, 1.0f,
    };
}

inline std::vector<GLfloat> grid_verts(float half_size, float y, int divisions) {
    std::vector<GLfloat> verts;
    float step = 2.0f * half_size / divisions;
    for (int i = 0; i <= divisions; i++) {
        float z = -half_size + i * step;
        verts.push_back(-half_size); verts.push_back(y); verts.push_back(z);
        verts.push_back(half_size);  verts.push_back(y); verts.push_back(z);
    }
    for (int i = 0; i <= divisions; i++) {
        float x = -half_size + i * step;
        verts.push_back(x); verts.push_back(y); verts.push_back(-half_size);
        verts.push_back(x); verts.push_back(y); verts.push_back(half_size);
    }
    return verts;
}

inline std::vector<int> grid_indices(int vert_count) {
    std::vector<int> idx(vert_count);
    for (int i = 0; i < vert_count; i++) idx[i] = i;
    return idx;
}

// --- NOTE GEOMETRY (annotation shapes: arrows, position markers, polylines) ---
// All of these emit GL_LINES vertex lists in world space, matching axis_verts/grid_verts.

// vec3 helpers used by the note builders
inline long double vec3_len(vec3 v) { return sqrtl(v.x*v.x + v.y*v.y + v.z*v.z); }

inline vec3 vec3_norm(vec3 v) {
    long double l = vec3_len(v);
    if (l < 1e-12L) return {0.0L, 0.0L, 0.0L};
    return {v.x / l, v.y / l, v.z / l};
}

inline vec3 vec3_cross(vec3 a, vec3 b) {
    return {a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x};
}

// Any unit vector perpendicular to v. Picks the axis least aligned with v so the
// cross product never degenerates.
inline vec3 vec3_any_perp(vec3 v) {
    vec3 axis = (fabsl(v.x) < fabsl(v.y) && fabsl(v.x) < fabsl(v.z)) ? vec3{1,0,0}
              : (fabsl(v.y) < fabsl(v.z))                            ? vec3{0,1,0}
                                                                     : vec3{0,0,1};
    return vec3_norm(vec3_cross(v, axis));
}

inline void push_vert(std::vector<GLfloat>& out, vec3 p) {
    out.push_back((GLfloat)p.x); out.push_back((GLfloat)p.y); out.push_back((GLfloat)p.z);
}

// Arrow: a shaft line plus `head_spokes` barbs swept back from the tip, so the head
// reads as an arrowhead from any camera angle. Returns empty for a zero-length vector.
inline std::vector<GLfloat> arrow_verts(vec3 from, vec3 to, float head_frac = 0.2f, int head_spokes = 6) {
    std::vector<GLfloat> verts;
    vec3 d = to - from;
    long double len = vec3_len(d);
    if (len < 1e-9L) return verts;

    vec3 dir = {d.x / len, d.y / len, d.z / len};
    long double head_len = len * (long double)head_frac;
    long double head_rad = head_len * 0.4L;
    vec3 base = to - dir * (float)head_len;

    // shaft
    push_vert(verts, from); push_vert(verts, to);

    // head barbs, from the tip back to a ring around the head base
    vec3 u = vec3_any_perp(dir);
    vec3 w = vec3_cross(dir, u);
    for (int i = 0; i < head_spokes; i++) {
        float theta = 2.0f * (float)M_PI * (float)i / (float)head_spokes;
        vec3 off = u * (float)(head_rad * cosf(theta)) + w * (float)(head_rad * sinf(theta));
        push_vert(verts, to);
        push_vert(verts, base + off);
    }
    return verts;
}

// Position marker: a small 3-axis cross at the point, ringed by three orthogonal
// circles (XY, YZ, XZ) so it reads as a circled point from any direction.
inline std::vector<GLfloat> point_marker_verts(vec3 pos, float radius, int segments = 24) {
    std::vector<GLfloat> verts;
    float cross_r = radius * 0.35f;

    push_vert(verts, {pos.x - cross_r, pos.y, pos.z}); push_vert(verts, {pos.x + cross_r, pos.y, pos.z});
    push_vert(verts, {pos.x, pos.y - cross_r, pos.z}); push_vert(verts, {pos.x, pos.y + cross_r, pos.z});
    push_vert(verts, {pos.x, pos.y, pos.z - cross_r}); push_vert(verts, {pos.x, pos.y, pos.z + cross_r});

    for (int plane = 0; plane < 3; plane++) {
        for (int i = 0; i < segments; i++) {
            float t0 = 2.0f * (float)M_PI * (float)i / (float)segments;
            float t1 = 2.0f * (float)M_PI * (float)(i + 1) / (float)segments;
            float c0 = radius * cosf(t0), s0 = radius * sinf(t0);
            float c1 = radius * cosf(t1), s1 = radius * sinf(t1);
            vec3 a, b;
            if (plane == 0)      { a = {pos.x + c0, pos.y + s0, pos.z}; b = {pos.x + c1, pos.y + s1, pos.z}; }
            else if (plane == 1) { a = {pos.x, pos.y + c0, pos.z + s0}; b = {pos.x, pos.y + c1, pos.z + s1}; }
            else                 { a = {pos.x + c0, pos.y, pos.z + s0}; b = {pos.x + c1, pos.y, pos.z + s1}; }
            push_vert(verts, a); push_vert(verts, b);
        }
    }
    return verts;
}

// A polyline through the given points, as GL_LINES segment pairs.
inline std::vector<GLfloat> line_strip_verts(const std::vector<vec3>& points) {
    std::vector<GLfloat> verts;
    if (points.size() < 2) return verts;
    verts.reserve((points.size() - 1) * 6);
    for (size_t i = 0; i + 1 < points.size(); i++) {
        push_vert(verts, points[i]);
        push_vert(verts, points[i + 1]);
    }
    return verts;
}

// Sequential indices for a GL_LINES vertex list (one index per vertex).
inline std::vector<int> note_indices(int vert_count) {
    std::vector<int> idx(vert_count);
    for (int i = 0; i < vert_count; i++) idx[i] = i;
    return idx;
}

inline std::vector<float> grid_colors(int vert_count, float r, float g, float b, float a) {
    std::vector<float> colors;
    colors.reserve(vert_count * 4);
    for (int i = 0; i < vert_count; i++) {
        colors.push_back(r); colors.push_back(g);
        colors.push_back(b); colors.push_back(a);
    }
    return colors;
}


// --- RANDOM UTILS ---

inline std::random_device rd;
inline std::mt19937 gen(RANDOM_SEED);

inline int get_random_int(int min, int max) {
    std::uniform_int_distribution<int> int_dist(min, max);

    return int_dist(gen);
}

inline float get_random_float() {
    std::uniform_real_distribution<float> float_dist(0.0f, 1.0f);
    float f = float_dist(gen);

    return f;
}

inline std::vector<float> get_random_point_in_circle(float max_r) {
    float angle = get_random_float() * 2.0f * static_cast<float>(M_PI);
    float r = max_r * sqrtf(get_random_float());
    return {r * cosf(angle), r * sinf(angle)};
}

// --- SHAPES ---

// circle
inline std::vector<GLfloat> raw_circle_fan(float r, vec2 pos, int n) {
    int lg_lv = 2;

    lg.log("Initing vars", true, lg_lv);
    std::vector<GLfloat> vertices;

    std::vector<GLfloat> center = {pos.x, pos.y};
    std::vector<GLfloat> nextp;
    std::vector<GLfloat> prevp = {pos.x, pos.y + r}; 
    lg.log("Initing Done", false, lg_lv);

    lg.log("Starting calc for Triangle fan", true, lg_lv);
    for (float theta = 0; theta <= 360.0f; theta = theta + (360.0f / n)) {
        GLfloat px = r * sin(theta * (M_PI / 180.0f)) + pos.x;
        GLfloat py = r * cos(theta * (M_PI / 180.0f)) + pos.y;

        nextp = {px, py};

        vertices.insert(vertices.end(), center.begin(), center.end());
        vertices.insert(vertices.end(), prevp.begin(), prevp.end());
        vertices.insert(vertices.end(), nextp.begin(), nextp.end());

        prevp = nextp;

        std::string logstring = std::format("Calculated Triangle with cords ({}, {}) ({}, {}) ({}, {})",
            center[0], center[1], prevp[0], prevp[1], nextp[0], nextp[1]);
        lg.log(logstring, false, 3);


    }

    lg.log("Starting calc for Triangle fan... DONE", false, lg_lv);
    return vertices;
}

inline std::vector<GLfloat> verts_circle_fan(float r, vec2 pos, int n) {
    int lg_lv = 2;

    lg.log("Initing vars", true, lg_lv);
    std::vector<GLfloat> vertices;

    std::vector<GLfloat> center = {pos.x, pos.y};
    lg.log("Initing Done", false, lg_lv);

    vertices.insert(vertices.end(), center.begin(), center.end()); // center

    lg.log("Starting calc for Triangle fan", true, lg_lv);
    for (int i = 0; i < n; i++) {
        float theta = i * (360.0f / n);
        GLfloat px = r * sin(theta * (M_PI / 180.0f)) + pos.x;
        GLfloat py = r * cos(theta * (M_PI / 180.0f)) + pos.y;

        vertices.insert(vertices.end(), {px, py});

    }

    lg.log("Starting calc for Triangle fan... DONE", false, lg_lv);
    return vertices;
}

inline std::vector<int> indicies_circle_fan(int n) {
    int lg_lv = 2;
    std::vector<int> indicies;

    lg.log("Starting calc for Triangle fan indicies...", true, lg_lv);

    for (int p = 1; p < n; p++) {
        indicies.push_back(0); // center = point 0
        indicies.push_back(p); // prev
        indicies.push_back(p+1); // current point
    }

    indicies.push_back(0);
    indicies.push_back(n);
    indicies.push_back(1);

    lg.log("Starting calc for Triangle fan indicies... DONE", false, lg_lv);
    return indicies;

}

// quad

// inline std::vector<GLfloat> raw_quad(float x, float y, float w, float h) {return;}

// inline std::vector<GLfloat> verts_quad(float x, float y, float w, float h) {return;}

// inline std::vector<int> indicies_quad() {return;}

// triangle

// inline std::vector<GLfloat> raw_triangle(float x1, float y1, float x2, float y2, float x3, float y3) {return;}

// inline std::vector<GLfloat> verts_quad(float x1, float y1, float x2, float y2, float x3, float y3) {return;}

// inline std::vector<int> indicies_quad() {return;}

// ring
inline std::vector<float> ring(float r1, float r2, int n, vec2 pos) {
    std::vector<float> vertices;
    for (int i = 0; i < n; i++) {
        float theta = i * (360.0f / n) * (M_PI / 180.0f);
        vertices.push_back(r1 * cos(theta) + pos.x);
        vertices.push_back(r1 * sin(theta) + pos.y);
        vertices.push_back(r2 * cos(theta) + pos.x);
        vertices.push_back(r2 * sin(theta) + pos.y);
    }
    return vertices;
}

inline std::vector<int> indicies_ring(int n) {
    std::vector<int> indices;
    for (int i = 0; i < n; i++) {
        int o0 = i * 2;
        int i0 = i * 2 + 1;
        int o1 = ((i + 1) % n) * 2;
        int i1 = ((i + 1) % n) * 2 + 1;
        indices.push_back(o0); indices.push_back(i0); indices.push_back(o1);
        indices.push_back(i0); indices.push_back(i1); indices.push_back(o1);
    }
    return indices;
}

inline std::vector<int> offset_indicies(const std::vector<int>& indicies, int offset) {
    std::vector<int> result;
    result.reserve(indicies.size());
    for (int idx : indicies)
        result.push_back(idx + offset);
    return result;
}

// --- COLOR FUNCTIONS ---
inline std::vector<float> color_circle_fan(int n) {
    std::vector<float> colors;
    for (int p=0; (p <= n); p++) {
        colors.push_back(1.0f);
        colors.push_back(0.0f);
        colors.push_back(0.0f);
        colors.push_back(1.0f);

    }
    return colors;

}

inline std::vector<float> random_colors(int n, bool ran_alpha) {
    std::vector<float> colors;

    for (int i = 0; i <= n; i++) {
        colors.push_back(get_random_float()); // r
        colors.push_back(get_random_float()); // g
        colors.push_back(get_random_float()); // b

        if (!ran_alpha) {colors.push_back(get_random_float());} // random alpha
        else {colors.push_back(1.0f);} // normal alpha

    }

    return colors;

}

inline std::vector<float> mono_color(float r, float g, float b, float a, int n) {
    std::vector<float> colors;

    for (int i = 0; i <= n; i++) {
        colors.push_back(r); // red
        colors.push_back(g); // green
        colors.push_back(b); // blue
        colors.push_back(a); // alpha
    }

    return colors;
}

// Exactly vert_count RGBA entries in one colour (mono_color emits n+1, hence the -1).
inline std::vector<float> note_colors(int vert_count, vec4 c) {
    if (vert_count <= 0) return {};
    return mono_color(c.r, c.g, c.b, c.a, vert_count - 1);
}

// --- FUNCTIONS ---

inline float parabola(float x, float a, float b, float c) {
    return a*(x*x) + b*x + c;
}

inline float linear(float x, float m, float c) {
    return m*x + c;
}

// inline float sin_squared(float x) {return;}

// inline float spiral(float x, float m, float c, float t) {return;}

