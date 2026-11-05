#pragma once

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_particles.h"

#include <vector>

#define IMADD_ANIMATIONS_SPEED 0.05f // Second

namespace ImAdd
{
    // Helpers
    ImVec4  HexToColorVec4(unsigned int hex_color, float alpha);

    // DoubleText
    bool    Label(const char* label, const ImVec2& size = ImVec2(0, 0), const ImVec2& text_align = ImVec2(0, 0));
    void    DoubleText(ImVec4 color1, ImVec4 color2, const char* label1, const char* label2);

    // Separator
	void    SeparatorText(const char* label, float thickness = 1.0f);
	void    VSeparator(float margin = 0.0f, float thickness = 1.0f);

    // Navigation
	bool    RadioFrame(const char* label, int* v, int radio_id, const ImVec2& size_arg = ImVec2(0, 0));
	bool    RadioFrameIcon(ImTextureID icon_texture, const char* label, int* v, int radio_id, const ImVec2& size = ImVec2(0, 0), const ImVec2& icon_padding = ImVec2(0, 0));
    bool    RadioFrameText(const char* label, int* v, int radio_id, bool underline = true, bool border = false, const ImVec2& size_arg = ImVec2(0, 0));

    // Button
    bool    LinkLabel(const char* label, bool underlined = true, const ImVec2& size_arg = ImVec2(0, 0), const ImVec2& text_align = ImVec2(0, 0), ImGuiButtonFlags button_flags = 0);
    bool    Button(const char* label, const ImVec2& size_arg = ImVec2(0, 0), ImGuiButtonFlags button_flags = 0, ImDrawFlags draw_flags = 0);
    bool    ButtonLabel(const char* label, const ImVec2& size_arg = ImVec2(0, 0), ImGuiButtonFlags button_flags = 0, ImDrawFlags draw_flags = 0);
    bool    ButtonIcon(const char* label, ImTextureID icon_texture, const ImVec2& size_arg = ImVec2(0, 0), ImGuiButtonFlags button_flags = 0, ImDrawFlags draw_flags = 0, bool background = true);
    bool    SelectableLabel(ImDrawList* drawlist, const char* label, bool selected = false, const ImVec2& size_arg = ImVec2(0, 0));

    // Toggle
    bool    CheckBox(const char* label, bool* v);

    // Slider
    bool    SliderScalar(const char* str_id, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0);
    bool    SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.1f", ImGuiSliderFlags flags = 0); 
    bool    SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);

    // Custom Child
    void    BeginChild(const char* label, const ImVec2& size = ImVec2(0, 0), bool scrollbar = true, bool dots_bg = false, bool bright_border = false);
    void    EndChild();
	
    void    BeginTabChild(const char* label, int* v, std::vector<const char*> vList, const ImVec2& size = ImVec2(0, 0));
    void    EndTabChild();

    // Inputs
	bool    InputText(ImTextureID icon_texture, const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0);

    // Combo
    bool    Combo(const char* str_id, int* current_item, const char* items_separated_by_zeros);
    bool    Combo(const char* str_id, int* current_item, const char* items_separated_by_zeros, float width);
    bool    MultiCombo(const char* str_id, std::vector<int>* selected, std::vector<const char*> items);
    bool    MultiCombo(const char* str_id, std::vector<int>* selected, std::vector<const char*> items, float width);


    // ColorPicker
    bool    ColorEdit4(const char* str_id, float col[4]);

    // Keybind
    bool	KeyBind(const char* str_id, int* k, int width);
    void	KeyBindEx(const char* str_id, int* k, int* mode, int width);

	// Popup
	bool    PopupButton(const char* popup_id);
	bool    BeginPopup(const char* popup_id);
	void    EndPopup();

	// Particle
	Particles_CFG GetParticlesConfigs();
}

const char* const szKeyNames[] =
{
	"-",
	"LBUTTON",
	"RBUTTON",
	"CANCEL",
	"MBUTTON",
	"XBUTTON1",
	"XBUTTON2",
	"Unknown",
	"BACK",
	"TAB",
	"Unknown",
	"Unknown",
	"CLEAR",
	"RETURN",
	"Unknown",
	"Unknown",
	"SHIFT",
	"CONTROL",
	"MENU",
	"PAUSE",
	"CAPITAL",
	"KANA",
	"Unknown",
	"JUNJA",
	"FINAL",
	"KANJI",
	"Unknown",
	"ESCAPE",
	"CONVERT",
	"NONCONVERT",
	"ACCEPT",
	"MODECHANGE",
	"SPACE",
	"PRIOR",
	"NEXT",
	"END",
	"HOME",
	"LEFT",
	"UP",
	"RIGHT",
	"DOWN",
	"SELECT",
	"PRINT",
	"EXECUTE",
	"SNAPSHOT",
	"INSERT",
	"DELETE",
	"HELP",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"LWIN",
	"RWIN",
	"APPS",
	"Unknown",
	"SLEEP",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"MULTIPLY",
	"ADD",
	"SEPARATOR",
	"SUBTRACT",
	"DECIMAL",
	"DIVIDE",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"F13",
	"F14",
	"F15",
	"F16",
	"F17",
	"F18",
	"F19",
	"F20",
	"F21",
	"F22",
	"F23",
	"F24",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"NUMLOCK",
	"SCROLL",
	"OEM_NEC_EQUAL",
	"OEM_FJ_MASSHOU",
	"OEM_FJ_TOUROKU",
	"OEM_FJ_LOYA",
	"OEM_FJ_ROYA",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"LSHIFT",
	"RSHIFT",
	"LCONTROL",
	"RCONTROL",
	"LMENU",
	"RMENU"
};
