#include "SpriteEditorTool.h"
#include "imgui.h"

#include "Textures.h"

SpriteEditorTool::SpriteEditorTool()
    : filepath{""}
{}

String SpriteEditorTool::get_menu_tool_name()
{
	return "Sprites";
}

void SpriteEditorTool::process_signal(core::EditorAssetSelectedIntent& selection)
{
    if (selection.path.find(".png") != selection.path.npos)
    {
        filepath = selection.path;
    }
}

namespace ImGui
{
    bool SelectionRect(ImVec2& start_pos, ImVec2& end_pos, ImGuiMouseButton mouse_button = ImGuiMouseButton_Left)
    {
        if (ImGui::IsMouseClicked(mouse_button))
            start_pos = ImGui::GetMousePos();
        if (ImGui::IsMouseDown(mouse_button))
        {
            end_pos = ImGui::GetMousePos();
            ImDrawList* draw_list = ImGui::GetForegroundDrawList(); //ImGui::GetWindowDrawList();
            draw_list->AddRect(start_pos, end_pos, ImGui::GetColorU32(IM_COL32(0, 130, 216, 255)));   // Border
            draw_list->AddRectFilled(start_pos, end_pos, ImGui::GetColorU32(IM_COL32(0, 130, 216, 50)));    // Background
        }
        return ImGui::IsMouseReleased(mouse_button);
    }
}

void SpriteEditorTool::show_tool_window()
{
    static ImVec2 start;
    static ImVec2 end;

    if (filepath != "")
    {
        auto texture = AccessModule<core::AssetModule>::access_module()->get_asset<Texture>(filepath);
        if (texture == ecs::no_entity) return;

        auto& textures = AccessStorage<Texture>::access_storage();

        auto preview = textures.get<Texture>(texture);
        SDL_Point preview_size;
        SDL_QueryTexture(preview.inner, nullptr, nullptr, &preview_size.x, &preview_size.y);

        auto size = ImGui::GetWindowSize();
        size.x -= 20;
        size.y -= 20;
        size.y = preview_size.y * size.x / preview_size.x;
        ImGui::Image(preview.inner, size);

        if (ImGui::SelectionRect(start, end, ImGuiMouseButton_Left))
        {
            Logger::info("SELECTED {}, {} => {}, {}", start.x, start.y, end.x, end.y);
        }
    }
}