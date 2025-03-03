#include "settings.hpp"
#include "nlohmann/json.hpp"
#include <filesystem>
#include <fstream>
#include <globals.hpp>

#include <nexus/Nexus.h>

using json = nlohmann::json;
namespace Settings
{
json json_settings;
std::mutex mutex;
std::filesystem::path settings_path;

bool ups_3d = true;
bool kmph_3d = false;
bool mph_3d = false;
bool ups_2d = false;
bool kmph_2d = false;
bool mph_2d = false;

const char UPS_3D[] = "UnitsPerSecond3D";
const char KMPH_3D[] = "KilometersPerHour3D";
const char MPH_3D[] = "MilesPerHour3D";
const char UPS_2D[] = "UnitsPerSecond2D";
const char KMPH_2D[] = "KilometersPerHour2D";
const char MPH_2D[] = "MilesPerHour2D";

void load(const std::filesystem::path &path)
{
    json_settings = json::object();
    if (!std::filesystem::exists(path)) {
        return;
    }

    {
        std::lock_guard lock(mutex);
        try {
            if (std::ifstream file(path); file.is_open()) {
                json_settings = json::parse(file);
                file.close();
            }
        } catch (json::parse_error &ex) {
            api->Log(ELogLevel_WARNING, addon_name, "settings.json could not be parsed.");
            api->Log(ELogLevel_WARNING, addon_name, ex.what());
        }
    }
    if (!json_settings[UPS_3D].is_null()) {
        json_settings[UPS_3D].get_to(ups_3d);
    }
    if (!json_settings[KMPH_3D].is_null()) {
        json_settings[MPH_3D].get_to(kmph_3d);
    }
    if (!json_settings[MPH_3D].is_null()) {
        json_settings[MPH_3D].get_to(mph_3d);
    }
    if (!json_settings[UPS_2D].is_null()) {
        json_settings[UPS_2D].get_to(ups_2d);
    }
    if (!json_settings[KMPH_2D].is_null()) {
        json_settings[MPH_2D].get_to(kmph_2d);
    }
    if (!json_settings[MPH_2D].is_null()) {
        json_settings[MPH_2D].get_to(mph_2d);
    }
    api->Log(ELogLevel_INFO, addon_name, "settings loaded!");
}

void save(const std::filesystem::path &path)
{
    if (!std::filesystem::exists(path.parent_path())) {
        std::filesystem::create_directories(path.parent_path());
    }
    {
        std::lock_guard lock(mutex);
        if (std::ofstream file(path); file.is_open()) {
            file << json_settings.dump(1, '\t') << std::endl;
            file.close();
        }
        api->Log(ELogLevel_INFO, addon_name, "settings saved!");
    }
}
} // namespace Settings
