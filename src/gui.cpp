#include <chrono>
#include <globals.hpp>
#include <gui.hpp>
#include <imgui/imgui.h>
#include <settings.hpp>

bool tmp_open = true;
Vector3 last_position = {};
std::chrono::time_point<std::chrono::steady_clock> last_time = std::chrono::steady_clock::now();
constexpr double FIXED_DELTA_TIME = 1.0 / 50.0;

double speed_2d;
double speed_3d;

double last_speed_2d;
double last_speed_3d;

double speed_in_game_per_sec;
char zero_count = 0;

std::chrono::time_point<std::chrono::steady_clock> last_mumble_update;

constexpr double CONVERSION_FACTOR_U_S = 19.68491;
constexpr double CONVERSION_FACTOR_FOOT = 6.69555;
constexpr double CONVERSION_FACTOR_BEETLE = 1.08252;

double calculate_speed_3d(const Vector3 &prev, const Vector3 &current, const double deltaTime)
{
    if (deltaTime <= 0)
        return 0.0f;
    const double dx = current.X - prev.X;
    const double dy = current.Y - prev.Y;
    const double dz = current.Z - prev.Z;
    return sqrt(dx * dx + dy * dy + dz * dz) / deltaTime;
}

double calculate_speed_2d(const Vector3 &prev, const Vector3 &current, const double deltaTime)
{
    if (deltaTime <= 0)
        return 0.0f;
    const double dx = current.X - prev.X;
    const double dz = current.Z - prev.Z;
    return sqrt(dx * dx + dz * dz) / deltaTime;
}

void update_speed()
{
    using namespace std::chrono_literals;

    auto current_time = std::chrono::steady_clock::now();
    if (current_time - last_mumble_update < std::chrono::duration<double>{FIXED_DELTA_TIME})
        return;
    last_mumble_update = current_time;

    if (mumble_link != nullptr) {
        const Vector3 current_position = mumble_link->AvatarPosition;

        if (current_position.X == 0.0f && current_position.Y == 0.0f && current_position.Z == 0.0f) {
            zero_count++;
            if (zero_count <= 14) {
                return;
            }
            speed_3d = 0.0f;
            speed_2d = 0.0f;
        } else {
            double new_speed_3d = calculate_speed_3d(last_position, current_position, FIXED_DELTA_TIME);
            double new_speed_2d = calculate_speed_2d(last_position, current_position, FIXED_DELTA_TIME);

            if (new_speed_3d == 0.0f) {
                zero_count++;
                if (zero_count <= 4) {
                    new_speed_3d = last_speed_3d;
                    new_speed_2d = last_speed_2d;
                } else {
                    last_speed_3d = 0.0f;
                    last_speed_2d = 0.0f;
                }
            } else {
                zero_count = 0;
                last_speed_3d = new_speed_3d;
                last_speed_2d = new_speed_2d;
            }

            speed_3d = new_speed_3d;
            speed_2d = new_speed_2d;
        }

        last_position = current_position;
        last_time = current_time;
    }
}

void render_window()
{
    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;
    ImGui::SetNextWindowPos(ImVec2(300, 400), ImGuiCond_FirstUseEver);
    if (tmp_open && ImGui::Begin("Speed##SpeedIndicatorMainWindow", &tmp_open, flags)) {
        update_speed();
        ImGui::Text("Speed (u/s): %.2f", speed_3d * CONVERSION_FACTOR_U_S);
        ImGui::End();
    }
}

void render_options() {}
