#include "tui/tui.hpp"
#include <ncurses.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <chrono>

namespace {

// colour pair ids
constexpr short CP_TITLE  = 1;
constexpr short CP_LABEL  = 2;
constexpr short CP_VALUE  = 3;
constexpr short CP_OK     = 4;
constexpr short CP_WARN   = 5;

long double vlen(vec3 v) { return sqrtl(v.x*v.x + v.y*v.y + v.z*v.z); }

void hline_at(int row, int width) {
    mvhline(row, 0, ACS_HLINE, width);
}

} // namespace

void TUI::start() {
    initscr();
    cbreak();               // no line buffering
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);  // getch returns ERR instead of blocking
    keypad(stdscr, TRUE);

    if (has_colors()) {
        start_color();
        use_default_colors();
        init_pair(CP_TITLE, COLOR_CYAN,   -1);
        init_pair(CP_LABEL, COLOR_WHITE,  -1);
        init_pair(CP_VALUE, COLOR_YELLOW, -1);
        init_pair(CP_OK,    COLOR_GREEN,  -1);
        init_pair(CP_WARN,  COLOR_RED,    -1);
    }
    active = true;
}

void TUI::stop() {
    if (!active) return;
    curs_set(1);
    echo();
    nodelay(stdscr, FALSE);
    endwin();
    active = false;
}

int TUI::poll() {
    if (!active) return NO_KEY;
    int key = getch();
    return key == ERR ? NO_KEY : key;
}

float TUI::prompt_float(const char* label, float current) {
    if (!active) return current;

    int row = getmaxy(stdscr) - 1;
    nodelay(stdscr, FALSE);
    echo();
    curs_set(1);

    move(row, 0);
    clrtoeol();
    attron(COLOR_PAIR(CP_VALUE));
    mvprintw(row, 0, "%s [%.3g] = ", label, current);
    attroff(COLOR_PAIR(CP_VALUE));
    refresh();

    char buf[64] = {0};
    getnstr(buf, sizeof(buf) - 1);

    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    move(row, 0);
    clrtoeol();

    // empty or unparseable input leaves the value alone
    char* end = nullptr;
    float parsed = strtof(buf, &end);
    if (end == buf || buf[0] == '\0') return current;
    return parsed;
}

void TUI::draw(const CatapultRig& rig, bool running, double sim_time) {
    if (!active) return;

    // wall clock, not clock(): with vsync the process spends most of its time asleep, so
    // CPU time would throttle the redraw far below the intended rate
    using clk = std::chrono::steady_clock;
    double now = std::chrono::duration<double>(clk::now().time_since_epoch()).count();
    if (now - last_draw < redraw_interval) return;
    last_draw = now;

    const PhysicsBody& b = rig.scene.objects[rig.ball];
    int w = getmaxx(stdscr);
    if (w > 60) w = 60;

    erase();
    int row = 0;

    attron(COLOR_PAIR(CP_TITLE) | A_BOLD);
    mvprintw(row++, 0, "CATAPULT SIM");
    attroff(COLOR_PAIR(CP_TITLE) | A_BOLD);
    hline_at(row++, w);

    // --- launch parameters ---
    attron(COLOR_PAIR(CP_LABEL));
    mvprintw(row++, 0, "LAUNCH");
    attroff(COLOR_PAIR(CP_LABEL));
    attron(COLOR_PAIR(CP_VALUE));
    mvprintw(row++, 2, "height   %8.2f m", rig.launch_height);
    mvprintw(row++, 2, "speed    %8.2f m/s", rig.launch_speed);
    mvprintw(row++, 2, "angle    %8.2f deg", rig.launch_angle);
    attroff(COLOR_PAIR(CP_VALUE));
    mvprintw(row++, 2, "predicted range %6.2f m  (t %.2f s)",
             rig.predicted_range(), rig.predicted_flight_time());
    hline_at(row++, w);

    // --- live state ---
    attron(COLOR_PAIR(CP_LABEL));
    mvprintw(row++, 0, "BALL");
    attroff(COLOR_PAIR(CP_LABEL));
    mvprintw(row++, 2, "pos   %9.3f %9.3f %9.3f  m",
             (double)b.position.x, (double)b.position.y, (double)b.position.z);
    mvprintw(row++, 2, "vel   %9.3f %9.3f %9.3f  m/s",
             (double)b.velocity.x, (double)b.velocity.y, (double)b.velocity.z);
    mvprintw(row++, 2, "speed %9.3f m/s      mass %.3f kg",
             (double)vlen(b.velocity), b.mass);
    mvprintw(row++, 2, "sim t %9.3f s", sim_time);

    // status
    move(row, 2);
    printw("state ");
    if (rig.armed) {
        attron(COLOR_PAIR(CP_VALUE) | A_BOLD);
        printw("ARMED - held at launch point, press f to fire");
        attroff(COLOR_PAIR(CP_VALUE) | A_BOLD);
    } else if (b.frozen) {
        attron(COLOR_PAIR(CP_WARN) | A_BOLD);
        printw("LANDED (frozen)");
        attroff(COLOR_PAIR(CP_WARN) | A_BOLD);
    } else if (!running) {
        attron(COLOR_PAIR(CP_WARN));
        printw("PAUSED");
        attroff(COLOR_PAIR(CP_WARN));
    } else if (rig.at_rest()) {
        printw("ready");
    } else {
        attron(COLOR_PAIR(CP_OK));
        printw("IN FLIGHT");
        attroff(COLOR_PAIR(CP_OK));
    }
    row++;
    hline_at(row++, w);

    // --- forces ---
    attron(COLOR_PAIR(CP_LABEL));
    mvprintw(row++, 0, "FORCES ON BALL");
    attroff(COLOR_PAIR(CP_LABEL));

    auto forces = rig.scene.forces_on(rig.ball);
    vec3 net{0.0L, 0.0L, 0.0L};
    for (const auto& f : forces) {
        net = net + f.force;
        mvprintw(row++, 2, "grav<-%-2d %8.3f %8.3f %8.3f  |%.3f| N",
                 f.source,
                 (double)f.force.x, (double)f.force.y, (double)f.force.z,
                 (double)vlen(f.force));
    }
    mvprintw(row++, 2, "net      %8.3f %8.3f %8.3f  |%.3f| N",
             (double)net.x, (double)net.y, (double)net.z, (double)vlen(net));

    // m*a as an independent read of the same quantity: it comes from the integrator,
    // the rows above come from recomputing gravity, so a mismatch means one is wrong.
    vec3 ma = b.acceleration * b.mass;
    mvprintw(row++, 2, "m*a      %8.3f %8.3f %8.3f  |%.3f| N",
             (double)ma.x, (double)ma.y, (double)ma.z, (double)vlen(ma));

    if (b.frozen)
        mvprintw(row++, 2, "contact  held by freeze-on-collision constraint");

    hline_at(row++, w);

    // --- keys ---
    attron(COLOR_PAIR(CP_LABEL));
    mvprintw(row++, 0, "KEYS");
    attroff(COLOR_PAIR(CP_LABEL));
    mvprintw(row++, 2, "space %s   f fire   r reset   c clear trail",
             running ? "stop " : "start");
    mvprintw(row++, 2, "h height   v speed  a angle   q quit");

    refresh();
}
