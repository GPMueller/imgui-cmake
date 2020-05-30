#include <imgui_impl/cousine_regular.h>
#include <imgui_impl/fonts.hpp>

#include <imgui/imgui.h>

namespace fonts
{

ImFont * default()
{
    ImGuiIO & io = ImGui::GetIO();
    return io.Fonts->AddFontDefault();
}

ImFont * cousine_regular()
{
    static const ImWchar ranges[] = { 0x0020, 0x00FF, 0x0100, 0x017F, 0 };

    ImGuiIO & io = ImGui::GetIO();
    return io.Fonts->AddFontFromMemoryCompressedTTF(
        cousine_regular_compressed_data, cousine_regular_compressed_size, 14.0f, NULL, ranges );
}

} // namespace fonts