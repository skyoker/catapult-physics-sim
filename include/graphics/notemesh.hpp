#pragma once
#include <vector>
#include "graphics/mesh3d.hpp"
#include "utl/utils.hpp"

/*
NoteMesh - annotation geometry.

A NoteMesh is drawn but never simulated. It has no mass, takes no forces, and is
invisible to gravity and collision: the physics loops only ever walk PhysicsEngine3D's
`objects` vector, and notes live in a separate `notes` vector. They exist purely to
show the user vectors, points and volumes in the scene.

Placement:
  attach_to < 0   -> `origin` is a world position
  attach_to >= 0  -> `origin` is an offset from that body, so the note follows it

Vectors (kind == Arrow):
  track == None   -> `vec` is whatever the caller set; left alone
  track != None   -> `vec` is refreshed each frame from the body's live data
*/

enum class NoteKind {
    Line,    // a straight segment from origin to origin + vec
    Arrow,   // Line with an arrowhead: shows a vector
    Point,   // a ringed point: shows a position
    Trail,   // a polyline accumulated over time
    Custom,  // caller supplies the vertices directly
};

enum class NoteTrack {
    None,          // manual: `vec` is set by the caller
    Velocity,      // body velocity
    Acceleration,  // body acceleration
    Force,         // body acceleration * mass
};

struct NoteMesh {
    NoteKind kind = NoteKind::Custom;
    Mesh3D   mesh;
    vec4     color{1.0f, 1.0f, 1.0f, 1.0f};

    bool visible       = true;
    bool always_on_top = false;  // draw ignoring the depth buffer
    bool dirty         = true;   // geometry needs regenerating before the next draw

    // placement. `origin` is what the caller authored; `anchor` is the resolved world
    // position the geometry is actually built around, refreshed by the scene each frame.
    int  attach_to = -1;
    vec3 origin{0.0L, 0.0L, 0.0L};
    vec3 anchor{0.0L, 0.0L, 0.0L};

    // vector notes (Line / Arrow)
    vec3      vec{0.0L, 0.0L, 0.0L};
    NoteTrack track       = NoteTrack::None;
    float     track_scale = 1.0f;
    float     head_frac   = 0.2f;
    int       head_spokes = 6;

    // point notes
    float radius   = 0.25f;
    int   segments = 24;

    // trail notes
    std::vector<vec3> points;
    int  max_points = 500;
    bool perma      = false;

    // Custom notes: caller-supplied world-space vertices, used as-is.
    std::vector<GLfloat> custom_verts;

    NoteMesh() = default;

    // Regenerate the CPU-side geometry for `kind` and upload it. Clears `dirty`.
    void rebuild();
    void draw();
};

// --- builders ---
NoteMesh make_arrow_note(vec3 from, vec3 to, vec4 color);
NoteMesh make_point_note(vec3 pos, float radius, vec4 color);
NoteMesh make_trail_note(int body_index, vec4 color, int max_points, bool perma);
NoteMesh make_vector_note(int body_index, NoteTrack src, float scale, vec4 color);
