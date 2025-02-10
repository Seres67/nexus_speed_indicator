#include <globals.hpp>
#include <gui.hpp>
#include <imgui/imgui.h>
#include <settings.hpp>

bool tmp_open = true;
Vector3 last_position = {};
std::chrono::time_point<std::chrono::system_clock> last_time = std::chrono::system_clock::now();
std::deque<std::pair<double, float>> speed_data; // Stores distance + time delta
constexpr int POLLING_RATE = 30;
constexpr double inches_per_mumble_unit = 100.0 / 2.54;

double speed_2d;
double speed_3d;

double last_speed_2d;
double last_speed_3d;

double speed_in_game_per_sec;
int zero_count = 0;

void render_window()
{
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;
    ImGui::SetNextWindowPos(ImVec2(300, 400), ImGuiCond_FirstUseEver);
    if (tmp_open && ImGui::Begin("Speed##SpeedIndicatorMainWindow", &tmp_open, flags)) {
        const auto current_time = std::chrono::system_clock::now();
        const Vector3 current_position = mumble_link->AvatarPosition;
        const auto time_span = std::chrono::duration<double>(current_time - last_time);
        // if (const auto time_span = std::chrono::duration<float>(current_time - last_time);
        //     time_span.count() >= 1.f / POLLING_RATE) {
        double distance = std::hypot(current_position.X - last_position.X, current_position.Z - last_position.Z);
        // double distance_3d = std::sqrt(std::pow(current_position.X - last_position.X, 2) +
        // std::pow(current_position.Y - last_position.Y, 2) +
        // std::pow(current_position.Z - last_position.Z, 2));

        // Store distance and time
        speed_data.emplace_back(distance, time_span.count());

        if (speed_data.size() > POLLING_RATE)
            speed_data.pop_front();

        // Sum distances and times
        double total_distance = 0.0;
        double total_time = 0.0;
        for (const auto &[dist, dt] : speed_data) {
            total_distance += dist;
            total_time += dt;
        }

        // Convert units and calculate speed
        constexpr double inches_per_mumble_unit = 100.0 / 2.54;
        const double distance_in_game = total_distance * inches_per_mumble_unit;
        speed_in_game_per_sec = (total_time > 0) ? (distance_in_game / total_time) : 0;

        last_time = current_time;
        last_position = current_position;
        // }
        ImGui::Text("Speed (u/s): %.0f", std::round(speed_in_game_per_sec));
        ImGui::End();
    }
}

// void render_window()
// {
//     ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;
//     ImGui::SetNextWindowPos(ImVec2(300, 400), ImGuiCond_FirstUseEver);
//     if (tmp_open && ImGui::Begin("Speed##SpeedIndicatorMainWindow", &tmp_open, flags)) {
//         if (std::chrono::duration<float>(std::chrono::system_clock::now() - last_time).count() >= 1.f / POLLING_RATE)
//         {
//             const auto current_time = std::chrono::system_clock::now();
//             const auto time_span = std::chrono::duration<float>(current_time - last_time);
//             const auto time_in_s = std::round(time_span.count() * 100) / 100;
//             const Vector3 current_position = mumble_link->AvatarPosition;
//             const double distance = std::sqrt(std::pow(current_position.X - last_position.X, 2) +
//                                               std::pow(current_position.Y - last_position.Y, 2) +
//                                               std::pow(current_position.Z - last_position.Z, 2));
//             const double distance_game = distance * inches_per_mumble_unit;
//             speed_3d = distance_game / time_in_s;
//             if (speed_3d == 0.0f) {
//                 ++zero_count;
//                 if (zero_count <= 7) {
//                     speed_3d = last_speed_3d;
//                     speed_2d = last_speed_2d;
//                 }
//             } else {
//                 zero_count = 0;
//                 last_speed_3d = speed_3d;
//                 last_speed_2d = speed_2d;
//             }
//             last_time = current_time;
//             last_position = current_position;
//         }
//         ImGui::Text("Speed 3D (u/s): %.0f", speed_3d);
//         ImGui::End();
//     }
// }

void render_options() {}