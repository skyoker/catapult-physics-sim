#include "graphics/notemesh.hpp"
#include "utl/logger.hpp"

void NoteMesh::rebuild() {
    std::vector<GLfloat> verts;

    switch (kind) {
        case NoteKind::Arrow:
            verts = arrow_verts(anchor, anchor + vec, head_frac, head_spokes);
            break;

        case NoteKind::Line: {
            if (vec3_len(vec) > 1e-9L) {
                push_vert(verts, anchor);
                push_vert(verts, anchor + vec);
            }
            break;
        }

        case NoteKind::Point:
            verts = point_marker_verts(anchor, radius, segments);
            break;

        case NoteKind::Trail:
            verts = line_strip_verts(points);
            break;

        case NoteKind::Custom:
            verts = custom_verts;
            break;
    }

    dirty = false;

    int vert_count = (int)verts.size() / 3;
    if (vert_count == 0) {
        // nothing to show this frame (zero-length vector, trail with <2 points).
        // Keep the buffers intact and just draw nothing.
        mesh.indices.clear();
        return;
    }

    mesh.draw_mode = GL_LINES;
    mesh.update(verts, note_indices(vert_count), note_colors(vert_count, color));
    lg.log("Rebuilt NoteMesh geometry", false, 3);
}

void NoteMesh::draw() {
    if (!visible || mesh.indices.empty() || !mesh.buffers_ready) return;
    mesh.Bind();
    mesh.draw();
}

// --- builders ---

NoteMesh make_arrow_note(vec3 from, vec3 to, vec4 color) {
    NoteMesh n;
    n.kind = NoteKind::Arrow;
    n.origin = from;
    n.anchor = from;
    n.vec = to - from;
    n.color = color;
    return n;
}

NoteMesh make_point_note(vec3 pos, float radius, vec4 color) {
    NoteMesh n;
    n.kind = NoteKind::Point;
    n.origin = pos;
    n.anchor = pos;
    n.radius = radius;
    n.color = color;
    return n;
}

NoteMesh make_trail_note(int body_index, vec4 color, int max_points, bool perma) {
    NoteMesh n;
    n.kind = NoteKind::Trail;
    n.attach_to = body_index;
    n.color = color;
    n.max_points = max_points;
    n.perma = perma;
    return n;
}

NoteMesh make_vector_note(int body_index, NoteTrack src, float scale, vec4 color) {
    NoteMesh n;
    n.kind = NoteKind::Arrow;
    n.attach_to = body_index;
    n.track = src;
    n.track_scale = scale;
    n.color = color;
    return n;
}
