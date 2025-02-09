#include <globals.hpp>
#include <gui.hpp>
#include <imgui/imgui.h>
#include <settings.hpp>

bool tmp_open = true;
Vector3 last_position = {};
std::chrono::time_point<std::chrono::system_clock> last_time = std::chrono::system_clock::now();
double speed_in_units_per_sec = 0;
double speed_in_inches_per_sec = 0;
double speed_in_mph = 0;
int count = 0;
Vector3 current_position = {};

constexpr double MOVEMENT_THRESHOLD = 0.002;
constexpr double MIN_SPEED_DISPLAY = 1;

void render_window()
{
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;
    ImGui::SetNextWindowPos(ImVec2(300, 400), ImGuiCond_FirstUseEver);
    if (tmp_open && ImGui::Begin("Speed##SpeedIndicatorMainWindow", &tmp_open, flags)) {
        current_position = mumble_link->AvatarPosition; //NOTE: this should be meters and game uses inches so we have to convert
        auto current_time = std::chrono::system_clock::now();
        auto time_span = std::chrono::duration<float>(current_time - last_time); // Time in seconds
        if (time_span.count() >= 0.033f) {
            Vector3 diff = {current_position.X - last_position.X, 0, current_position.Z - last_position.Z};
            double distance_2d = std::hypot(diff.X, diff.Z);
            if (distance_2d > MOVEMENT_THRESHOLD) {
                count = 0;
                double inches_per_game_unit = 100.0 / 2.54; // 1 game unit = 100 / 2.54 inches
                double distance_in_inches = distance_2d * inches_per_game_unit;
                double time_in_s = std::round(time_span.count() * 100) / 100;
                speed_in_units_per_sec = distance_2d / time_in_s;
                speed_in_inches_per_sec = distance_in_inches / time_in_s;
                speed_in_mph = (speed_in_inches_per_sec * 3600) /
                               63360; //NOTE: 1 mph = 63360 inches per hour (1 inch = 2.54 cm) & 1 hour = 3600 seconds
                last_time = current_time;
                last_position = current_position;
            } else {
                ++count;
                if (count >= 10) {
                    speed_in_units_per_sec = 0;
                    speed_in_inches_per_sec = 0;
                    speed_in_mph = 0;
                    count = 0;
                }
            }
        }
        ImGui::Text("Speed (u/s): %.0f\nSpeed (mph): %.0f",
                    speed_in_units_per_sec < MIN_SPEED_DISPLAY ? 0 : std::round(speed_in_units_per_sec),
                    speed_in_mph < MIN_SPEED_DISPLAY ? 0 : std::round(speed_in_mph));
        ImGui::End();
    }
}

void render_options() {}
