#include "graphics/engine.hpp"
#include "graphics/shader.hpp"
#include "graphics/shader_sources.hpp"
#include "graphics/scene.hpp"
#include "utl/logger.hpp"

#include "physics/newton_mechanics/astronomy.hpp"
#include "physics/newton_mechanics/earth_mechanics.hpp"
#include "physics/newton_mechanics/catapultscene.hpp"
#include "tui/tui.hpp"


int main() {
    lg.log("Instancing engine...", true, 0);
    Engine3D engine(800, 600);
    lg.log("Instancing engine... DONE", false, 0);
 
    lg.log("Instancing shader...", true, 0);
    Shader shader3D(vertex3D_src, fragment3D_src);
    lg.log("Instancing shader... DONE", false, 0);

    lg.log("Setting up scene...", true, 0);
    CatapultRig rig = make_catapult_rig();
    lg.log("Setting up scene... DONE", false, 0);

    lg.log("Starting render loop", false, 0);
    float time_scale = 1.0f;
    double last_time = glfwGetTime();
    double sim_time = 0.0;
    bool running = true;

    // The logger writes straight to stdout and would shred the ncurses display, so it
    // goes quiet for exactly as long as the TUI owns the terminal.
    TUI tui;
    lg.on = false;
    tui.start();

    while (!glfwWindowShouldClose(engine.window)) {
        double current_time = glfwGetTime();
        float dt = (float)(current_time - last_time);
        last_time = current_time;

        bool quit = false;
        for (int key = tui.poll(); key != TUI::NO_KEY; key = tui.poll()) {
            switch (key) {
                case ' ': running = !running; break;
                case 'f': rig.fire();  sim_time = 0.0; running = true; break;
                case 'r': rig.reset(); sim_time = 0.0; break;
                case 'c': rig.clear_trail(); break;
                // changing a launch parameter re-points the notes (via reset) and
                // re-frames the view, so the new arc is actually on screen
                case 'h': rig.launch_height = tui.prompt_float("launch height (m)", rig.launch_height);
                          rig.reset(); rig.reframe(); sim_time = 0.0; break;
                case 'v': rig.launch_speed  = tui.prompt_float("launch speed (m/s)", rig.launch_speed);
                          rig.reset(); rig.reframe(); sim_time = 0.0; break;
                case 'a': rig.launch_angle  = tui.prompt_float("launch angle (deg)", rig.launch_angle);
                          rig.reset(); rig.reframe(); sim_time = 0.0; break;
                case 'q': quit = true; break;
                default: break;
            }
        }
        if (quit) break;

        rig.scene.camera.handle_input(engine.window);

        // paused means dt = 0: one code path, and the camera still orbits so you can
        // inspect a frozen arc from any angle
        float step = running ? dt * time_scale : 0.0f;
        sim_time += step;

        rig.scene.update(step);
        rig.scene.render(shader3D, (float)engine.sx / (float)engine.sy);
        engine.update_frame();

        tui.draw(rig, running, sim_time);
    }

    tui.stop();
    lg.on = true;

    return 0;
}
