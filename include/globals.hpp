#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <mumble/Mumble.h>
#include <nexus/Nexus.h>

// handle to self hmodule
extern HMODULE self_module;
// addon definition
extern AddonDefinition addon_def;
// addon api
extern AddonAPI *api;

extern Mumble::Data *mumble_link;

extern char addon_name[];

extern HWND game_handle;

#endif // GLOBALS_HPP
