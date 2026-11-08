#pragma once
#include <cmath> #include "graphics/scene.hpp"
#include "graphics/camera.hpp"
#include "utl/utils.hpp"
#include "utl/constants.hpp"


inline PhysicsEngine3D falling_body() {
    static Mesh3D gravity_source({}, {}, {});
    static Mesh3D sphere_mesh(sphere_verts(0.5f, 16, 16), sphere_indices(16, 16), mono_color(1.0f, 0.0f, 0.0f, 1.0f, 241));

    PhysicsEngine3D scene;
    scene.show_axis = false;

    scene.grid_size = 200.0f;
    scene.grid_tile = 0.5f;
    scene.grid_y = 0.0f;
    scene.grid_color = {0.4f, 0.5f, 0.3f, 1.0f};
    scene.rebuild_grid();

    scene.camera.distance = 150.0f;
    scene.camera.pitch = 25.0f;
    scene.camera.near = 0.01f;
    scene.camera.far = 1000.0f;
    scene.G = G_SI;

    scene.add(gravity_source, {0.0, -EARTH_RADIUS_M, 0.0}, {0.0, 0.0, 0.0}, {1,1,1}, EARTH_MASS_KG, true);
    scene.add(sphere_mesh, {0.0, 10.0, 0.0}, {0.0, 0.0, 0.0}, {1,1,1}, 1.0f);

    scene.objects[0].collidable = true;
    scene.objects[0].bounding_radius = EARTH_RADIUS_M;

    scene.objects[1].collidable = true;
    scene.objects[1].bounding_radius = 0.5f;
    scene.objects[1].restitution = 0.0f;
    scene.objects[1].velocity = {20.0f, 10.0f, 0.0f};

    scene.add_trail(1, {0.2f, 0.5f, 1.0f, 1.0f});

    // NoteMesh demo - safe to delete, none of this affects the simulation
    scene.add_vector_note(1, NoteTrack::Velocity, 0.5f, {0.2f, 1.0f, 0.2f, 1.0f});
    scene.add_point({0.0, 10.0, 0.0}, 0.5f, {1.0f, 1.0f, 0.2f, 1.0f});

    return scene;
}
