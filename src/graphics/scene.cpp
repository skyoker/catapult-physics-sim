#include "graphics/scene.hpp"
#include "../physics/newton_mechanics/gravity.cpp"
#include "../physics/newton_mechanics/forces.cpp"
#include <glm/gtc/matrix_transform.hpp>
#include <format>

PhysicsBody::PhysicsBody(Mesh3D* mesh, vec3 position, vec3 rotation, vec3 scale, float mass, bool is_static)
    : mesh(mesh), position(position), rotation(rotation), scale(scale),
      velocity{0.0f, 0.0f, 0.0f}, acceleration{0.0f, 0.0f, 0.0f}, mass(mass), is_static(is_static) {
    lg.log("SceneObject constructed", false, 2);
}

void PhysicsBody::apply_force(vec3 F) {
    acceleration = acceleration + F / mass;
}

void PhysicsBody::update_physics(float dt) {
    if (is_fixed()) return;
    velocity = velocity + acceleration * dt;
    position = position + velocity * dt;
}

glm::mat4 PhysicsBody::model_matrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3((float)position.x, (float)position.y, (float)position.z));
    model = glm::rotate(model, glm::radians((float)rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians((float)rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians((float)rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3((float)scale.x, (float)scale.y, (float)scale.z));
    return model;
}

PhysicsEngine3D::PhysicsEngine3D() {
    int lg_lv = 1;
    lg.log("Constructing Scene...", true, lg_lv);
    auto av = axis_verts(2.0f);
    auto ai = axis_indices();
    auto ac = axis_colors();
    axis_mesh = Mesh3D(av, ai, ac, GL_LINES);
    rebuild_grid();
    lg.log("Constructing Scene... DONE", false, lg_lv);
}

void PhysicsEngine3D::rebuild_grid() {
    float half = grid_size * 0.5f;
    int divs = (int)(grid_size / grid_tile);
    auto gv = grid_verts(half, grid_y, divs);
    int vc = (int)gv.size() / 3;
    auto gi = grid_indices(vc);
    auto gc = grid_colors(vc, grid_color.r, grid_color.g, grid_color.b, grid_color.a);
    grid_mesh = Mesh3D(gv, gi, gc, GL_LINES);
}

int PhysicsEngine3D::add(Mesh3D& mesh, vec3 position, vec3 rotation, vec3 scale, float mass, bool is_static) {
    objects.emplace_back(&mesh, position, rotation, scale, mass, is_static);
    int index = (int)objects.size() - 1;
    lg.log(std::format("Added object to scene at index {}", index), false, 2);
    return index;
}

void PhysicsEngine3D::remove(int index) {
    int lg_lv = 1;
    if (index >= 0 && index < (int)objects.size()) {
        lg.log(std::format("Removing object at index {}...", index), true, lg_lv);
        objects.erase(objects.begin() + index);

        // erasing shifts every later index down, which would leave notes attached to
        // the wrong body (or past the end). Drop the orphans and rebase the rest.
        for (int i = (int)notes.size() - 1; i >= 0; i--) {
            if (notes[i].attach_to == index)
                notes.erase(notes.begin() + i);
            else if (notes[i].attach_to > index)
                notes[i].attach_to--;
        }

        lg.log(std::format("Removing object at index {}... DONE", index), false, lg_lv);
    }
}

std::vector<PhysicsEngine3D::ForceOn> PhysicsEngine3D::forces_on(int body_index) const {
    std::vector<ForceOn> out;
    if (body_index < 0 || body_index >= (int)objects.size()) return out;

    for (int j = 0; j < (int)objects.size(); j++) {
        if (j == body_index) continue;
        // universal_gravity_3d returns the pull on the first body towards the second
        vec3 F = universal_gravity_3d(
            objects[body_index].mass, objects[j].mass,
            objects[body_index].position, objects[j].position, G
        );
        out.push_back({j, F});
    }
    return out;
}

// --- notes ---

int PhysicsEngine3D::add_note(const NoteMesh& note) {
    notes.push_back(note);
    int index = (int)notes.size() - 1;
    lg.log(std::format("Added note to scene at index {}", index), false, 2);
    return index;
}

int PhysicsEngine3D::add_arrow(vec3 from, vec3 to, vec4 color) {
    return add_note(make_arrow_note(from, to, color));
}

int PhysicsEngine3D::add_point(vec3 pos, float radius, vec4 color) {
    return add_note(make_point_note(pos, radius, color));
}

int PhysicsEngine3D::add_trail(int body_index, vec4 color, int max_points, bool perma) {
    return add_note(make_trail_note(body_index, color, max_points, perma));
}

int PhysicsEngine3D::add_vector_note(int body_index, NoteTrack src, float scale, vec4 color) {
    return add_note(make_vector_note(body_index, src, scale, color));
}

void PhysicsEngine3D::remove_note(int index) {
    if (index >= 0 && index < (int)notes.size()) {
        notes.erase(notes.begin() + index);
        lg.log(std::format("Removed note at index {}", index), false, 2);
    }
}

// Refresh note placement and tracked vectors from the current body state. Called at the
// end of update(), so notes always describe the step that was just simulated.
void PhysicsEngine3D::update_notes() {
    for (auto& note : notes) {
        if (!note.visible) continue;

        // resolve the world anchor: an offset from the attached body, or a fixed point
        if (note.attach_to >= 0 && note.attach_to < (int)objects.size()) {
            note.anchor = objects[note.attach_to].position + note.origin;
            note.dirty = true;
        } else {
            note.anchor = note.origin;
        }

        // tracked vectors read live body data; manual ones keep whatever was set
        if (note.track != NoteTrack::None &&
            note.attach_to >= 0 && note.attach_to < (int)objects.size()) {
            const PhysicsBody& b = objects[note.attach_to];
            vec3 src{0.0L, 0.0L, 0.0L};
            switch (note.track) {
                case NoteTrack::Velocity:     src = b.velocity; break;
                case NoteTrack::Acceleration: src = b.acceleration; break;
                case NoteTrack::Force:        src = b.acceleration * b.mass; break;
                case NoteTrack::None:         break;
            }
            note.vec = src * note.track_scale;
            note.dirty = true;
        }

        if (note.kind == NoteKind::Trail && note.attach_to >= 0 &&
            note.attach_to < (int)objects.size()) {
            note.points.push_back(objects[note.attach_to].position);
            if (!note.perma && (int)note.points.size() > note.max_points)
                note.points.erase(note.points.begin());
            note.dirty = true;
        }
    }
}

void PhysicsEngine3D::update(float dt) {
    if (integrator == Integrator::Euler) {
        for (auto& obj : objects)
            if (!obj.is_fixed()) obj.acceleration = {0.0f, 0.0f, 0.0f};

        for (int i = 0; i < (int)objects.size(); i++) {
            for (int j = i + 1; j < (int)objects.size(); j++) {
                vec3 F = universal_gravity_3d(
                    objects[i].mass, objects[j].mass,
                    objects[i].position, objects[j].position, G
                );
                objects[i].apply_force(F);
                objects[j].apply_force({-F.x, -F.y, -F.z});
            }
        }

        for (auto& obj : objects)
            obj.update_physics(dt);
    } else {
        int n = (int)objects.size();
        vec3 pos[32] = {}, vel[32] = {}, acc[32] = {};
        vec3 temp_pos[32] = {}, temp_vel[32] = {};
        vec3 k_pos[4][32] = {}, k_vel[4][32] = {};

        for (int i = 0; i < n; i++) {
            pos[i] = objects[i].position;
            vel[i] = objects[i].velocity;
        }

        auto compute_acc = [&](vec3* p, vec3* a) {
            for (int i = 0; i < n; i++) a[i] = {0.0f, 0.0f, 0.0f};
            for (int i = 0; i < n; i++) {
                for (int j = i + 1; j < n; j++) {
                    vec3 F = universal_gravity_3d(
                        objects[i].mass, objects[j].mass, p[i], p[j], G
                    );
                    a[i] = a[i] + F / objects[i].mass;
                    a[j] = a[j] + vec3{-F.x, -F.y, -F.z} / objects[j].mass;
                }
            }
        };

        // Stage 1
        compute_acc(pos, acc);
        for (int i = 0; i < n; i++) {
            if (objects[i].is_fixed()) {
                k_pos[0][i] = {0,0,0}; k_vel[0][i] = {0,0,0};
            } else {
                k_pos[0][i] = vel[i];
                k_vel[0][i] = acc[i];
            }
        }

        // Stage 2
        for (int i = 0; i < n; i++) {
            temp_pos[i] = pos[i] + k_pos[0][i] * (dt * 0.5f);
            temp_vel[i] = vel[i] + k_vel[0][i] * (dt * 0.5f);
        }
        compute_acc(temp_pos, acc);
        for (int i = 0; i < n; i++) {
            if (objects[i].is_fixed()) {
                k_pos[1][i] = {0,0,0}; k_vel[1][i] = {0,0,0};
            } else {
                k_pos[1][i] = temp_vel[i];
                k_vel[1][i] = acc[i];
            }
        }

        // Stage 3
        for (int i = 0; i < n; i++) {
            temp_pos[i] = pos[i] + k_pos[1][i] * (dt * 0.5f);
            temp_vel[i] = vel[i] + k_vel[1][i] * (dt * 0.5f);
        }
        compute_acc(temp_pos, acc);
        for (int i = 0; i < n; i++) {
            if (objects[i].is_fixed()) {
                k_pos[2][i] = {0,0,0}; k_vel[2][i] = {0,0,0};
            } else {
                k_pos[2][i] = temp_vel[i];
                k_vel[2][i] = acc[i];
            }
        }

        // Stage 4
        for (int i = 0; i < n; i++) {
            temp_pos[i] = pos[i] + k_pos[2][i] * dt;
            temp_vel[i] = vel[i] + k_vel[2][i] * dt;
        }
        compute_acc(temp_pos, acc);
        for (int i = 0; i < n; i++) {
            if (objects[i].is_fixed()) {
                k_pos[3][i] = {0,0,0}; k_vel[3][i] = {0,0,0};
            } else {
                k_pos[3][i] = temp_vel[i];
                k_vel[3][i] = acc[i];
            }
        }

        // Final weighted sum
        for (int i = 0; i < n; i++) {
            // Write the acceleration back onto the body. The RK4 path integrates out of
            // its local k arrays and would otherwise leave `acceleration` at zero
            // forever, which starves both the force readout and the Acceleration/Force
            // note tracks. k_vel[0] is the acceleration at the start of this step.
            objects[i].acceleration = objects[i].is_fixed() ? vec3{0.0f, 0.0f, 0.0f}
                                                            : k_vel[0][i];

            if (objects[i].is_fixed()) continue;
            objects[i].velocity = vel[i] +
                (dt / 6.0f) * (k_vel[0][i] + k_vel[1][i] * 2.0f + k_vel[2][i] * 2.0f + k_vel[3][i]);
            objects[i].position = pos[i] +
                (dt / 6.0f) * (k_pos[0][i] + k_pos[1][i] * 2.0f + k_pos[2][i] * 2.0f + k_pos[3][i]);
        }
    }

    for (int i = 0; i < (int)objects.size(); i++) {
        if (!objects[i].collidable) continue;
        for (int j = i + 1; j < (int)objects.size(); j++) {
            if (!objects[j].collidable) continue;

            float e = objects[i].is_fixed() ? objects[j].restitution :
                      objects[j].is_fixed() ? objects[i].restitution :
                      std::min(objects[i].restitution, objects[j].restitution);
            if (sphere_collision(
                objects[i].position, objects[j].position,
                objects[i].velocity, objects[j].velocity,
                objects[i].mass, objects[j].mass,
                objects[i].bounding_radius, objects[j].bounding_radius,
                e
            )) {
                vec3 n = objects[i].position - objects[j].position;
                long double dist = sqrtl(n.x*n.x + n.y*n.y + n.z*n.z);
                long double overlap = (long double)objects[i].bounding_radius + (long double)objects[j].bounding_radius - dist;
                if (overlap > 0 && dist > 1e-10L) {
                    n = {n.x/dist, n.y/dist, n.z/dist};
                    if (objects[i].is_fixed())
                        objects[j].position = objects[j].position - n * overlap;
                    else if (objects[j].is_fixed())
                        objects[i].position = objects[i].position + n * overlap;
                    else {
                        objects[i].position = objects[i].position + n * (overlap * 0.5f);
                        objects[j].position = objects[j].position - n * (overlap * 0.5f);
                    }
                }

                // Stop dead on contact, after the overlap correction above has lifted the
                // body clear of the surface - freezing first would pin it half-sunk.
                for (int k : {i, j}) {
                    if (objects[k].freeze_on_collision && !objects[k].is_static) {
                        objects[k].frozen = true;
                        objects[k].velocity = {0.0f, 0.0f, 0.0f};
                        objects[k].acceleration = {0.0f, 0.0f, 0.0f};
                        lg.log(std::format("Body {} froze on collision", k), false, 2);
                    }
                }
            }
        }
    }

    for (auto& obj : objects) {
        if (obj.is_fixed() || !obj.collidable) continue;
        bool touching = false;
        for (auto& other : objects) {
            if (&obj == &other || !other.collidable) continue;
            vec3 d = obj.position - other.position;
            long double dist = sqrtl(d.x*d.x + d.y*d.y + d.z*d.z);
            if (dist < (long double)obj.bounding_radius + (long double)other.bounding_radius + 0.01L) {
                touching = true;
                break;
            }
        }
        if (touching) {
            float speed = sqrt(obj.velocity.x*obj.velocity.x + obj.velocity.y*obj.velocity.y + obj.velocity.z*obj.velocity.z);
            if (speed < 9.82f * dt)
                obj.velocity = {0, 0, 0};
        }
    }

    update_notes();
}

void PhysicsEngine3D::render(Shader& shader, float aspect) {
    int lg_lv = 3;
    lg.log("Rendering scene...", true, lg_lv);

    shader.Activate();
    glm::mat4 view = camera.view_matrix();
    glm::mat4 proj = camera.projection_matrix(aspect);

    GLint viewLoc = glGetUniformLocation(shader.ID, "u_view");
    GLint projLoc = glGetUniformLocation(shader.ID, "u_proj");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &proj[0][0]);

    for (auto& obj : objects) {
        GLint modelLoc = glGetUniformLocation(shader.ID, "u_model");
        glm::mat4 model = obj.model_matrix();
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

        obj.mesh->Bind();
        obj.mesh->draw();
    }

    // Notes and the grid/axis are authored in world space, so they draw with an
    // identity model matrix.
    glm::mat4 identity(1.0f);
    GLint modelLoc = glGetUniformLocation(shader.ID, "u_model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &identity[0][0]);

    if (show_notes && !notes.empty()) {
        // notes carry meaningful alpha, so blend them
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        bool depth_off = false;
        for (auto& note : notes) {
            if (!note.visible) continue;
            if (note.dirty) note.rebuild();

            if (note.always_on_top != depth_off) {
                // only touch the depth state when it actually has to change
                if (note.always_on_top) glDisable(GL_DEPTH_TEST);
                else                    glEnable(GL_DEPTH_TEST);
                depth_off = note.always_on_top;
            }
            note.draw();
        }
        if (depth_off) glEnable(GL_DEPTH_TEST);

        glDisable(GL_BLEND);
    }

    if (show_grid) {
        grid_mesh.Bind();
        grid_mesh.draw();
    }

    if (show_axis) {
        axis_mesh.Bind();
        axis_mesh.draw();
    }

    lg.log("Rendering scene... DONE", false, lg_lv);
}
