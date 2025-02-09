#include <globals.hpp>

HMODULE self_module = nullptr;
AddonDefinition addon_def{};
AddonAPI *api = nullptr;
char addon_name[] = "Speed Indicator";
Mumble::Data *mumble_link;
HWND game_handle = nullptr;
