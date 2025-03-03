#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <mutex>
#include <nlohmann/json.hpp>

namespace Settings
{

void load(const std::filesystem::path &path);
void save(const std::filesystem::path &path);

extern nlohmann::json json_settings;
extern std::filesystem::path settings_path;
extern std::mutex mutex;

extern bool ups_3d;
extern bool kmph_3d;
extern bool mph_3d;
extern bool ups_2d;
extern bool kmph_2d;
extern bool mph_2d;
} // namespace Settings

#endif // SETTINGS_HPP
