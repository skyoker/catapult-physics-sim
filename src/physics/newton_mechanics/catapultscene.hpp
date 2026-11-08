#pragma once
#include <cmath>
#include <algorithm>
#include "graphics/scene.hpp"
#include "graphics/camera.hpp"
#include "utl/utils.hpp"
#include "utl/constants.hpp"


/*
A catapult shot.

The projectile leaves the arm 5 m above the ground with a launch speed at some angle,
so it carries both an upward and a forward component - the same release a real catapult
gives. Gravity comes from the n-body solver: an Earth-mass static body is buried one
Earth radius below the origin, so the ball feels ~9.81 m/s^2 pulling it back down and
collides with that body's surface at y = 0.

NoteMeshes annotate the shot. None of them affect the simulation:
  - a point marking where the ball started
  - a point marking where it lands
  - a trail tracing the path it actually flew
  - the launch velocity and the ball's live velocity
  - the live acceleration changing that velocity
*/

inline PhysicsEngine3D catapult_scene(
    float launch_height = 5.0f,     // m above the ground
    float launch_speed  = 25.0f,    // m/s at release
    float launch_angle  = 45.0f,    // degrees above horizontal
    bool  stop_on_impact = true     // stop dead where it lands, instead of sliding on
) {
    static Mesh3D gravity_source({}, {}, {});
    static Mesh3D ball_mesh(sphere_verts(0.5f, 16, 16), sphere_indices(16, 16), mono_color(1.0f, 0.0f, 0.0f, 1.0f, 241));

    const float ball_radius = 0.5f;

    PhysicsEngine3D scene;
    scene.show_axis = false;
    scene.G = G_SI;

    // launch velocity: up and forward, along +x
    float theta = launch_angle * (float)M_PI / 180.0f;
    float vx = launch_speed * cosf(theta);
    float vy = launch_speed * sinf(theta);

    // Where it lands, from the projectile equation, so the marker can be placed before
    // the ball is ever stepped. The ball rests on the ground when its centre is one
    // radius up, so solve for y = ball_radius rather than y = 0:
    //   launch_height + vy*t - g*t^2/2 = ball_radius
    // derived from the same constants the solver uses, so the predicted landing agrees
    // with the simulated one instead of drifting from the 9.81 shorthand
    float g = G_SI * EARTH_MASS_KG / (EARTH_RADIUS_M * EARTH_RADIUS_M);
    float disc = vy * vy + 2.0f * g * (launch_height - ball_radius);
    float flight_time = (vy + sqrtf(disc)) / g;
    float landing_x = vx * flight_time;

    scene.grid_size = 200.0f;
    scene.grid_tile = 1.0f;
    scene.grid_y = 0.0f;
    scene.grid_color = {0.4f, 0.5f, 0.3f, 1.0f};
    scene.rebuild_grid();

    // frame the whole arc side-on, so the parabola reads at its true shape
    scene.camera.target = {landing_x * 0.5f, launch_height, 0.0f};
    scene.camera.distance = landing_x * 1.4f;
    scene.camera.pitch = 12.0f;
    scene.camera.yaw = 0.0f;
    scene.camera.near = 0.01f;
    scene.camera.far = 1000.0f;

    // the ground: an Earth-mass body whose surface sits at y = 0
    scene.add(gravity_source, {0.0, -EARTH_RADIUS_M, 0.0}, {0.0, 0.0, 0.0}, {1,1,1}, EARTH_MASS_KG, true);
    scene.objects[0].collidable = true;
    scene.objects[0].bounding_radius = EARTH_RADIUS_M;

    // the projectile
    scene.add(ball_mesh, {0.0, launch_height, 0.0}, {0.0, 0.0, 0.0}, {1,1,1}, 1.0f);
    scene.objects[1].collidable = true;
    scene.objects[1].bounding_radius = ball_radius;
    // only consulted when stop_on_impact is off - a frozen ball never bounces
    scene.objects[1].restitution = 0.0f;
    scene.objects[1].velocity = {vx, vy, 0.0f};
    scene.objects[1].freeze_on_collision = stop_on_impact;

    // --- annotation ---

    // the path actually flown
    scene.add_trail(1, {0.2f, 0.6f, 1.0f, 1.0f}, 4000);

    // where it started
    vec3 launch_pos = {0.0, launch_height, 0.0};
    scene.add_point(launch_pos, 1.0f, {1.0f, 0.85f, 0.1f, 1.0f});

    // the launch velocity. Deliberately world-fixed rather than attached to the ball, so
    // it stays at the release point as a record of the shot instead of flying along with
    // it. Drawn at 0.6 m of arrow per m/s, so a 25 m/s release reads as a 15 m arrow.
    const float vel_arrow_scale = 0.6f;
    vec3 launch_vel = {vx, vy, 0.0f};
    scene.add_arrow(launch_pos, launch_pos + launch_vel * vel_arrow_scale,
                    {0.3f, 1.0f, 0.4f, 1.0f});

    // Current velocity, attached to the projectile and refreshed by the scene each
    // frame. The shorter cyan arrow shows how its direction and magnitude change.
    scene.add_vector_note(1, NoteTrack::Velocity, vel_arrow_scale,
                          {0.2f, 0.9f, 1.0f, 1.0f});

    // Show gravity's live acceleration beside the velocity arrow, making the
    // frame-by-frame change in velocity direction easier to read.
    scene.add_vector_note(1, NoteTrack::Acceleration, 0.15f,
                          {1.0f, 0.45f, 0.15f, 1.0f});

    // where it lands
    scene.add_point({landing_x, ball_radius, 0.0}, 1.0f, {1.0f, 0.3f, 0.2f, 1.0f});

    return scene;
}


/*
An interactive, replayable version of the same shot.

catapult_scene() above bakes its parameters in at construction. The rig keeps them, plus
the indices of the notes it created, so the launch settings can be changed and the shot
re-fired without rebuilding the scene.
*/

struct CatapultRig {
    PhysicsEngine3D scene;

    float launch_height = 5.0f;
    float launch_speed  = 25.0f;
    float launch_angle  = 45.0f;
    float ball_radius   = 0.5f;

    int ball = 1;          // body index of the projectile
    int trail_note = 0;    // note indices, stable for the life of the rig
    int start_note = 1;
    int vel_note   = 2;
    int land_note  = 3;
    int live_vel_note = 4;
    int accel_note = 5;

    static constexpr float vel_arrow_scale = 0.6f;

    // gravity as the solver itself computes it, not the 9.81 shorthand
    float surface_g() const {
        return G_SI * EARTH_MASS_KG / (EARTH_RADIUS_M * EARTH_RADIUS_M);
    }

    vec3 launch_pos() const { return {0.0, (long double)launch_height, 0.0}; }

    vec3 launch_velocity() const {
        float theta = launch_angle * (float)M_PI / 180.0f;
        return {launch_speed * cosf(theta), launch_speed * sinf(theta), 0.0f};
    }

    // Closed-form landing point for the current parameters. Drag-free, so it is a
    // prediction rather than a measurement.
    float predicted_range() const {
        vec3 v = launch_velocity();
        float g = surface_g();
        float vy = (float)v.y;
        float disc = vy * vy + 2.0f * g * (launch_height - ball_radius);
        if (disc < 0.0f) return 0.0f;
        return (float)v.x * ((vy + sqrtf(disc)) / g);
    }

    float predicted_flight_time() const {
        float vx = (float)launch_velocity().x;
        return vx > 1e-6f ? predicted_range() / vx : 0.0f;
    }

    // Re-point every note at the current parameters. Mutates the existing notes in place
    // rather than adding or removing any, so the stored indices stay valid.
    void refresh_notes() {
        vec3 p = launch_pos();

        NoteMesh& start = scene.notes[start_note];
        start.origin = p;
        start.anchor = p;
        start.dirty = true;

        NoteMesh& arrow = scene.notes[vel_note];
        arrow.origin = p;
        arrow.anchor = p;
        arrow.vec = launch_velocity() * vel_arrow_scale;
        arrow.dirty = true;

        NoteMesh& land = scene.notes[land_note];
        land.origin = {(long double)predicted_range(), (long double)ball_radius, 0.0};
        land.anchor = land.origin;
        land.dirty = true;
    }

    // True while the ball is parked at the launch point waiting to be fired.
    bool armed = false;

    // Ball back to the launch point, trail wiped, and *held* there. Without the hold it
    // would simply free-fall the moment it was reset, landing before you could fire it -
    // reuse of `frozen` parks it, since a fixed body is skipped by the integrators.
    void reset() {
        PhysicsBody& b = scene.objects[ball];
        b.position = launch_pos();
        b.velocity = {0.0f, 0.0f, 0.0f};
        b.acceleration = {0.0f, 0.0f, 0.0f};
        b.frozen = true;
        armed = true;
        clear_trail();
        refresh_notes();
    }

    void clear_trail() { scene.notes[trail_note].points.clear(); scene.notes[trail_note].dirty = true; }

    // Re-frame on the new arc after the launch parameters change, otherwise a longer
    // shot simply flies off the edge of the view. yaw and pitch are left alone so
    // whatever angle you have orbited to is preserved. The ground grid grows with the
    // range too - it is only scenery (collision is against the Earth body, not the
    // grid), but a ball landing past its edge looks wrong.
    void reframe() {
        float range = predicted_range();
        scene.camera.target = {(long double)(range * 0.5f), (long double)launch_height, 0.0};
        scene.camera.distance = std::max(range * 1.4f, 10.0f);

        float wanted = std::max(200.0f, range * 2.4f);
        if (wanted > scene.grid_size) {
            scene.grid_size = wanted;
            // keep the line count sane on long shots
            scene.grid_tile = std::max(1.0f, wanted / 200.0f);
            scene.rebuild_grid();
        }
    }

    // Launch from a clean reset using the current parameters: release the hold, then
    // give it the launch velocity.
    void fire() {
        reset();
        scene.objects[ball].frozen = false;
        armed = false;
        scene.objects[ball].velocity = launch_velocity();
    }

    bool frozen() const { return scene.objects[ball].frozen; }

    bool at_rest() const {
        const PhysicsBody& b = scene.objects[ball];
        return b.frozen || vec3_len(b.velocity) < 1e-6L;
    }
};

inline CatapultRig make_catapult_rig(
    float launch_height = 5.0f,
    float launch_speed  = 25.0f,
    float launch_angle  = 45.0f
) {
    CatapultRig rig;
    rig.launch_height = launch_height;
    rig.launch_speed = launch_speed;
    rig.launch_angle = launch_angle;
    rig.scene = catapult_scene(launch_height, launch_speed, launch_angle);

    // note indices follow the order catapult_scene() adds them in
    rig.trail_note = 0;
    rig.start_note = 1;
    rig.vel_note   = 2;
    rig.land_note  = 3;
    rig.live_vel_note = 4;
    rig.accel_note = 5;

    return rig;
}
