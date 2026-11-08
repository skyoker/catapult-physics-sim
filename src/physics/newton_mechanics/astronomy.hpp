#pragma once
#include <cmath>
#include "graphics/scene.hpp"
#include "graphics/camera.hpp"
#include "utl/utils.hpp"
#include "utl/constants.hpp"

inline PhysicsEngine3D make_solar_system_scene_true_scale() {
    const int sectors = 24, stacks = 12;
    const int vc = 2 + (stacks - 1) * sectors;

    static Mesh3D sun_mesh(sphere_verts(SUN_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(1.0f, 0.9f, 0.1f, 1.0f, vc - 1));
    static Mesh3D mercury_mesh(sphere_verts(MERCURY_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(0.7f, 0.7f, 0.7f, 1.0f, vc - 1));
    static Mesh3D venus_mesh(sphere_verts(VENUS_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(0.9f, 0.8f, 0.3f, 1.0f, vc - 1));
    static Mesh3D earth_mesh(sphere_verts(EARTH_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(0.2f, 0.5f, 1.0f, 1.0f, vc - 1));
    static Mesh3D mars_mesh(sphere_verts(MARS_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(0.8f, 0.3f, 0.2f, 1.0f, vc - 1));
    static Mesh3D jupiter_mesh(sphere_verts(JUPITER_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(0.9f, 0.7f, 0.4f, 1.0f, vc - 1));
    static Mesh3D saturn_mesh(sphere_verts(SATURN_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(0.9f, 0.8f, 0.5f, 1.0f, vc - 1));
    static Mesh3D uranus_mesh(sphere_verts(URANUS_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(0.5f, 0.8f, 0.9f, 1.0f, vc - 1));
    static Mesh3D neptune_mesh(sphere_verts(NEPTUNE_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(0.2f, 0.3f, 0.9f, 1.0f, vc - 1));
    static Mesh3D pluto_mesh(sphere_verts(PLUTO_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(0.6f, 0.6f, 0.5f, 1.0f, vc - 1));

    PhysicsEngine3D scene;
    scene.camera.distance = 2.0f;
    scene.camera.min_distance = 0.0001f;
    scene.camera.near = 0.00001f;
    scene.camera.far = 100.0f;

    scene.add(sun_mesh, {0,0,0}, {0,0,0}, {1,1,1}, SUN_MASS_AU, true);

    struct PlanetData { Mesh3D* mesh; float orbit_au; vec4 color; float mass; };
    PlanetData planets[] = {
        {&mercury_mesh, 0.387f,  {0.7f, 0.7f, 0.7f, 1.0f}, MERCURY_MASS_AU},
        {&venus_mesh,   0.723f,  {0.9f, 0.8f, 0.3f, 1.0f}, VENUS_MASS_AU},
        {&earth_mesh,   1.0f,    {0.2f, 0.5f, 1.0f, 1.0f}, EARTH_MASS_AU},
        {&mars_mesh,    1.524f,  {0.8f, 0.3f, 0.2f, 1.0f}, MARS_MASS_AU},
        {&jupiter_mesh, 5.203f,  {0.9f, 0.7f, 0.4f, 1.0f}, JUPITER_MASS_AU},
        {&saturn_mesh,  9.537f,  {0.9f, 0.8f, 0.5f, 1.0f}, SATURN_MASS_AU},
        {&uranus_mesh,  19.19f,  {0.5f, 0.8f, 0.9f, 1.0f}, URANUS_MASS_AU},
        {&neptune_mesh, 30.07f,  {0.2f, 0.3f, 0.9f, 1.0f}, NEPTUNE_MASS_AU},
        {&pluto_mesh,   39.48f,  {0.6f, 0.6f, 0.5f, 1.0f}, PLUTO_MASS_AU},
    };

    for (auto& p : planets) {
        float v = sqrtf(G_AU * SUN_MASS_AU / p.orbit_au);
        int idx = scene.add(*p.mesh, {p.orbit_au, 0, 0}, {0,0,0}, {1,1,1}, p.mass);
        scene.objects[idx].velocity = {0, 0, v};
        scene.add_trail(idx, p.color);
    }

    scene.show_grid = true;
    scene.grid_color = {0.3f, 0.3f, 0.3f, 0.15f};
    scene.grid_y = -0.01f;
    scene.show_axis = false;

    return scene;
}

inline PhysicsEngine3D make_sun_earth_moon_true_scale() {
    const int sectors = 24, stacks = 12;
    const int vc = 2 + (stacks - 1) * sectors;

    static Mesh3D sun_mesh(sphere_verts(SUN_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(1.0f, 0.9f, 0.1f, 1.0f, vc - 1));
    static Mesh3D earth_mesh(sphere_verts(EARTH_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(0.2f, 0.5f, 1.0f, 1.0f, vc - 1));
    static Mesh3D moon_mesh(sphere_verts(MOON_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(0.7f, 0.7f, 0.7f, 1.0f, vc - 1));

    PhysicsEngine3D scene;
    scene.camera.target = {0, 0, 0};
    scene.camera.distance = 1.5f;
    scene.camera.min_distance = 0.00001f;
    scene.camera.near = 0.000001f;
    scene.camera.far = 10.0f;
    scene.show_grid = false;
    scene.show_axis = false;

    float M_sun = SUN_MASS_AU;
    float M_earth = EARTH_MASS_AU;
    float M_moon = MOON_MASS_AU;

    float v_earth = sqrtf(G_AU * M_sun / 1.0f);
    float v_moon_rel = sqrtf(G_AU * M_earth / MOON_ORBIT_RADIUS_AU);

    scene.add(sun_mesh, {0,0,0}, {0,0,0}, {1,1,1}, M_sun, true);
    scene.add(earth_mesh, {1.0f,0,0}, {0,0,0}, {1,1,1}, M_earth);
    scene.objects[1].velocity = {0, 0, v_earth};
    scene.add(moon_mesh, {1.0f, MOON_ORBIT_RADIUS_AU, 0}, {0,0,0}, {1,1,1}, M_moon);
    scene.objects[2].velocity = {-v_moon_rel, 0, v_earth};

    scene.add_trail(1, {0.2f, 0.5f, 1.0f, 1.0f}, 500, true);
    scene.add_trail(2, {0.7f, 0.7f, 0.7f, 1.0f}, 500, true);

    return scene;
}

inline PhysicsEngine3D make_solar_system_scene_true_scale_3d() {
    const int sectors = 24, stacks = 12;
    const int vc = 2 + (stacks - 1) * sectors;

    static Mesh3D sun_mesh(sphere_verts(SUN_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(1.0f, 0.9f, 0.1f, 1.0f, vc - 1));
    static Mesh3D mercury_mesh(sphere_verts(MERCURY_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(0.7f, 0.7f, 0.7f, 1.0f, vc - 1));
    static Mesh3D venus_mesh(sphere_verts(VENUS_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(0.9f, 0.8f, 0.3f, 1.0f, vc - 1));
    static Mesh3D earth_mesh(sphere_verts(EARTH_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(0.2f, 0.5f, 1.0f, 1.0f, vc - 1));
    static Mesh3D mars_mesh(sphere_verts(MARS_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(0.8f, 0.3f, 0.2f, 1.0f, vc - 1));
    static Mesh3D jupiter_mesh(sphere_verts(JUPITER_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(0.9f, 0.7f, 0.4f, 1.0f, vc - 1));
    static Mesh3D saturn_mesh(sphere_verts(SATURN_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(0.9f, 0.8f, 0.5f, 1.0f, vc - 1));
    static Mesh3D uranus_mesh(sphere_verts(URANUS_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(0.5f, 0.8f, 0.9f, 1.0f, vc - 1));
    static Mesh3D neptune_mesh(sphere_verts(NEPTUNE_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(0.2f, 0.3f, 0.9f, 1.0f, vc - 1));
    static Mesh3D pluto_mesh(sphere_verts(PLUTO_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(0.6f, 0.6f, 0.5f, 1.0f, vc - 1));

    PhysicsEngine3D scene;
    scene.camera.distance = 2.0f;
    scene.camera.min_distance = 0.0001f;
    scene.camera.near = 0.00001f;
    scene.camera.far = 100.0f;

    scene.add(sun_mesh, {0,0,0}, {0,0,0}, {1,1,1}, SUN_MASS_AU, true);

    struct PlanetData { Mesh3D* mesh; float orbit_au; vec4 color; float mass; float incl_deg; };
    PlanetData planets[] = {
        {&mercury_mesh, 0.387f,  {0.7f, 0.7f, 0.7f, 1.0f}, MERCURY_MASS_AU, 7.0f},
        {&venus_mesh,   0.723f,  {0.9f, 0.8f, 0.3f, 1.0f}, VENUS_MASS_AU,   3.4f},
        {&earth_mesh,   1.0f,    {0.2f, 0.5f, 1.0f, 1.0f}, EARTH_MASS_AU,   0.0f},
        {&mars_mesh,    1.524f,  {0.8f, 0.3f, 0.2f, 1.0f}, MARS_MASS_AU,    1.85f},
        {&jupiter_mesh, 5.203f,  {0.9f, 0.7f, 0.4f, 1.0f}, JUPITER_MASS_AU, 1.3f},
        {&saturn_mesh,  9.537f,  {0.9f, 0.8f, 0.5f, 1.0f}, SATURN_MASS_AU,  2.49f},
        {&uranus_mesh,  19.19f,  {0.5f, 0.8f, 0.9f, 1.0f}, URANUS_MASS_AU,  0.77f},
        {&neptune_mesh, 30.07f,  {0.2f, 0.3f, 0.9f, 1.0f}, NEPTUNE_MASS_AU, 1.77f},
        {&pluto_mesh,   39.48f,  {0.6f, 0.6f, 0.5f, 1.0f}, PLUTO_MASS_AU,   17.2f},
    };

    for (auto& p : planets) {
        float v = sqrtf(G_AU * SUN_MASS_AU / p.orbit_au);
        float i = p.incl_deg * (float)M_PI / 180.0f;
        int idx = scene.add(*p.mesh, {p.orbit_au, 0, 0}, {0,0,0}, {1,1,1}, p.mass);
        scene.objects[idx].velocity = {0, v * sinf(i), v * cosf(i)};
        scene.add_trail(idx, p.color, 500000);
    }

    scene.show_grid = true;
    scene.grid_color = {0.3f, 0.3f, 0.3f, 0.15f};
    scene.grid_y = -0.01f;
    scene.show_axis = false;

    return scene;
}

inline PhysicsEngine3D make_sun_earth_moon_true_scale_3d() {
    const int sectors = 24, stacks = 12;
    const int vc = 2 + (stacks - 1) * sectors;

    static Mesh3D sun_mesh(sphere_verts(SUN_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(1.0f, 0.9f, 0.1f, 1.0f, vc - 1));
    static Mesh3D earth_mesh(sphere_verts(EARTH_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(0.2f, 0.5f, 1.0f, 1.0f, vc - 1));
    static Mesh3D moon_mesh(sphere_verts(MOON_RADIUS_AU, sectors, stacks), sphere_indices(sectors, stacks), mono_color(0.7f, 0.7f, 0.7f, 1.0f, vc - 1));

    PhysicsEngine3D scene;
    scene.camera.target = {0, 0, 0};
    scene.camera.distance = 1.5f;
    scene.camera.min_distance = 0.00001f;
    scene.camera.near = 0.000001f;
    scene.camera.far = 10.0f;
    scene.show_grid = false;
    scene.show_axis = false;

    float M_sun = SUN_MASS_AU;
    float M_earth = EARTH_MASS_AU;
    float M_moon = MOON_MASS_AU;

    float v_earth = sqrtf(G_AU * M_sun / 1.0f);

    float moon_incl = 5.14f * (float)M_PI / 180.0f;
    float r = MOON_ORBIT_RADIUS_AU;
    float v_moon_rel = sqrtf(G_AU * M_earth / r);

    scene.add(sun_mesh, {0,0,0}, {0,0,0}, {1,1,1}, M_sun, true);
    scene.add(earth_mesh, {1.0f,0,0}, {0,0,0}, {1,1,1}, M_earth);
    scene.objects[1].velocity = {0, 0, v_earth};
    scene.add(moon_mesh, {1.0f, r * cosf(moon_incl), r * sinf(moon_incl)}, {0,0,0}, {1,1,1}, M_moon);
    scene.objects[2].velocity = {-v_moon_rel, 0, v_earth};

    scene.add_trail(1, {0.2f, 0.5f, 1.0f, 1.0f}, 500, true);
    scene.add_trail(2, {0.7f, 0.7f, 0.7f, 1.0f}, 500, true);

    return scene;
}