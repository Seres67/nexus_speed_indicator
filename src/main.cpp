#include <globals.hpp>
#include <gui.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <nexus/Nexus.h>
#include <settings.hpp>

void addon_load(AddonAPI *api_p);
void addon_unload();
void addon_render();
void addon_options();

BOOL APIENTRY dll_main(const HMODULE hModule, const DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        self_module = hModule;
        break;
    case DLL_PROCESS_DETACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    default:
        break;
    }
    return TRUE;
}

// NOLINTNEXTLINE(readability-identifier-naming)
extern "C" __declspec(dllexport) AddonDefinition *GetAddonDef()
{
    addon_def.Signature = -273486234;
    addon_def.APIVersion = NEXUS_API_VERSION;
    addon_def.Name = addon_name;
    addon_def.Version.Major = 0;
    addon_def.Version.Minor = 2;
    addon_def.Version.Build = 0;
    addon_def.Version.Revision = 0;
    addon_def.Author = "Seres67";
    addon_def.Description = "Just displays your speed in an overlay (more features coming!)";
    addon_def.Load = addon_load;
    addon_def.Unload = addon_unload;
    addon_def.Flags = EAddonFlags_None;
    addon_def.Provider = EUpdateProvider_GitHub;
    addon_def.UpdateLink = "https://github.com/Seres67/nexus_speed_indicator";

    return &addon_def;
}

void addon_load(AddonAPI *api_p)
{
    api = api_p;

    ImGui::SetCurrentContext(static_cast<ImGuiContext *>(api->ImguiContext));
    ImGui::SetAllocatorFunctions((void *(*)(size_t, void *))(api->ImguiMalloc),
                                 (void (*)(void *, void *))(api->ImguiFree)); // on imgui 1.80+
    mumble_link = static_cast<Mumble::Data *>(api->DataLink.Get("DL_MUMBLE_LINK"));

    api->Renderer.Register(ERenderType_Render, addon_render);
    api->Renderer.Register(ERenderType_OptionsRender, addon_options);

    Settings::settings_path = api->Paths.GetAddonDirectory("speed_indicator\\settings.json");
    if (std::filesystem::exists(Settings::settings_path)) {
        Settings::load(Settings::settings_path);
    }
    speed_data.clear();
    api->Log(ELogLevel_INFO, addon_name, "addon loaded!");
}

void addon_unload()
{
    api->Log(ELogLevel_INFO, addon_name, "unloading addon...");
    speed_data.clear();
    api->Renderer.Deregister(addon_render);
    api->Renderer.Deregister(addon_options);
    api->Log(ELogLevel_INFO, addon_name, "addon unloaded!");
    mumble_link = nullptr;
    api = nullptr;
}

void addon_render() { render_window(); }

void addon_options() { render_options(); }
