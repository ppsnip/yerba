#include "imgui_addons.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_particles.h"

#include <map>
#include <string>
#include <cmath>

using namespace ImGui;
using std::string;

ImVec4 ImAdd::HexToColorVec4(unsigned int hex_color, float alpha)
{
    ImVec4 color;

    color.x = ((hex_color >> 16) & 0xFF) / 255.0f;
    color.y = ((hex_color >> 8) & 0xFF) / 255.0f;
    color.z = (hex_color & 0xFF) / 255.0f;
    color.w = alpha;

    return color;
}

bool ImAdd::Label(const char* label, const ImVec2& size_arg, const ImVec2& text_align)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, label_size.x, label_size.y);

    const ImRect bb(pos, pos + size);
    ItemSize(size);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    RenderNavHighlight(bb, id);
    //    RenderFrame(bb.Min, bb.Max, IM_COL32(255, 0, 0, 255), false);
    RenderTextClipped(bb.Min, bb.Max, label, NULL, &label_size, text_align, &bb);

    return pressed;
}

void ImAdd::DoubleText(ImVec4 color1, ImVec4 color2, const char* label1, const char* label2)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(std::string(label1 + std::string(label2)).c_str());
    const ImVec2 label1_size = CalcTextSize(label1, NULL, true);
    const ImVec2 label2_size = CalcTextSize(label2, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(ImVec2(-0.1f, g.FontSize), label1_size.x + label2_size.x, g.FontSize);

    const ImRect total_bb(pos, pos + size);
    ItemSize(total_bb);
    if (!ItemAdd(total_bb, id)) {
        return;
    }

    window->DrawList->AddText(pos, GetColorU32(color1), label1);
    window->DrawList->AddText(pos + ImVec2(size.x - ImGui::CalcTextSize(label2).x, 0), GetColorU32(color2), label2);
}

void ImAdd::SeparatorText(const char* label, float thickness)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(ImVec2(-0.1f, g.FontSize), label_size.x, g.FontSize);

    const ImRect total_bb(pos, pos + size);
    ItemSize(total_bb);
    if (!ItemAdd(total_bb, id)) {
        return;
    }

    window->DrawList->AddText(pos, GetColorU32(ImGuiCol_TextDisabled), label);

    if (thickness > 0)
        window->DrawList->AddLine(pos + ImVec2(label_size.x + style.ItemInnerSpacing.x, size.y / 2), pos + ImVec2(size.x, size.y / 2), GetColorU32(ImGuiCol_Border), thickness);
}

void ImAdd::VSeparator(float margin, float thickness)
{
    if (thickness <= 0)
        return;

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(ImVec2(thickness, -0.1f), thickness, thickness);

    const ImRect bb(pos, pos + size);
    const ImRect bb_rect(pos + ImVec2(0, margin), pos + size - ImVec2(0, margin));

    ItemSize(ImVec2(thickness, 0.0f));
    if (!ItemAdd(bb, 0))
        return;

    //window->DrawList->AddRectFilled(bb_rect.Min, bb_rect.Max, GetColorU32(ImGuiCol_Border));

    window->DrawList->AddRectFilledMultiColor(pos + ImVec2(0, margin), pos + ImVec2(size.x, size.y / 2), GetColorU32(ImGuiCol_SliderGrabActive), GetColorU32(ImGuiCol_SliderGrabActive), GetColorU32(ImGuiCol_SliderGrab), GetColorU32(ImGuiCol_SliderGrab));
    window->DrawList->AddRectFilledMultiColor(pos + ImVec2(0, size.y / 2), pos + ImVec2(size.x, size.y - margin), GetColorU32(ImGuiCol_SliderGrab), GetColorU32(ImGuiCol_SliderGrab), GetColorU32(ImGuiCol_SliderGrabActive), GetColorU32(ImGuiCol_SliderGrabActive));
}

bool ImAdd::RadioFrame(const char* label, int* v, int radio_id, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect total_bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
        return false;

    bool hovered, held, active;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    active = *v == radio_id;
    if (pressed) {
        *v = radio_id;
    }

    bool has_border = style.FrameBorderSize > 0;

    // Colors
    ImVec4 colFrameMain = GetStyleColorVec4(ImGuiCol_Header);
	ImVec4 colFrameNull = colFrameMain; colFrameNull.w = 0.0f;
	ImVec4 colFrame = (active ? colFrameMain : colFrameNull);

	ImVec4 colBorderMain = GetStyleColorVec4(ImGuiCol_Border);
	ImVec4 colBorderNull = colBorderMain; colBorderNull.w = 0.0f;
	ImVec4 colBorder = (active ? colBorderMain : colBorderNull);

    ImVec4 colLabel = GetStyleColorVec4((active || hovered) ? ImGuiCol_Text : ImGuiCol_TextDisabled);

    // Animation
    struct stColors_State {
        ImVec4 Frame;
        ImVec4 Border;
        ImVec4 Label;
    };

    static std::map<ImGuiID, stColors_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, stColors_State() });
        it_anim = anim.find(id);

        it_anim->second.Frame = colFrame;
        it_anim->second.Border = colBorder;
        it_anim->second.Label = colLabel;
    }

    it_anim->second.Frame = ImLerp(it_anim->second.Frame, colFrame, 1.0f / IMADD_ANIMATIONS_SPEED * ImGui::GetIO().DeltaTime);
    if (has_border) {    // good for preformance
        it_anim->second.Border = ImLerp(it_anim->second.Border, colBorder, 1.0f / IMADD_ANIMATIONS_SPEED * ImGui::GetIO().DeltaTime);
    }
    it_anim->second.Label = ImLerp(it_anim->second.Label, colLabel, 1.0f / IMADD_ANIMATIONS_SPEED * ImGui::GetIO().DeltaTime);

    // Render
    RenderNavHighlight(total_bb, id);

    window->DrawList->AddRectFilled(total_bb.Min, total_bb.Max, GetColorU32(it_anim->second.Frame), style.TabRounding);
    if (has_border) {
        window->DrawList->AddRect(total_bb.Min, total_bb.Max, GetColorU32(it_anim->second.Border), style.TabRounding, 0, style.FrameBorderSize);
    }

    window->DrawList->AddText(pos + ImVec2(size.x / 2 - label_size.x / 2, size.y / 2 - label_size.y / 2), GetColorU32(it_anim->second.Label), label);

    return pressed;
}

bool ImAdd::RadioFrameIcon(ImTextureID icon_texture, const char* label, int* v, int radio_id, const ImVec2& size_arg, const ImVec2& icon_padding)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    bool has_icon = icon_texture != 0;
    bool has_border = style.FrameBorderSize > 0;

    const ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, has_icon ? GetFrameHeight() + label_size.x + style.FramePadding.x : label_size.x + style.FramePadding.x * 2, GetFrameHeight());

    const ImRect total_bb(pos, pos + size);
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
    {
        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
        return false;
    }

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
    {
        *v = radio_id;
        MarkItemEdited(id);
    }
    bool active = *v == radio_id;

    // Colors
    ImVec4 colFrameMain = GetStyleColorVec4(ImGuiCol_Header);
    ImVec4 colFrameNull = colFrameMain; colFrameNull.w = 0.0f;
    ImVec4 colFrame = (active ? colFrameMain : colFrameNull);
    
    ImVec4 colBorderMain = GetStyleColorVec4(ImGuiCol_Border);
    ImVec4 colBorderNull = colFrameMain; colBorderNull.w = 0.0f;
    ImVec4 colBorder = (active ? colBorderMain : colBorderNull);

    ImVec4 colLabel = GetStyleColorVec4((active || hovered) ? ImGuiCol_Text : ImGuiCol_TextDisabled);
    ImVec4 colIcon = GetStyleColorVec4((hovered && !held && !active) ? ImGuiCol_Text : active ? ImGuiCol_SliderGrab : ImGuiCol_TextDisabled);

    // Animation
    struct stColors_State {
        ImVec4 Frame;
        ImVec4 Border;
        ImVec4 Label;
        ImVec4 Icon;
    };

    static std::map<ImGuiID, stColors_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, stColors_State() });
        it_anim = anim.find(id);

        it_anim->second.Frame = colFrame;
        it_anim->second.Border = colBorder;
        it_anim->second.Label = colLabel;
        it_anim->second.Icon = colIcon;
    }

    it_anim->second.Frame = ImLerp(it_anim->second.Frame, colFrame, 1.0f / IMADD_ANIMATIONS_SPEED * ImGui::GetIO().DeltaTime);
    if (has_border) {    // good for preformance
        it_anim->second.Border = ImLerp(it_anim->second.Border, colBorder, 1.0f / IMADD_ANIMATIONS_SPEED * ImGui::GetIO().DeltaTime);
    }
    it_anim->second.Label = ImLerp(it_anim->second.Label, colLabel, 1.0f / IMADD_ANIMATIONS_SPEED * ImGui::GetIO().DeltaTime);
    it_anim->second.Icon = ImLerp(it_anim->second.Icon, colIcon, 1.0f / IMADD_ANIMATIONS_SPEED * ImGui::GetIO().DeltaTime);

    RenderNavHighlight(total_bb, id);

    window->DrawList->AddRectFilled(total_bb.Min, total_bb.Max, GetColorU32(it_anim->second.Frame), style.TabRounding);
    if (has_border) {
        window->DrawList->AddRect(total_bb.Min, total_bb.Max, GetColorU32(it_anim->second.Border), style.TabRounding, 0, style.FrameBorderSize);
    }

    if (has_icon) {
        window->DrawList->AddImage(icon_texture, pos + icon_padding, pos + ImVec2(size.y, size.y) - icon_padding, ImVec2(), ImVec2(1, 1), GetColorU32(it_anim->second.Icon));
    }

    if (label_size.x > 0.0f) {
        PushStyleColor(ImGuiCol_Text, it_anim->second.Label);
        RenderText(has_icon ? pos + ImVec2(GetFrameHeight(), style.FramePadding.y) : pos + style.FramePadding, label);
        PopStyleColor();
    }

    return pressed;
}

bool ImAdd::RadioFrameText(const char* label, int* v, int radio_id, bool underline, bool border, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, label_size.x, label_size.y);

    const ImRect bb(pos, pos + size);
    ItemSize(size);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held, active;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);
    active = *v == radio_id;
    if (pressed) {
        *v = radio_id;
    }

    // Colors
    ImVec4 colLabel = GetStyleColorVec4(active ? ImGuiCol_Text : (hovered && !held) ? ImGuiCol_SliderGrab : ImGuiCol_TextDisabled);

    ImVec4 colUnderline1Main = GetStyleColorVec4(ImGuiCol_SliderGrab);
    ImVec4 colUnderline1Null = colUnderline1Main; colUnderline1Null.w = 0.0f;
    ImVec4 colUnderline1 = (active ? colUnderline1Main : colUnderline1Null);
    
    ImVec4 colUnderline2Main = GetStyleColorVec4(ImGuiCol_SliderGrabActive);
    ImVec4 colUnderline2Null = colUnderline2Main; colUnderline2Null.w = 0.0f;
    ImVec4 colUnderline2 = (active ? colUnderline2Main : colUnderline2Null);

    // Animations
    struct stColors_State {
        ImVec4 Label;
        ImVec4 Underline1;
        ImVec4 Underline2;
    };

    static std::map<ImGuiID, stColors_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, stColors_State() });
        it_anim = anim.find(id);

        it_anim->second.Label = colLabel;
        it_anim->second.Underline1 = colUnderline1;
        it_anim->second.Underline2 = colUnderline2;
    }

    it_anim->second.Label = ImLerp(it_anim->second.Label, colLabel, 1.0f / IMADD_ANIMATIONS_SPEED * GetIO().DeltaTime);
    it_anim->second.Underline1 = ImLerp(it_anim->second.Underline1, colUnderline1, 1.0f / IMADD_ANIMATIONS_SPEED * GetIO().DeltaTime);
    it_anim->second.Underline2 = ImLerp(it_anim->second.Underline2, colUnderline2, 1.0f / IMADD_ANIMATIONS_SPEED * GetIO().DeltaTime);

    // Render
    RenderNavHighlight(bb, id);

    if (border) {
        window->DrawList->AddText(pos + ImVec2(size.x / 2 - label_size.x / 2, size.y / 2 - label_size.y / 2 + 1) + ImVec2(-1, -1), IM_COL32_BLACK, label);
        window->DrawList->AddText(pos + ImVec2(size.x / 2 - label_size.x / 2, size.y / 2 - label_size.y / 2 + 1) + ImVec2(-1, 1), IM_COL32_BLACK, label);
        window->DrawList->AddText(pos + ImVec2(size.x / 2 - label_size.x / 2, size.y / 2 - label_size.y / 2 + 1) + ImVec2(1, -1), IM_COL32_BLACK, label);
        window->DrawList->AddText(pos + ImVec2(size.x / 2 - label_size.x / 2, size.y / 2 - label_size.y / 2 + 1) + ImVec2(1, 1), IM_COL32_BLACK, label);
    }

    window->DrawList->AddText(pos + ImVec2(size.x / 2 - label_size.x / 2, size.y / 2 - label_size.y / 2 + 1), GetColorU32(it_anim->second.Label), label);

    if (underline) 
    {
        //window->DrawList->AddLine(pos + ImVec2(-1, label_size.y + style.FrameBorderSize * 2), pos + ImVec2(label_size.x, label_size.y + style.FrameBorderSize * 2), ImGui::GetColorU32(it_anim->second.Underline), style.FrameBorderSize);
        window->DrawList->AddRectFilledMultiColor(pos + ImVec2(-1, label_size.y + style.FrameBorderSize * 2), pos + ImVec2(label_size.x / 2, label_size.y + style.FrameBorderSize * 4), ImGui::GetColorU32(it_anim->second.Underline2), ImGui::GetColorU32(it_anim->second.Underline1), ImGui::GetColorU32(it_anim->second.Underline1), ImGui::GetColorU32(it_anim->second.Underline2));
        window->DrawList->AddRectFilledMultiColor(pos + ImVec2(label_size.x / 2, label_size.y + style.FrameBorderSize * 2), pos + ImVec2(label_size.x + 1, label_size.y + style.FrameBorderSize * 4), ImGui::GetColorU32(it_anim->second.Underline1), ImGui::GetColorU32(it_anim->second.Underline2), ImGui::GetColorU32(it_anim->second.Underline2), ImGui::GetColorU32(it_anim->second.Underline1));
    }

    return pressed;
}

bool ImAdd::LinkLabel(const char* label, bool underlined, const ImVec2& size_arg, const ImVec2& text_align, ImGuiButtonFlags button_flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, label_size.x, label_size.y);

    const ImRect bb(pos, pos + size);
    ItemSize(size);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);
    bool has_border = style.FrameBorderSize > 0;

    // Colors
    ImVec4 colText = GetStyleColorVec4((held && hovered) ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab);

    // Animations
    struct stColors_State {
        ImVec4 Text;
    };

    static std::map<ImGuiID, stColors_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, stColors_State() });
        it_anim = anim.find(id);

        it_anim->second.Text = colText;
    }

    it_anim->second.Text = ImLerp(it_anim->second.Text, colText, 1.0f / IMADD_ANIMATIONS_SPEED * GetIO().DeltaTime);

    // Render
    RenderNavHighlight(bb, id);

    PushStyleColor(ImGuiCol_Text, it_anim->second.Text);
    RenderTextClipped(bb.Min, bb.Max, label, NULL, &label_size, text_align, &bb);
    PopStyleColor();

    if (underlined)
    {
        window->DrawList->AddLine(bb.Min + ImVec2(0, label_size.y - 1), bb.Min + ImVec2(bb.Max.x - bb.Min.x, label_size.y - 1), GetColorU32(it_anim->second.Text));
    }

    return pressed;
}

bool ImAdd::Button(const char* label, const ImVec2& size_arg, ImGuiButtonFlags button_flags, ImDrawFlags draw_flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    if ((button_flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);
    bool has_border = style.FrameBorderSize > 0;

    // Colors
    ImVec4 colFrame = GetStyleColorVec4((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);

    // Animations
    struct stColors_State {
        ImVec4 Frame;
    };

    static std::map<ImGuiID, stColors_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, stColors_State() });
        it_anim = anim.find(id);

        it_anim->second.Frame = colFrame;
    }

    it_anim->second.Frame = ImLerp(it_anim->second.Frame, colFrame, 1.0f / IMADD_ANIMATIONS_SPEED * GetIO().DeltaTime);

    // Render
    RenderNavHighlight(bb, id);

    window->DrawList->AddRectFilledMultiColorRounded(bb.Min, bb.Max, GetColorU32(ImGuiCol_ButtonHovered), GetColorU32(ImGuiCol_ButtonHovered), GetColorU32(it_anim->second.Frame), GetColorU32(it_anim->second.Frame), style.FrameRounding, draw_flags);
    if (has_border)
    {
        window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(ImGuiCol_Border), style.FrameRounding, draw_flags, style.FrameBorderSize);
    }

    std::string label_str(label);
    size_t cut_pos = label_str.find("##");
    if (cut_pos != std::string::npos)
    {
        label_str = label_str.substr(0, cut_pos);
    }

    window->DrawList->AddText(pos + size / 2 - label_size / 2 + ImVec2(-1, -1), IM_COL32(0, 0, 0, style.Alpha * 255), label_str.c_str());
    window->DrawList->AddText(pos + size / 2 - label_size / 2 + ImVec2(1, -1), IM_COL32(0, 0, 0, style.Alpha * 255), label_str.c_str());
    window->DrawList->AddText(pos + size / 2 - label_size / 2 + ImVec2(-1, 1), IM_COL32(0, 0, 0, style.Alpha * 255), label_str.c_str());
    window->DrawList->AddText(pos + size / 2 - label_size / 2 + ImVec2(1, 1), IM_COL32(0, 0, 0, style.Alpha * 255), label_str.c_str());
    window->DrawList->AddText(pos + size / 2 - label_size / 2, GetColorU32(ImGuiCol_Text), label_str.c_str());

    return pressed;
}

bool ImAdd::ButtonLabel(const char* label, const ImVec2& size_arg, ImGuiButtonFlags button_flags, ImDrawFlags draw_flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, label_size.x, label_size.y);

    const ImRect bb(pos, pos + size);
    ItemSize(size);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);
    bool has_border = style.FrameBorderSize > 0;

    // Colors
    ImVec4 colFrame = GetStyleColorVec4((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);

    // Animations
    struct stColors_State {
        ImVec4 Frame;
    };

    static std::map<ImGuiID, stColors_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, stColors_State() });
        it_anim = anim.find(id);

        it_anim->second.Frame = colFrame;
    }

    it_anim->second.Frame = ImLerp(it_anim->second.Frame, colFrame, 1.0f / IMADD_ANIMATIONS_SPEED * GetIO().DeltaTime);

    // Render
    RenderNavHighlight(bb, id);

    std::string label_str(label);
    size_t cut_pos = label_str.find("##");
    if (cut_pos != std::string::npos)
    {
        label_str = label_str.substr(0, cut_pos);
    }

    window->DrawList->AddText(pos + size / 2 - label_size / 2 + ImVec2(-1, -1), IM_COL32(0, 0, 0, style.Alpha * 255), label_str.c_str());
    window->DrawList->AddText(pos + size / 2 - label_size / 2 + ImVec2(1, -1), IM_COL32(0, 0, 0, style.Alpha * 255), label_str.c_str());
    window->DrawList->AddText(pos + size / 2 - label_size / 2 + ImVec2(-1, 1), IM_COL32(0, 0, 0, style.Alpha * 255), label_str.c_str());
    window->DrawList->AddText(pos + size / 2 - label_size / 2 + ImVec2(1, 1), IM_COL32(0, 0, 0, style.Alpha * 255), label_str.c_str());
    window->DrawList->AddText(pos + size / 2 - label_size / 2, GetColorU32(ImGuiCol_Text), label_str.c_str());

    return pressed;
}

bool ImAdd::ButtonIcon(const char* label, ImTextureID icon_texture, const ImVec2& size_arg, ImGuiButtonFlags button_flags, ImDrawFlags draw_flags, bool background)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

	bool has_icon = icon_texture != 0;
	bool has_label = label_size.x > 0.0f;

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, (has_icon ? g.FontSize + style.FramePadding.x : 0.0f) + (has_label ? label_size.x + style.FramePadding.x : 0.0f) + style.FramePadding.x, GetFrameHeight());

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);
    bool has_border = style.FrameBorderSize > 0;

    // Colors
    ImVec4 colFrame = GetStyleColorVec4((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
    ImVec4 colIcon = GetStyleColorVec4((hovered && !held) ? ImGuiCol_Text : ImGuiCol_TextDisabled);

    // Animations
    struct stColors_State {
        ImVec4 Frame;
        ImVec4 Icon;
    };

    static std::map<ImGuiID, stColors_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, stColors_State() });
        it_anim = anim.find(id);

        it_anim->second.Frame = colFrame;
        it_anim->second.Icon = colIcon;
    }

    it_anim->second.Frame = ImLerp(it_anim->second.Frame, colFrame, 1.0f / IMADD_ANIMATIONS_SPEED * GetIO().DeltaTime);
    it_anim->second.Icon = ImLerp(it_anim->second.Icon, colIcon, 1.0f / IMADD_ANIMATIONS_SPEED * GetIO().DeltaTime);

    // Render
    RenderNavHighlight(bb, id);

    if (background)
    {
        window->DrawList->AddRectFilledMultiColorRounded(bb.Min, bb.Max, GetColorU32(ImGuiCol_ButtonHovered), GetColorU32(ImGuiCol_ButtonHovered), GetColorU32(it_anim->second.Frame), GetColorU32(it_anim->second.Frame), style.FrameRounding, draw_flags);
        if (has_border)
        {
            window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(ImGuiCol_Border), style.FrameRounding, draw_flags, style.FrameBorderSize);
        }
    }

    if (has_label)
    {
        window->DrawList->AddText(pos + ImVec2(has_icon ? size.y : style.FramePadding.x, style.FramePadding.y), GetColorU32(it_anim->second.Icon), label);
    }

    if (has_icon)
    {
        window->DrawList->AddImage(icon_texture, pos + style.FramePadding, pos + ImVec2(size.y, size.y) - style.FramePadding, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(it_anim->second.Icon));
    }

    return pressed;
}

bool ImAdd::SelectableLabel(ImDrawList* drawlist, const char* label, bool selected, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, label_size.x, label_size.y);

    const ImRect bb(pos, pos + size);
    ItemSize(size);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);
    bool has_border = style.FrameBorderSize > 0;

    // Colors
    ImVec4 colText = GetStyleColorVec4(selected ? ImGuiCol_SliderGrab : (hovered && !held) ? ImGuiCol_Text : ImGuiCol_TextDisabled);

    // Animations
    struct stColors_State {
        ImVec4 Text;
    };

    static std::map<ImGuiID, stColors_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, stColors_State() });
        it_anim = anim.find(id);

        it_anim->second.Text = colText;
    }

    it_anim->second.Text = ImLerp(it_anim->second.Text, colText, 1.0f / IMADD_ANIMATIONS_SPEED * GetIO().DeltaTime);

    // Render
    RenderNavHighlight(bb, id);

    drawlist->AddText(pos, GetColorU32(it_anim->second.Text), label);

    return pressed;
}

bool ImAdd::CheckBox(const char* label, bool* v)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const float square_sz   = GetFontSize();
    float width             = CalcItemWidth();
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect check_bb(pos + ImVec2(width - square_sz, 0), pos + ImVec2(width, square_sz));
    const ImRect total_bb(pos, pos + ImVec2(width, square_sz));
    ItemSize(total_bb);

    if (!ItemAdd(total_bb, id))
    {
        return false;
    }

    //const bool is_multi_select = (g.LastItemData.InFlags & ImGuiItemFlags_IsMultiSelect) != 0;
    const bool has_border = style.FrameBorderSize > 0;

    // Range-Selection/Multi-selection support (header)
    bool checked = *v;
    //if (is_multi_select)
    //    MultiSelectItemHeader(id, &checked, NULL);

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);

    if (pressed) {
        checked = !checked;
    }

    if (*v != checked)
    {
        *v = checked;
        pressed = true; // return value
        MarkItemEdited(id);
    }

    // Colors
    ImVec4 colFrame = GetStyleColorVec4(checked ? ImGuiCol_SliderGrabActive : (held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBg : ImGuiCol_FrameBgHovered);
    ImVec4 colBorder = GetStyleColorVec4(checked || (hovered && !held) ? ImGuiCol_SliderGrab : ImGuiCol_Border);

    ImVec4 colCheckMarkMain = GetStyleColorVec4(ImGuiCol_CheckMark);
    ImVec4 colCheckMarkNull = colCheckMarkMain; colCheckMarkNull.w = 0.0f;
    ImVec4 colCheckMark = (checked ? colCheckMarkMain : colCheckMarkNull);

    // Animations
    struct stColors_State {
        ImVec4 Frame;
        ImVec4 Border;
        ImVec4 CheckMark;
    };

    static std::map<ImGuiID, stColors_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, stColors_State() });
        it_anim = anim.find(id);

        it_anim->second.Frame = colFrame;
        it_anim->second.Border = colBorder;
        it_anim->second.CheckMark = colCheckMark;
    }

    it_anim->second.Frame = ImLerp(it_anim->second.Frame, colFrame, 1.0f / IMADD_ANIMATIONS_SPEED * GetIO().DeltaTime);
    it_anim->second.Border = ImLerp(it_anim->second.Border, colBorder, 1.0f / IMADD_ANIMATIONS_SPEED * GetIO().DeltaTime);
    it_anim->second.CheckMark = ImLerp(it_anim->second.CheckMark, colCheckMark, 1.0f / IMADD_ANIMATIONS_SPEED * GetIO().DeltaTime);

    RenderNavHighlight(total_bb, id);

    //window->DrawList->AddRectFilled(check_bb.Min, check_bb.Max, GetColorU32(it_anim->second.Frame), style.FrameRounding);
    window->DrawList->AddRectFilledMultiColorRounded(check_bb.Min, check_bb.Max, GetColorU32(ImGuiCol_PopupBg), GetColorU32(ImGuiCol_PopupBg), GetColorU32(it_anim->second.Frame), GetColorU32(it_anim->second.Frame), style.FrameRounding);
    if (has_border)
    {
        window->DrawList->AddRect(check_bb.Min, check_bb.Max, GetColorU32(it_anim->second.Border), style.FrameRounding, 0, style.FrameBorderSize);
    }

    //const float pad = ImMax(1.0f, IM_TRUNC(square_sz / 3.2));
    //RenderCheckMark(window->DrawList, check_bb.Min + ImVec2(pad, pad), GetColorU32(it_anim->second.CheckMark), square_sz - pad * 2.0f);

    if (label_size.x > 0.0f) {
        PushStyleColor(ImGuiCol_Text, IM_COL32_BLACK);
        RenderText(total_bb.Min + ImVec2(-1, -1), label);
        RenderText(total_bb.Min + ImVec2(-1, 1), label);
        RenderText(total_bb.Min + ImVec2(1, -1), label);
        RenderText(total_bb.Min + ImVec2(1, 1), label);
        PopStyleColor();
        RenderText(total_bb.Min, label);
    }

    return pressed;
}

bool ImAdd::SliderScalar(const char* str_id, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(str_id);
    const float width = CalcItemWidth();
    const ImVec2 pos = window->DC.CursorPos;

    const float slider_height = g.FontSize;
    const float frame_height = style.GrabMinSize - 2.0f;
    const float frame_pad = (slider_height - frame_height) / 2;

    const ImRect total_bb(pos, pos + ImVec2(width, frame_height));
    const ImRect frame_bb(pos + ImVec2(0, frame_pad), pos + ImVec2(width, slider_height - frame_pad));

    ItemSize(total_bb);
    if (!ItemAdd(total_bb, id, &frame_bb, 0))
        return false;

    if (format == NULL)
        format = DataTypeGetInfo(data_type)->PrintFmt;

    const bool hovered = ItemHoverable(frame_bb, id, g.LastItemData.ItemFlags);
    //const bool hovered = ItemHoverable(frame_bb, id , g.LastItemData.InFlags);
    const bool clicked = hovered && IsMouseClicked(0, ImGuiInputFlags_None, id);
    const bool held = g.ActiveId == id;
    const bool make_active = (clicked || g.NavActivateId == id);
    if (make_active)
    {
        SetActiveID(id, window);
        SetFocusID(id, window);
        FocusWindow(window);
        g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
    }

    // Colors
    ImVec4 colFrame = GetStyleColorVec4(held ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
    ImVec4 colLine = GetStyleColorVec4(held ? ImGuiCol_SliderGrab : ImGuiCol_SliderGrabActive);

    // Animations
    struct stColors_State {
        ImVec4 Frame;
        ImVec4 Line;
    };

    static std::map<ImGuiID, stColors_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, stColors_State() });
        it_anim = anim.find(id);

        it_anim->second.Frame = colFrame;
        it_anim->second.Line = colLine;
    }

    it_anim->second.Frame = ImLerp(it_anim->second.Frame, colFrame, 1.0f / IMADD_ANIMATIONS_SPEED * GetIO().DeltaTime);
    it_anim->second.Line = ImLerp(it_anim->second.Line, colLine, 1.0f / IMADD_ANIMATIONS_SPEED * GetIO().DeltaTime);

    // Slider behavior
    ImRect grab_bb;
    const bool value_changed = SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);
    if (value_changed)
        MarkItemEdited(id);

    grab_bb.Min.x = grab_bb.Max.x - style.GrabMinSize;

    const ImVec2 grab_padding = ImVec2(grab_bb.Min.y - frame_bb.Min.y, grab_bb.Min.y - frame_bb.Min.y);

    // Draw frame
    RenderNavHighlight(frame_bb, id);
    RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(it_anim->second.Frame), false, g.Style.FrameRounding);

    // Render grab
    if (grab_bb.Max.x > grab_bb.Min.x) {
        window->DrawList->AddRectFilled(frame_bb.Min, ImVec2(grab_bb.Max.x + grab_padding.x, frame_bb.Max.y), GetColorU32(it_anim->second.Line), style.FrameRounding);
        //window->DrawList->AddRectFilled(ImVec2(grab_bb.Min.x, frame_bb.Min.y), ImVec2(grab_bb.Max.x, frame_bb.Max.y), GetColorU32(ImGuiCol_Text), style.GrabRounding);
    }

    if (style.FrameBorderSize)
    {
        window->DrawList->AddRect(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_SliderGrab), style.FrameRounding, 0, style.FrameBorderSize);
    }

    char value_buf[64];
    const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);

    ImVec2 value_size = CalcTextSize(value_buf);

    window->DrawList->AddRectFilled(pos + ImVec2(width / 2 - value_size.x / 2 - style.CellPadding.x, 0.0f), pos + ImVec2(width / 2 + value_size.x / 2 + style.CellPadding.x, slider_height), GetColorU32(ImGuiCol_ChildBg), style.FrameRounding);
    if (style.WindowBorderSize > 0)
    {
        window->DrawList->AddRect(pos + ImVec2(width / 2 - value_size.x / 2 - style.CellPadding.x, 0.0f), pos + ImVec2(width / 2 + value_size.x / 2 + style.CellPadding.x, slider_height), GetColorU32(ImGuiCol_Border), style.FrameRounding);
    }

    window->DrawList->AddText(total_bb.Min + ImVec2(width / 2 - value_size.x / 2, 0) + ImVec2(-1, -1), IM_COL32_BLACK, value_buf);
    window->DrawList->AddText(total_bb.Min + ImVec2(width / 2 - value_size.x / 2, 0) + ImVec2(-1, 1), IM_COL32_BLACK, value_buf);
    window->DrawList->AddText(total_bb.Min + ImVec2(width / 2 - value_size.x / 2, 0) + ImVec2(1, -1), IM_COL32_BLACK, value_buf);
    window->DrawList->AddText(total_bb.Min + ImVec2(width / 2 - value_size.x / 2, 0) + ImVec2(1, 1), IM_COL32_BLACK, value_buf);
    window->DrawList->AddText(total_bb.Min + ImVec2(width / 2 - value_size.x / 2, 0), GetColorU32(ImGuiCol_TextDisabled), value_buf);

    return value_changed;
}

bool ImAdd::SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    BeginGroup();

    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, style.ItemInnerSpacing.y));
    Text(label);
    SameLine(CalcItemWidth() - ImGui::GetFontSize() * 2);
    if (ImAdd::ButtonLabel(std::string("+##" + std::string(label)).c_str(), ImVec2(GetFontSize(), GetFontSize())))
    {
        if (*v + 1 <= v_max) *v = *v + 1;
    }

    ImGui::SameLine();
    if (ImAdd::ButtonLabel(std::string("-##" + std::string(label)).c_str(), ImVec2(GetFontSize(), GetFontSize())))
    {
        if (*v - 1 >= v_min) *v = *v - 1;
    }

    PopStyleVar();

    bool result = SliderScalar(label, ImGuiDataType_Float, v, &v_min, &v_max, format, flags);

    EndGroup();

    return result;
}

bool ImAdd::SliderInt(const char* label, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
{
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    BeginGroup();

    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, style.ItemInnerSpacing.y));
    Text(label);
    SameLine(CalcItemWidth() - ImGui::GetFontSize() * 2);
    if (ImAdd::ButtonLabel(std::string("+##" + std::string(label)).c_str(), ImVec2(GetFontSize(), GetFontSize())))
    {
        if (*v + 1 <= v_max) *v = *v + 1;
    }

    ImGui::SameLine();
    if (ImAdd::ButtonLabel(std::string("-##" + std::string(label)).c_str(), ImVec2(GetFontSize(), GetFontSize())))
    {
        if (*v - 1 >= v_min) *v = *v - 1;
    }

    PopStyleVar();

    bool result = SliderScalar(label, ImGuiDataType_S32, v, &v_min, &v_max, format, flags);

    EndGroup();

    return result;
}

void ImAdd::BeginChild(const char* label, const ImVec2& size_arg, bool scrollbar, bool dots_bg, bool bright_border)
{
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    ImGui::BeginChild(label, size_arg, ImGuiChildFlags_None, ImGuiWindowFlags_NoBackground);

    float HeaderHeight = GetFrameHeight();
    ImVec2 pos = ImGui::GetWindowPos();
    ImVec2 size = ImGui::GetWindowSize();
    ImDrawList* pDrawList = ImGui::GetWindowDrawList();

    float fGlowAlphaT = 0.0f;
    float fGlowAlphaB = 0.1f;

    //pDrawList->AddRectFilled(pos,  pos + ImVec2(size.x, HeaderHeight), ImGui::GetColorU32(ImGuiCol_MenuBarBg), style.ChildRounding, ImDrawFlags_RoundCornersTop);

    pDrawList->AddRectFilled(pos + ImVec2(0, HeaderHeight), pos + size, ImGui::GetColorU32(ImGuiCol_ChildBg), style.ChildRounding, ImDrawFlags_RoundCornersBottom);

    pDrawList->AddRectFilledMultiColorRounded(pos, pos + ImVec2(size.x, HeaderHeight), ImGui::GetColorU32(ImGuiCol_MenuBarBg), ImGui::GetColorU32(ImGuiCol_MenuBarBg), ImGui::GetColorU32(ImGuiCol_ButtonActive), ImGui::GetColorU32(ImGuiCol_ButtonActive), style.ChildRounding, ImDrawFlags_RoundCornersTop);


    SetCursorScreenPos(pos);
    ImGui::BeginChild(std::string(std::string(label) + "##Header").c_str(), ImVec2(0, HeaderHeight), ImGuiChildFlags_None, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    {

    }
    ImGui::EndChild();

    if (style.ChildBorderSize > 0)
    {
        pDrawList->AddLine(pos + ImVec2(0, HeaderHeight - style.ChildBorderSize), pos + ImVec2(size.x, HeaderHeight - style.ChildBorderSize), ImGui::GetColorU32(ImGuiCol_SliderGrab));
        pDrawList->AddLine(pos + ImVec2(0, HeaderHeight), pos + ImVec2(size.x, HeaderHeight), ImGui::GetColorU32(ImGuiCol_SliderGrabActive));

        pDrawList->AddRect(pos, pos + size, ImGui::GetColorU32(bright_border ? ImGuiCol_SliderGrab : ImGuiCol_Border), style.ChildRounding);
    }

    if (dots_bg)
    {
        int dots_pad = 6;
        int w_dots_count = std::trunc((size.x - style.WindowBorderSize * 2) / dots_pad);
        int h_dots_count = std::trunc((size.y - HeaderHeight - style.WindowBorderSize * 2) / dots_pad);

        for (int i = 0; i < w_dots_count + 1; i++)
        {
            for (int j = 0; j < h_dots_count + 1; j++)
            {
                ImVec2 dot_pos = pos + ImVec2(style.WindowBorderSize + i * dots_pad + 2, HeaderHeight + style.WindowBorderSize + j * dots_pad + 1);

                pDrawList->AddRectFilled(dot_pos, dot_pos + ImVec2(1, 1), ImGui::GetColorU32(ImGuiCol_Separator));
            }
        }
    }

    pDrawList->AddText(pos + style.FramePadding + ImVec2(-1, -1), IM_COL32_BLACK, label);
    pDrawList->AddText(pos + style.FramePadding + ImVec2(-1, 1), IM_COL32_BLACK, label);
    pDrawList->AddText(pos + style.FramePadding + ImVec2(1, -1), IM_COL32_BLACK, label);
    pDrawList->AddText(pos + style.FramePadding + ImVec2(1, 1), IM_COL32_BLACK, label);

    pDrawList->AddText(pos + style.FramePadding, GetColorU32(ImGuiCol_Text), label);

    SetCursorScreenPos(pos + ImVec2(0, HeaderHeight));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemInnerSpacing.x, style.ItemInnerSpacing.x));
    ImGui::BeginChild(std::string(std::string(label) + "##Main").c_str(), ImVec2(0, 0), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoBackground | (!scrollbar ? (ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse) : 0));
}

void ImAdd::EndChild()
{
    ImGui::EndChild();
    ImGui::PopStyleVar();
    ImGui::EndChild();
}

void ImAdd::BeginTabChild(const char* label, int* v, std::vector<const char*> vList, const ImVec2& size_arg)
{
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    ImGui::BeginChild(label, size_arg, ImGuiChildFlags_None, ImGuiWindowFlags_NoBackground);

    float HeaderHeight = GetFrameHeight();
    ImVec2 pos = ImGui::GetWindowPos();
    ImVec2 size = ImGui::GetWindowSize();
    ImDrawList* pDrawList = ImGui::GetWindowDrawList();

    float fGlowAlphaT = 0.0f;
    float fGlowAlphaB = 0.1f;

    //pDrawList->AddRectFilled(pos,  pos + ImVec2(size.x, HeaderHeight), ImGui::GetColorU32(ImGuiCol_MenuBarBg), style.ChildRounding, ImDrawFlags_RoundCornersTop);

    pDrawList->AddRectFilled(pos + ImVec2(0, HeaderHeight), pos + size, ImGui::GetColorU32(ImGuiCol_ChildBg), style.ChildRounding, ImDrawFlags_RoundCornersBottom);

    pDrawList->AddRectFilledMultiColorRounded(pos, pos + ImVec2(size.x, HeaderHeight), ImGui::GetColorU32(ImGuiCol_MenuBarBg), ImGui::GetColorU32(ImGuiCol_MenuBarBg), ImGui::GetColorU32(ImGuiCol_ButtonActive), ImGui::GetColorU32(ImGuiCol_ButtonActive), style.ChildRounding, ImDrawFlags_RoundCornersTop);

    if (style.ChildBorderSize > 0)
    {
        pDrawList->AddLine(pos + ImVec2(0, HeaderHeight - style.ChildBorderSize), pos + ImVec2(size.x, HeaderHeight - style.ChildBorderSize), ImGui::GetColorU32(ImGuiCol_SliderGrab));
        pDrawList->AddLine(pos + ImVec2(0, HeaderHeight), pos + ImVec2(size.x, HeaderHeight), ImGui::GetColorU32(ImGuiCol_SliderGrabActive));

        pDrawList->AddRect(pos, pos + size, ImGui::GetColorU32(ImGuiCol_Border), style.ChildRounding);
    }

    pDrawList->AddText(pos + style.FramePadding + ImVec2(-1, -1), IM_COL32_BLACK, label);
    pDrawList->AddText(pos + style.FramePadding + ImVec2(-1, 1), IM_COL32_BLACK, label);
    pDrawList->AddText(pos + style.FramePadding + ImVec2(1, -1), IM_COL32_BLACK, label);
    pDrawList->AddText(pos + style.FramePadding + ImVec2(1, 1), IM_COL32_BLACK, label);
    pDrawList->AddText(pos + style.FramePadding, GetColorU32(ImGuiCol_Text), label);

    SetCursorScreenPos(pos);
    ImGui::BeginChild(std::string(std::string(label) + "##MenuBar").c_str(), ImVec2(size.x, HeaderHeight), ImGuiChildFlags_None, ImGuiWindowFlags_NoBackground);
    {
        float fRadioWidth = 0;

		for (int i = 0; i < vList.size(); i++)
		{
			fRadioWidth += ImGui::CalcTextSize(vList[i]).x;
		}

		fRadioWidth += (vList.size() - 1) * style.ItemSpacing.x; // spacing between items

        ImGui::SetCursorScreenPos(ImGui::GetWindowPos() + ImVec2(ImGui::GetWindowWidth() - fRadioWidth - style.FramePadding.y, style.FramePadding.y - 1));
        ImGui::BeginGroup();
        for (int i = 0; i < vList.size(); i++)
        {
            ImAdd::RadioFrameText(vList[i], v, i, false, true);
			if (i < vList.size() - 1) {
				ImGui::SameLine();
			}
        }
        ImGui::EndGroup();
    }
    ImGui::EndChild();

    SetCursorScreenPos(pos + ImVec2(0, HeaderHeight));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemInnerSpacing.x, style.ItemInnerSpacing.x));
    ImGui::BeginChild(std::string(std::string(label) + "##Main").c_str(), ImVec2(size.x, size.y - HeaderHeight), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoBackground);
}

void ImAdd::EndTabChild()
{
    ImGui::EndChild();
    ImGui::PopStyleVar();
    ImGui::EndChild();
}

bool ImAdd::InputText(ImTextureID icon_texture, const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags)
{
    IM_ASSERT(!(flags & ImGuiInputTextFlags_Multiline)); // call InputTextMultiline()

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    ImVec2 pos = GetCursorScreenPos();
    bool has_icon = icon_texture != 0;

    ImGui::BeginChild(label, ImVec2(CalcItemWidth(), GetFrameHeight()), ImGuiChildFlags_None, ImGuiWindowFlags_NoBackground);

    window->DrawList->AddRectFilled(pos, pos + GetWindowSize(), GetColorU32(ImGuiCol_FrameBg), style.FrameRounding);

    if (style.FrameBorderSize > 0) {
        window->DrawList->AddRect(pos, pos + GetWindowSize(), GetColorU32(ImGuiCol_Border), style.FrameRounding, 0, style.FrameBorderSize);
    }

    if (has_icon) {
        window->DrawList->AddImage(icon_texture, pos + style.FramePadding, pos + ImVec2(GetWindowHeight(), GetWindowHeight()) - style.FramePadding, ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(ImGuiCol_Text));
    }

    PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
    PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
    PushItemWidth(has_icon ? (GetWindowWidth() - GetFontSize() - style.FramePadding.x) : GetWindowWidth());
    if (has_icon) {
        SetCursorScreenPos(pos + ImVec2(GetFontSize() + style.FramePadding.x, 0));
    }
    bool input = InputTextEx(string("##" + string(label)).c_str(), NULL, buf, (int)buf_size, ImVec2(0, 0), flags);
    PopItemWidth();
    PopStyleVar();
    PopStyleColor();

    if (has_icon) {
        GetForegroundDrawList()->AddRectFilledMultiColor(pos + ImVec2(GetFontSize() + style.FramePadding.x, style.FrameBorderSize), pos + ImVec2(GetWindowHeight(), GetWindowHeight() - style.FrameBorderSize), GetColorU32(ImGuiCol_FrameBg), GetColorU32(ImGuiCol_FrameBg, 0.0f), GetColorU32(ImGuiCol_FrameBg, 0.0f), GetColorU32(ImGuiCol_FrameBg));
    }

    if (!IsItemActive() && strlen(buf) == 0) {
        window->DrawList->AddText(pos + ImVec2(has_icon ? GetWindowHeight() : style.FramePadding.x, style.FramePadding.y) + ImVec2(1, 1), IM_COL32_BLACK, label);
        window->DrawList->AddText(pos + ImVec2(has_icon ? GetWindowHeight() : style.FramePadding.x, style.FramePadding.y) + ImVec2(-1, -1), IM_COL32_BLACK, label);
        window->DrawList->AddText(pos + ImVec2(has_icon ? GetWindowHeight() : style.FramePadding.x, style.FramePadding.y) + ImVec2(-1, 1), IM_COL32_BLACK, label);
        window->DrawList->AddText(pos + ImVec2(has_icon ? GetWindowHeight() : style.FramePadding.x, style.FramePadding.y) + ImVec2(1, -1), IM_COL32_BLACK, label);
        window->DrawList->AddText(pos + ImVec2(has_icon ? GetWindowHeight() : style.FramePadding.x, style.FramePadding.y), GetColorU32(ImGuiCol_TextDisabled), label);
    }

    ImGui::EndChild();

    return input;
}

const char* GetStringByIndex(const char* multiString, size_t index) 
{
    if (!multiString) return nullptr;

    size_t currentIndex = 0;
    const char* ptr = multiString;

    while (*ptr) {
        if (currentIndex == index)
            return ptr;

        // Move to the next string
        ptr += std::strlen(ptr) + 1;
        currentIndex++;
    }

    // Index not found
    return nullptr;
}

size_t GetMultiStringItemCount(const char* multiString) 
{
    const char* ptr = multiString;
    size_t count = 0;

    while (*ptr) {
        ++count;
        ptr += std::strlen(ptr) + 1;
    }

    return count;
}

bool ImAdd::Combo(const char* str_id, int* current_item, const char* items_separated_by_zeros)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(str_id);

    const float width = CalcItemWidth();
    const float height = g.FontSize + style.CellPadding.y * 2;

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImVec2(width, height);

    const ImRect bb(pos, pos + size);
    ItemSize(size);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);
    bool has_border = style.FrameBorderSize > 0;

    // Colors
    ImVec4 colFrame = GetStyleColorVec4((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBg : ImGuiCol_FrameBgHovered);

    // Animations
    struct stColors_State {
        ImVec4 Frame;
    };

    static std::map<ImGuiID, stColors_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, stColors_State() });
        it_anim = anim.find(id);

        it_anim->second.Frame = colFrame;
    }

    it_anim->second.Frame = ImLerp(it_anim->second.Frame, colFrame, 1.0f / IMADD_ANIMATIONS_SPEED * GetIO().DeltaTime);

    // Render
    RenderNavHighlight(bb, id);

    window->DrawList->AddRectFilledMultiColorRounded(bb.Min, bb.Max, GetColorU32(ImGuiCol_PopupBg), GetColorU32(ImGuiCol_PopupBg), GetColorU32(it_anim->second.Frame), GetColorU32(it_anim->second.Frame), style.FrameRounding);
    if (has_border)
    {
        window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(ImGuiCol_SeparatorActive), style.FrameRounding, 0, style.FrameBorderSize);
    }

    window->DrawList->AddText(pos + ImVec2(size.x - ImGui::CalcTextSize("+").x - style.CellPadding.x, size.y / 2 - g.FontSize / 2 - 0.5f) + ImVec2(-1, -1), IM_COL32_BLACK, "+");
    window->DrawList->AddText(pos + ImVec2(size.x - ImGui::CalcTextSize("+").x - style.CellPadding.x, size.y / 2 - g.FontSize / 2 - 0.5f) + ImVec2(1, -1), IM_COL32_BLACK, "+");
    window->DrawList->AddText(pos + ImVec2(size.x - ImGui::CalcTextSize("+").x - style.CellPadding.x, size.y / 2 - g.FontSize / 2 - 0.5f) + ImVec2(-1, 1), IM_COL32_BLACK, "+");
    window->DrawList->AddText(pos + ImVec2(size.x - ImGui::CalcTextSize("+").x - style.CellPadding.x, size.y / 2 - g.FontSize / 2 - 0.5f) + ImVec2(1, 1), IM_COL32_BLACK, "+");
    window->DrawList->AddText(pos + ImVec2(size.x - ImGui::CalcTextSize("+").x - style.CellPadding.x, size.y / 2 - g.FontSize / 2 - 0.5f), GetColorU32(ImGuiCol_Text), "+");

    string item_preview = GetStringByIndex(items_separated_by_zeros, *current_item);

    window->DrawList->AddText(pos + style.CellPadding + ImVec2(-1, -1), IM_COL32_BLACK, item_preview.c_str() == nullptr ? "*error item*" : item_preview.c_str());
    window->DrawList->AddText(pos + style.CellPadding + ImVec2(-1, 1), IM_COL32_BLACK, item_preview.c_str() == nullptr ? "*error item*" : item_preview.c_str());
    window->DrawList->AddText(pos + style.CellPadding + ImVec2(1, -1), IM_COL32_BLACK, item_preview.c_str() == nullptr ? "*error item*" : item_preview.c_str());
    window->DrawList->AddText(pos + style.CellPadding + ImVec2(1, 1), IM_COL32_BLACK, item_preview.c_str() == nullptr ? "*error item*" : item_preview.c_str());
    window->DrawList->AddText(pos + style.CellPadding, GetColorU32(ImGuiCol_TextDisabled), item_preview.c_str() == nullptr ? "*error item*" : item_preview.c_str());

    if (pressed)
    {
        OpenPopup(str_id);
    }

    PushStyleVar(ImGuiStyleVar_WindowPadding, style.CellPadding);
    PushStyleVar(ImGuiStyleVar_ItemSpacing  , style.CellPadding);
    PushStyleVar(ImGuiStyleVar_FramePadding , style.CellPadding);
    PushStyleVar(ImGuiStyleVar_PopupRounding , style.FrameRounding);
    PushStyleVar(ImGuiStyleVar_PopupBorderSize , style.FrameBorderSize);
    if (ImGui::BeginPopup(str_id, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground))
    {
        ImDrawList* pDrawList = GetForegroundDrawList();
        pDrawList->AddRectFilled(pos, pos + GetWindowSize(), GetColorU32(ImGuiCol_PopupBg), style.PopupRounding);
        if (style.PopupBorderSize > 0)
        {
            pDrawList->AddRect(pos, pos + GetWindowSize(), GetColorU32(ImGuiCol_Border), style.PopupRounding, 0, style.PopupBorderSize);
        }

        const int ItemsCount = GetMultiStringItemCount(items_separated_by_zeros);
        const float PopupHeight = 
            (ItemsCount * g.FontSize + style.FramePadding.y * 2) +  // Total frames
            ((ItemsCount - 1) * style.ItemSpacing.y) +              // Total spaces
            (style.WindowPadding.y * 2);                            // window padding

        SetWindowPos(pos, ImGuiCond_Always);
        //SetWindowSize(ImVec2(width, PopupHeight), ImGuiCond_Always);

        for (size_t i = 0; i < ItemsCount; i++)
        {
            string item_label = GetStringByIndex(items_separated_by_zeros, i);

            if (SelectableLabel(pDrawList, item_label.c_str(), i == *current_item, ImVec2(width - style.WindowPadding.x * 2, 0)))
            {
                *current_item = i;
                CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }
    PopStyleVar(5);

    return pressed;
}

bool ImAdd::Combo(const char* str_id, int* current_item, const char* items_separated_by_zeros, float width)
{
    PushItemWidth(width <= 0 ? CalcItemWidth() : width);
    bool result = Combo(str_id, current_item, items_separated_by_zeros);
    PopItemWidth();
    return result;
}

bool ImAdd::MultiCombo(const char* str_id, std::vector<int>* selected, std::vector<const char*> items)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(str_id);

    const float width = CalcItemWidth();
    const float height = g.FontSize + style.CellPadding.y * 2;

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImVec2(width, height);

    const ImRect bb(pos, pos + size);
    ItemSize(size);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held, popup_open = false;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);
    if (pressed) popup_open = !popup_open;

    std::string preview = "";

    int stop_at = -1;
    for (int i = 0; i < items.size(); i++)
    {
        if ((*selected)[i] == 1)
        {
            stop_at = i;
        }
    }

    for (int i = 0; i < items.size(); i++)
    {
        if ((*selected)[i] == 1)
        {
            preview += items[i] + std::string(i == stop_at ? "" : ", ");
        }
    }

    // Colors
    ImU32 bg_col = GetColorU32(hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
    ImU32 text_col = GetColorU32((popup_open || hovered) ? ImGuiCol_Text : ImGuiCol_TextDisabled);

    // Render
    RenderNavHighlight(bb, id);
    
    window->DrawList->AddRectFilledMultiColorRounded(bb.Min, bb.Max, GetColorU32(ImGuiCol_PopupBg), GetColorU32(ImGuiCol_PopupBg), bg_col, bg_col, style.FrameRounding);
    
    window->DrawList->PushClipRect(pos, pos + ImVec2(size.x - size.y + style.CellPadding.x, size.y));
    window->DrawList->AddText(pos + style.CellPadding + ImVec2(-1, -1), IM_COL32_BLACK, preview.length() == 0 ? "none" : preview.c_str());
    window->DrawList->AddText(pos + style.CellPadding + ImVec2(-1, 1), IM_COL32_BLACK, preview.length() == 0 ? "none" : preview.c_str());
    window->DrawList->AddText(pos + style.CellPadding + ImVec2(1, -1), IM_COL32_BLACK, preview.length() == 0 ? "none" : preview.c_str());
    window->DrawList->AddText(pos + style.CellPadding + ImVec2(1, 1), IM_COL32_BLACK, preview.length() == 0 ? "none" : preview.c_str());
    window->DrawList->AddText(pos + style.CellPadding, GetColorU32(ImGuiCol_TextDisabled), preview.length() == 0 ? "none" : preview.c_str());
    window->DrawList->PopClipRect();

    //window->DrawList->AddRectFilledMultiColor(pos + ImVec2(size.x - size.y - g.FontSize, 0), pos + ImVec2(size.x - size.y + style.CellPadding.x, size.y), GetColorU32(bg_col, 0.0f), bg_col, bg_col, GetColorU32(bg_col, 0.0f));
    
    if (style.FrameBorderSize > 0)
    {
        window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(ImGuiCol_SeparatorActive), style.FrameRounding, 0, style.FrameBorderSize);
    }

    window->DrawList->AddText(pos + ImVec2(size.x - ImGui::CalcTextSize("+").x - style.CellPadding.x, size.y / 2 - g.FontSize / 2 - 0.5f) + ImVec2(-1, -1), IM_COL32_BLACK, "+");
    window->DrawList->AddText(pos + ImVec2(size.x - ImGui::CalcTextSize("+").x - style.CellPadding.x, size.y / 2 - g.FontSize / 2 - 0.5f) + ImVec2(1, -1), IM_COL32_BLACK, "+");
    window->DrawList->AddText(pos + ImVec2(size.x - ImGui::CalcTextSize("+").x - style.CellPadding.x, size.y / 2 - g.FontSize / 2 - 0.5f) + ImVec2(-1, 1), IM_COL32_BLACK, "+");
    window->DrawList->AddText(pos + ImVec2(size.x - ImGui::CalcTextSize("+").x - style.CellPadding.x, size.y / 2 - g.FontSize / 2 - 0.5f) + ImVec2(1, 1), IM_COL32_BLACK, "+");
    window->DrawList->AddText(pos + ImVec2(size.x - ImGui::CalcTextSize("+").x - style.CellPadding.x, size.y / 2 - g.FontSize / 2 - 0.5f), GetColorU32(ImGuiCol_Text), "+");

    if (popup_open)
    {
        OpenPopup(std::string("MultiCombo_" + std::string(str_id)).c_str());
    }

    PushStyleVar(ImGuiStyleVar_WindowPadding, style.CellPadding);
    PushStyleVar(ImGuiStyleVar_ItemSpacing, style.CellPadding);
    PushStyleVar(ImGuiStyleVar_FramePadding, style.CellPadding);
    PushStyleVar(ImGuiStyleVar_PopupRounding, style.FrameRounding);
    PushStyleVar(ImGuiStyleVar_PopupBorderSize, style.FrameBorderSize);
    if (ImGui::BeginPopup(std::string("MultiCombo_" + std::string(str_id)).c_str(), ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize))
    {
        SetWindowPos(pos);
        SetWindowSize(ImVec2(size.x, 0));

        for (int i = 0; i < items.size(); i++)
        {
            if (ImAdd::SelectableLabel(GetForegroundDrawList(), items[i], (*selected)[i] == 1, ImVec2(size.x - style.WindowPadding.x * 2, 0)))
            {
                (*selected)[i] = (*selected)[i] == 1 ? 0 : 1;
            }
        }

        ImGui::EndPopup();
    }
    PopStyleVar(5);

    return pressed;
}

bool ImAdd::MultiCombo(const char* str_id, std::vector<int>* selected, std::vector<const char*> items, float width)
{
	PushItemWidth(width <= 0 ? CalcItemWidth() : width);
	bool result = MultiCombo(str_id, selected, items);
	PopItemWidth();
	return result;
}

bool ImAdd::ColorEdit4(const char* str_id, float col[4])
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImVec4 col_v4(col[0], col[1], col[2], col[3]);

    float height = ImGui::GetFontSize();
    float width = height * 2;

    ImGuiColorEditFlags flags = ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoSidePreview;

    PushStyleColor(ImGuiCol_Border, ImVec4(col_v4.x, col_v4.y, col_v4.z, 1.0f));
    PushStyleColor(ImGuiCol_BorderShadow, ImVec4());
    PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
    bool result = ColorButton(str_id, col_v4, flags, ImVec2(width, height));
    PopStyleVar();
    PopStyleColor(2);
    if (result)
    {
        OpenPopup(std::string(std::string(str_id) + "##ColorEdit4").c_str());
    }
    if (ImGui::BeginPopup(std::string(std::string(str_id) + "##ColorEdit4").c_str()))
    {
        PushStyleColor(ImGuiCol_BorderShadow, ImVec4());
        ColorPicker4(str_id, col, flags);
        PopStyleColor();
        ImGui::EndPopup();
    }

    return result;
}

#include <Windows.h>

bool ImAdd::KeyBind(const char* str_id, int* k, int width)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    ImGuiIO& io = g.IO;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(str_id);

    ImVec2 pos = window->DC.CursorPos;

    float height = g.FontSize;

    ImVec2 size = ImVec2(width, height);
    ImRect frame_bb(pos, pos + size);
    ImRect total_bb(pos, frame_bb.Max);

    ImGui::ItemSize(total_bb);
    if (!ImGui::ItemAdd(total_bb, id))
        return false;

    const bool hovered = ImGui::ItemHoverable(frame_bb, id, 0);

    if (hovered)
    {
        ImGui::SetHoveredID(id);
        g.MouseCursor = ImGuiMouseCursor_Hand;
    }

    const bool user_clicked = hovered && io.MouseClicked[0];

    if (user_clicked)
    {
        if (g.ActiveId != id)
        {
            memset(io.MouseDown, 0, sizeof(io.MouseDown));
            //memset(io.KeysDown, 0, sizeof(io.KeysDown));
            *k = 0;
        }
        ImGui::SetActiveID(id, window);
        ImGui::FocusWindow(window);
    }
    else if (io.MouseClicked[0])
    {
        if (g.ActiveId == id)
            ImGui::ClearActiveID();
    }

    bool value_changed = false;
    int key = *k;

    if (g.ActiveId == id)
    {
        if (!value_changed)
        {
            for (int vk = 0x08; vk <= 0xA5; ++vk) // VK_BACK to VK_OEM_CLEAR
            {
                if (GetAsyncKeyState(vk) & 0x8000)
                {
                    key = vk;
                    value_changed = true;
                    ImGui::ClearActiveID();
                    break;
                }
            }
        }

        if (ImGui::IsKeyPressed(ImGuiKey_Escape))
        {
            *k = 0;
            ImGui::ClearActiveID();
        }
        else *k = key;
    }

    window->DrawList->AddRectFilled(frame_bb.Min, frame_bb.Max, ImGui::GetColorU32(ImGuiCol_FrameBg), style.FrameRounding);
    if (style.FrameBorderSize > 0)
    {
        window->DrawList->AddRect(frame_bb.Min, frame_bb.Max, ImGui::GetColorU32(ImGuiCol_Border), style.FrameRounding, 0, style.FrameBorderSize);
    }

    ImGui::RenderNavHighlight(total_bb, id);

    char buf_display[32] = "-";

    if (*k != 0 && g.ActiveId != id)
        strcpy_s(buf_display, sizeof buf_display, szKeyNames[*k]);
    else if (g.ActiveId == id)
        strcpy_s(buf_display, sizeof buf_display, "?");

    ImVec2 buf_display_size = ImGui::CalcTextSize(buf_display, NULL, true);

    window->DrawList->AddText(pos + ImVec2(size.x / 2 - buf_display_size.x / 2, 0), GetColorU32(ImGuiCol_TextDisabled), buf_display);

    return value_changed;
}

void ImAdd::KeyBindEx(const char* str_id, int* k, int* mode, int width)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    ImGuiIO& io = g.IO;
    const ImGuiStyle& style = g.Style;

    ImAdd::KeyBind(str_id, k, width);

    static const char* popup_name = std::string(str_id + std::string("##popup")).c_str();

    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right, false))
    {
        ImGui::OpenPopup(popup_name);
    }

    ImVec2 wnd_pad = style.WindowPadding;

    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
    PushStyleVar(ImGuiStyleVar_PopupBorderSize, 0);
    if (ImGui::BeginPopup(popup_name, ImGuiWindowFlags_NoBackground))
    {
        PushStyleColor(ImGuiCol_ChildBg, style.Colors[ImGuiCol_PopupBg]);
        PushStyleVar(ImGuiStyleVar_WindowPadding, wnd_pad);
        ImAdd::BeginChild("Keybind", ImVec2(120, GetFrameHeight() + style.WindowPadding.y * 2 + style.ItemSpacing.y * 2 + GetFontSize() * 3));
        {
            if (ImAdd::SelectableLabel(GetWindowDrawList(), "hold"  , *mode == 0))  *mode = 0;
            if (ImAdd::SelectableLabel(GetWindowDrawList(), "toggle", *mode == 1))  *mode = 1;
            if (ImAdd::SelectableLabel(GetWindowDrawList(), "always", *mode == 2))  *mode = 2;
        }
        ImAdd::EndChild();
        PopStyleVar();
        PopStyleColor();

        ImGui::EndPopup();
    }
    PopStyleVar(2);
}

bool ImAdd::PopupButton(const char* popup_id)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(popup_id);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImVec2(g.FontSize, g.FontSize);

    const ImRect bb(pos, pos + size);
    ItemSize(size);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);
    bool has_border = style.FrameBorderSize > 0;

    if (pressed)
    {
        ImGui::OpenPopup(popup_id);
    }

    // Colors
    ImVec4 colText = GetStyleColorVec4((hovered && held) ? ImGuiCol_TextDisabled : ImGuiCol_Text);

    // Animations
    struct stColors_State {
        ImVec4 Text;
    };

    static std::map<ImGuiID, stColors_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, stColors_State() });
        it_anim = anim.find(id);

        it_anim->second.Text = colText;
    }

    it_anim->second.Text = ImLerp(it_anim->second.Text, colText, 1.0f / IMADD_ANIMATIONS_SPEED * GetIO().DeltaTime);

    // Render
    RenderNavHighlight(bb, id);

    std::string sign = "+";
    if (IsPopupOpen(popup_id)) sign = "-";

    ImVec2 sign_size = CalcTextSize(sign.c_str(), NULL, true);
    
    window->DrawList->AddText(pos + size / 2 - sign_size / 2 + ImVec2(-1, -1), IM_COL32(0, 0, 0, style.Alpha * 255), sign.c_str());
    window->DrawList->AddText(pos + size / 2 - sign_size / 2 + ImVec2(1, -1), IM_COL32(0, 0, 0, style.Alpha * 255), sign.c_str());
    window->DrawList->AddText(pos + size / 2 - sign_size / 2 + ImVec2(-1, 1), IM_COL32(0, 0, 0, style.Alpha * 255), sign.c_str());
    window->DrawList->AddText(pos + size / 2 - sign_size / 2 + ImVec2(1, 1), IM_COL32(0, 0, 0, style.Alpha * 255), sign.c_str());
    window->DrawList->AddText(pos + size / 2 - sign_size / 2, GetColorU32(it_anim->second.Text), sign.c_str());

    return pressed;
}

bool ImAdd::BeginPopup(const char* popup_id)
{
    PushStyleColor(ImGuiCol_Border, ImVec4());
    PushStyleColor(ImGuiCol_PopupBg, ImVec4());
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    PushStyleVar(ImGuiStyleVar_PopupBorderSize, 0.0f);
    bool result = ImGui::BeginPopup(popup_id, ImGuiWindowFlags_NoResize);
    PopStyleVar(2);
    PopStyleColor(2);

    return result;
}

void ImAdd::EndPopup()
{
    ImGui::EndPopup();
}

Particles_CFG ImAdd::GetParticlesConfigs()
{
    Particles_CFG config;
    config.draw_dots = true;
    config.draw_lines = false;
    config.killer_mouse = false;
    config.dot_color = ImGui::GetColorU32(ImGuiCol_Text, 0.2f);
    config.line_color = ImGui::GetColorU32(ImGuiCol_SliderGrab);
    config.dot_radius = 1.5f;
    config.killer_mouse_radius = 15.f;
    config.line_thickness = 1.5f;
    config.connection_distance = 100;
    config.particle_speed = 0.01f;

    return config;
}