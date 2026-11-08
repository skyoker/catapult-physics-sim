#pragma once
#include <string>
#include "physics/newton_mechanics/catapultscene.hpp"

/*
A small ncurses panel in the launching terminal, running in the same process and the same
frame loop as the 3D window. It reports live telemetry for the projectile and drives the
shot.

Keys:
  space  start/stop simulation time, wherever the ball happens to be
  r      reset  - ball back to its launch position, at rest, trail cleared
  f      fire   - launch from a reset using the current parameters
  c      clear  - wipe the trail, leave the ball where it is
  h/v/a  set launch height / speed / angle
  q      quit

The logger writes to stdout and would shred the display, so main disables it
(`lg.on = false`) for as long as the TUI is up.
*/

struct TUI {
    bool active = false;
    double last_draw = 0.0;
    double redraw_interval = 0.05;   // ~20 Hz; full frame rate just flickers

    // Returned by poll() when nothing is pending. Its own sentinel rather than ncurses'
    // ERR, so callers don't have to include <ncurses.h> and inherit its macros.
    static constexpr int NO_KEY = -1;

    void start();
    void stop();

    // Non-blocking: returns NO_KEY when no key is pending, so the render loop never stalls.
    int poll();

    void draw(const CatapultRig& rig, bool running, double sim_time);

    // Modal line entry. Returns `current` unchanged if the input is empty or unparseable.
    float prompt_float(const char* label, float current);
};
