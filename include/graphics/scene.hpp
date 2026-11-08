#pragma once
#include <vector>
#include "graphics/mesh3d.hpp"
#include "graphics/notemesh.hpp"
#include "graphics/camera.hpp"
#include "utl/utils.hpp"
#include "utl/constants.hpp"
#include <glm/glm.hpp>

struct PhysicsBody {
    Mesh3D* mesh;
    vec3 position;
    vec3 rotation;
    vec3 scale;
    vec3 velocity;
    vec3 acceleration;
    float mass;
    bool is_static;
    bool collidable = true;
    float bounding_radius = 0.5f;
    float restitution = 0.0f;

    // Stop dead on first contact and stay put, like a billiard ball dropped on the
    // ground. `frozen` is set by the engine once it has actually happened.
    bool freeze_on_collision = false;
    bool frozen = false;

    // Immovable for integration purposes, but still a normal body otherwise: it keeps
    // its mass and goes on pulling on everything else.
    bool is_fixed() const { return is_static || frozen; }

    PhysicsBody(Mesh3D* mesh, vec3 position, vec3 rotation, vec3 scale, float mass, bool is_static = false);
    glm::mat4 model_matrix() const;
    void apply_force(vec3 F);
    void update_physics(float dt);
};

struct PhysicsEngine3D {
    enum class Integrator {Euler, RK4};

    std::vector<PhysicsBody> objects;
    // Annotation geometry. Deliberately a separate vector from `objects`: the gravity,
    // collision and integration loops only ever walk `objects`, so notes cannot take
    // part in the simulation.
    std::vector<NoteMesh> notes;
    bool show_notes = true;
    OrbitCamera camera;
    Mesh3D axis_mesh;
    Mesh3D grid_mesh;
    bool show_grid = true;
    bool show_axis = true;
    float grid_size = 100.0f;
    float grid_tile = 1.0f;
    float grid_y = -0.1f;
    vec4 grid_color = {0.5f, 0.5f, 0.5f, 0.25f};
    Integrator integrator = Integrator::RK4;
    float G = G_AU;

    void rebuild_grid();

    PhysicsEngine3D();
    int add(Mesh3D& mesh, vec3 position, vec3 rotation, vec3 scale, float mass = 1.0f, bool is_static = false);
    void remove(int index);
    void render(Shader& shader, float aspect);
    void update(float dt);

    // Every gravitational pull currently acting on a body, one entry per source, for
    // readouts that want the breakdown rather than just the net.
    struct ForceOn { int source; vec3 force; };
    std::vector<ForceOn> forces_on(int body_index) const;

    // --- notes ---
    int  add_note(const NoteMesh& note);
    int  add_arrow(vec3 from, vec3 to, vec4 color);
    int  add_point(vec3 pos, float radius, vec4 color);
    int  add_trail(int body_index, vec4 color, int max_points = 500, bool perma = false);
    int  add_vector_note(int body_index, NoteTrack src, float scale, vec4 color);
    void remove_note(int index);
    void update_notes();
};
