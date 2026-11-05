#include "CGui.h"
#include "CDirectX.h"
#include "Globals.h"
#include "CTextEditor.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "addons/imgui_addons.h"
#include "addons/imgui_particles.h"
#include "misc/freetype/imgui_freetype.h"

#include "../../resources/fonts/font_cascadia_mono_pl_regular.h"
#include "../Resources/Fonts/font_montserrat_bold.h"
#include "../Resources/Fonts/font_montserrat_medium.h"
#include "../Resources/Fonts/font_montserrat_regular.h"
#include "../Resources/Fonts/font_montserrat_semibold.h"
#include "../Resources/Fonts/font_roboto_bold.h"
#include "../Resources/Fonts/font_roboto_medium.h"
#include "../Resources/Fonts/font_roboto_regular.h"
#include "../Resources/Fonts/font_roboto_semibold.h"
#include "../Resources/Fonts/font_exo2_bold.h"
#include "../Resources/Fonts/font_exo2_medium.h"
#include "../Resources/Fonts/font_exo2_regular.h"
//#include "../Resources/Fonts/font_exo2_semibold.h"
#include "../Resources/Fonts/font_inter_bold.h"
#include "../Resources/Fonts/font_inter_medium.h"
#include "../Resources/Fonts/font_inter_regular.h"
#include "../Resources/Fonts/font_inter_semibold.h"
#include "../Resources/Fonts/font_poppins_bold.h"
#include "../Resources/Fonts/font_poppins_light.h"
#include "../Resources/Fonts/font_poppins_medium.h"
#include "../Resources/Fonts/font_poppins_regular.h"
#include "../Resources/Fonts/font_poppins_semibold.h"

#include <iostream>
#include <iomanip>
#include <ctime>
#include <string>

CGui::CGui()
{
    m_hWnd              = 0;
    m_pDevice           = 0;
    m_pDeviceContext    = 0;
    m_pTextures         = 0;
    m_pParticles        = 0;
    m_iCurrentTab       = 4;
    
    m_vTabs.push_back("  aimbot ");
    m_vTabs.push_back(" visuals ");
    m_vTabs.push_back(" misc ");
    m_vTabs.push_back(" settings ");
    m_vTabs.push_back(" scripts  ");

    memset(m_szSearch, 0, sizeof m_szSearch);
}

CGui::~CGui()
{
}

bool CGui::Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context)
{
    if (!hwnd)
        return false;
    
    if (!device)
        return false;
    
    if (!device_context)
        return false;

    m_hWnd = hwnd;
    m_pDevice = device;
    m_pDeviceContext = device_context;
    
    m_pTextures = new CTextures();
    if (!m_pTextures->Initialize(device))
    {
        return false;
    }

    m_pParticles = new Particles();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.IniFilename = nullptr;               // Disable INI File 

    // Setup Dear ImGui default style
    ImGui::StyleColorsDark();

    // Custom Styles
    style.WindowRounding    = 4;
    style.ChildRounding     = 3;
    style.FrameRounding     = 3;
    style.PopupRounding     = 4;
    style.ScrollbarRounding = 2;
    style.GrabRounding      = 2;
    style.TabRounding       = 2;

    style.ButtonTextAlign   = { 0.5f, 0.5f };
    style.WindowTitleAlign  = { 0.5f, 0.5f };

    style.FramePadding      = { 8.0f, 8.0f };
    style.WindowPadding     = { 10.0f, 10.0f };
    style.ItemSpacing       = { 10.0f, 10.0f };
    style.ItemInnerSpacing  = { 6.0f, 3.0f };
    style.CellPadding       = { 4.0f, 2.0f };

    style.WindowBorderSize  = 1;
    style.FrameBorderSize   = 1;
    style.PopupBorderSize   = 1;

    style.ScrollbarSize     = 12.0f;
    style.GrabMinSize       = 10.0f;
    style.DisabledAlpha     = 0.5f;

    style.WindowShadowSize  = 0;

	// Custom Colors
    style.Colors[ImGuiCol_WindowBg]             = ImAdd::HexToColorVec4(0x0c0c0c, 1.00f);
    style.Colors[ImGuiCol_ChildBg]              = ImAdd::HexToColorVec4(0x141416, 0.60f);
    style.Colors[ImGuiCol_MenuBarBg]            = ImAdd::HexToColorVec4(0x212121, 1.00f);
    style.Colors[ImGuiCol_PopupBg]              = ImAdd::HexToColorVec4(0x0d0d0d, 1.00f);   
    
    style.Colors[ImGuiCol_CheckMark]            = ImAdd::HexToColorVec4(0xb4b4b4, 1.00f);
    style.Colors[ImGuiCol_Text]                 = ImAdd::HexToColorVec4(0xb4b4b4, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]         = ImAdd::HexToColorVec4(0x858585, 1.00f);
    
    style.Colors[ImGuiCol_SliderGrab]           = ImAdd::HexToColorVec4(0x255a77, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive]     = ImAdd::HexToColorVec4(0x1e3441, 1.00f);
    style.Colors[ImGuiCol_WindowShadow]         = style.Colors[ImGuiCol_SliderGrab];
    
    style.Colors[ImGuiCol_BorderShadow]         = ImAdd::HexToColorVec4(0x000000, 1.00f);
    style.Colors[ImGuiCol_Border]               = ImAdd::HexToColorVec4(0x313131, 0.80f);
    style.Colors[ImGuiCol_SeparatorActive]      = ImAdd::HexToColorVec4(0x3f3f3f, 0.80f);
    style.Colors[ImGuiCol_Separator]            = ImAdd::HexToColorVec4(0x222222, 1.00f);
    
    style.Colors[ImGuiCol_Button]               = ImAdd::HexToColorVec4(0x191919, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered]        = ImAdd::HexToColorVec4(0x212121, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]         = ImAdd::HexToColorVec4(0x131313, 1.00f);
    
    style.Colors[ImGuiCol_FrameBg]              = style.Colors[ImGuiCol_Button];
    style.Colors[ImGuiCol_FrameBgHovered]       = style.Colors[ImGuiCol_ButtonHovered];
    style.Colors[ImGuiCol_FrameBgActive]        = style.Colors[ImGuiCol_ButtonActive];
    
    style.Colors[ImGuiCol_Header]               = ImAdd::HexToColorVec4(0x2a2a2a, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered]        = ImAdd::HexToColorVec4(0x3f3f3f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive]         = ImAdd::HexToColorVec4(0x111111, 1.00f);
    
    style.Colors[ImGuiCol_ScrollbarBg]          = ImVec4();
    style.Colors[ImGuiCol_ScrollbarGrab]        = ImAdd::HexToColorVec4(0x191919, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = style.Colors[ImGuiCol_ScrollbarGrab];
    style.Colors[ImGuiCol_ScrollbarGrabActive]  = style.Colors[ImGuiCol_ScrollbarGrab];
    
    style.Colors[ImGuiCol_ResizeGrip]           = ImVec4();
    style.Colors[ImGuiCol_ResizeGripHovered]    = ImVec4();
    style.Colors[ImGuiCol_ResizeGripActive]     = ImVec4();

    // Setup Font
    ImFontConfig cfg_main;
    cfg_main.FontLoaderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint;
    cfg_main.GlyphOffset = ImVec2(0, -1);
    cfg_main.SizePixels = 14.0f;
    io.Fonts->AddFontFromMemoryCompressedTTF(font_exo2_regular_compressed_data, font_exo2_regular_compressed_size, cfg_main.SizePixels, &cfg_main, io.Fonts->GetGlyphRangesDefault());
    
    ImFontConfig cfg_menu_bar;
    cfg_menu_bar.FontLoaderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint;
    cfg_menu_bar.GlyphOffset = ImVec2(0, -2);
    cfg_menu_bar.SizePixels = 16.0f;
    io.Fonts->AddFontFromMemoryCompressedTTF(font_exo2_medium_compressed_data, font_exo2_medium_compressed_size, cfg_menu_bar.SizePixels, &cfg_menu_bar, io.Fonts->GetGlyphRangesDefault());
    
    ImFontConfig cfg_menu_bar2;
    cfg_menu_bar2.FontLoaderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint;
    cfg_menu_bar2.GlyphOffset = ImVec2(0, -1);
    cfg_menu_bar2.SizePixels = 14.0f;
    io.Fonts->AddFontFromMemoryCompressedTTF(font_exo2_medium_compressed_data, font_exo2_medium_compressed_size, cfg_menu_bar2.SizePixels, &cfg_menu_bar2, io.Fonts->GetGlyphRangesDefault());

    ImFontConfig cfg_code;
    cfg_code.FontLoaderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint;
    cfg_code.SizePixels = 12;
    cfg_code.OversampleH = 1;
    cfg_code.OversampleV = 1;
    io.Fonts->AddFontFromMemoryCompressedTTF(font_cascadia_mono_pl_regular_compressed_data, font_cascadia_mono_pl_regular_compressed_size, cfg_code.SizePixels, &cfg_code, io.Fonts->GetGlyphRangesDefault());
    
    // Setup Platform/Renderer backends
    if (!ImGui_ImplWin32_Init(hwnd))
    {
        return false;
    }

    if (!ImGui_ImplDX11_Init(device, device_context))
    {
        return false;
    }

    m_pTextEditor = new CTextEditor();
    m_pTextEditor->SetLanguageDefinition(CTextEditor::LanguageDefinitionId::Lua);
    m_pTextEditor->SetPalette(CTextEditor::PaletteId::Dark);

    m_vKeys.push_back({ "Toggle Menu", 0, VK_INSERT });
    m_vKeys.push_back({ "Enable Aimbot", 0, VK_DELETE });
    m_vKeys.push_back({ "Enable ESP", 0, VK_UP });

	Scripts.m_vScripts.push_back("Script 1");
	Scripts.m_vScripts.push_back("Script 2");
	Scripts.m_vScripts.push_back("Script 3");
	Scripts.m_vScripts.push_back("Script 4");
	Scripts.m_vScripts.push_back("Script 5");

	return true;
}

void CGui::Shutdown()
{
    if (m_pTextEditor != nullptr) {
        delete m_pTextEditor;
        m_pTextEditor = 0;
    }
    
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    if (m_pTextures != nullptr) {
		delete m_pTextures;
        m_pTextures = 0;
    }
}

void CGui::Render(const char* title, POINTS size, bool* b_open)
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    static ImVec2 g_MenuSize   = ImVec2(529, 418);
    static ImVec2 g_MenuPos    = ImVec2();

    const int LogoSize = 32;
    float HeaderHeight = LogoSize + style.WindowPadding.x * 2;

    // Menu Animation
    static float fAnimDur = 0.1f;

    fFadeAlpha = ImLerp<float>(fFadeAlpha, bDrawMenu ? 1.0f : 0.0f, 1.0f / fAnimDur * ImGui::GetIO().DeltaTime);
    style.Alpha = fFadeAlpha;

    // Console Animation
    bDrawConsole = m_iCurrentTab == 4; // Change the index to match the script page id
    fConsoleProgress = ImLerp<float>(fConsoleProgress, bDrawConsole ? 1.0f : 0.0f, 1.0f / fAnimDur * ImGui::GetIO().DeltaTime);
    
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    {
        // Always Draw 
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
        {
            ImGui::SetNextWindowPos(style.WindowPadding);
            ImGui::SetNextWindowSize(ImVec2(ImGui::CalcTextSize("Toggle Menu").x + style.FramePadding.x * 2, ImGui::GetFrameHeight() + ImGui::GetFontSize() * 2 + style.ItemSpacing.y * 2), ImGuiCond_Once);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::Begin("debug", b_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize);
            ImGui::PopStyleVar(2);
            {
                if (ImAdd::Button("Toggle Menu"))
                {
                    bDrawMenu = !bDrawMenu;
                }

                ImGui::Text("%.1f", fFadeAlpha);
                ImGui::Text(bDrawMenu ? "Draw" : "Hide");
            }
            ImGui::End();

            static ImVec2 WaterMarkSize = ImVec2(180.0f, ImGui::GetFontSize() + style.WindowPadding.y * 2);

            ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - WaterMarkSize.x - style.WindowPadding.x, style.WindowPadding.y), ImGuiCond_Always);
            ImGui::SetNextWindowSize(WaterMarkSize, ImGuiCond_Once);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            style.WindowShadowSize = 50;
            ImGui::Begin("watermark", b_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize);
            style.WindowShadowSize = 0;
            ImGui::PopStyleVar(2);
            // Rendering
            {
                ImVec2 pos = ImGui::GetWindowPos();
                ImVec2 size = ImGui::GetWindowSize();
                ImDrawList* pDrawList = ImGui::GetWindowDrawList();

                pDrawList->AddRectFilled(pos, pos + size, ImGui::GetColorU32(ImGuiCol_WindowBg), style.WindowRounding);

                if (style.WindowBorderSize > 0)
                {
                    pDrawList->AddRect(pos, pos + size, ImGui::GetColorU32(ImGuiCol_SliderGrab), style.WindowRounding);
                }
            }
            // content
            {
                ImGui::BeginChild("Main", ImGui::GetWindowSize(), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoBackground);
                {
                    ImGui::Image(m_pTextures->tLogoSmall, ImVec2(ImGui::GetFontSize(), ImGui::GetFontSize()));

                    std::string fps_label = std::string(std::to_string(int(io.Framerate)) + " :fps");

                    ImGui::SameLine(ImGui::GetWindowHeight());
                    ImAdd::VSeparator(0.0f, 2.0f);
                    ImGui::SameLine();
                    ImGui::TextDisabled("beta");
                    ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::CalcTextSize(fps_label.c_str()).x - style.WindowPadding.x);
                    ImGui::TextDisabled(fps_label.c_str());
                }
                ImGui::EndChild();
            }
            ImGui::End();

            ImVec2 KeyBindListSize = ImVec2(220.0f, ImGui::GetFontSize() + style.WindowPadding.y * 4 + ImGui::GetFontSize() * m_vKeys.size() + style.ItemInnerSpacing.y * (m_vKeys.size() - 1) + 2);

            ImGui::SetNextWindowPos(ImVec2(style.WindowPadding.x, size.y / 2 - KeyBindListSize.y / 2), ImGuiCond_Once);
            ImGui::SetNextWindowSize(KeyBindListSize, ImGuiCond_Always);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            style.WindowShadowSize = 50;
            ImGui::Begin("KeyBindList", b_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize);
            style.WindowShadowSize = 0;
            ImGui::PopStyleVar(2);
            // Rendering
            {
                ImVec2 pos = ImGui::GetWindowPos();
                ImVec2 size = ImGui::GetWindowSize();
                ImDrawList* pDrawList = ImGui::GetWindowDrawList();

                pDrawList->AddRectFilled(pos, pos + size, ImGui::GetColorU32(ImGuiCol_WindowBg), style.WindowRounding);

                if (style.WindowBorderSize > 0)
                {
                    pDrawList->AddRect(pos, pos + size, ImGui::GetColorU32(ImGuiCol_SliderGrab), style.WindowRounding);
                }
            }
            // content
            {
                ImGui::PushStyleColor(ImGuiCol_Border, style.Colors[ImGuiCol_SliderGrab]);
                ImAdd::BeginChild("Keybinds", ImGui::GetWindowSize());
                ImGui::PopStyleColor();
                {
                    for (int i = 0; i < m_vKeys.size(); i++)
                    {
                        ImAdd::DoubleText(style.Colors[ImGuiCol_Text], style.Colors[ImGuiCol_Text], std::string(m_vKeys[i].label + " [" + (m_vKeys[i].iMode == 0 ? "HOLD" : m_vKeys[i].iMode == 1 ? "TOGGLE" : "ALWAYS") + "]").c_str(), szKeyNames[m_vKeys[i].iKey]);
                    }
                }
                ImAdd::EndChild();
            }
            ImGui::End();
        }
        ImGui::PopStyleVar();

        if (style.Alpha > 0.1f)
        {
            ImGui::SetNextWindowSize(g_MenuSize , ImGuiCond_Once);
            ImGui::SetNextWindowPos(io.DisplaySize / 2 - g_MenuSize  / 2, ImGuiCond_Once);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            style.WindowShadowSize = 50;
            ImGui::Begin(title, b_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize);
            style.WindowShadowSize = 0;
            ImGui::PopStyleVar(2);
            // Particles
            {
                static Particles_CFG config;
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

                m_pParticles->Setup(config);
            }
            // Rendering
            {
                ImVec2 pos = ImGui::GetWindowPos();
                g_MenuPos = pos;
                ImVec2 size = ImGui::GetWindowSize();
                g_MenuSize = size;
                ImDrawList* pDrawList = ImGui::GetWindowDrawList();

                pDrawList->AddRectFilledMultiColorRounded(pos, pos + ImVec2(size.x, HeaderHeight), ImGui::GetColorU32(ImGuiCol_MenuBarBg), ImGui::GetColorU32(ImGuiCol_MenuBarBg), ImGui::GetColorU32(ImGuiCol_ButtonActive), ImGui::GetColorU32(ImGuiCol_ButtonActive), style.WindowRounding, ImDrawFlags_RoundCornersTop);


                pDrawList->PushClipRect(pos, pos + ImVec2(size.x, HeaderHeight));
                m_pParticles->Draw();
                pDrawList->PopClipRect();


                pDrawList->AddRectFilled(pos + ImVec2(0, HeaderHeight), pos + size, ImGui::GetColorU32(ImGuiCol_WindowBg), style.WindowRounding, ImDrawFlags_RoundCornersBottom);

                if (style.WindowBorderSize > 0)
                {
                    pDrawList->AddLine(pos + ImVec2(0, HeaderHeight - style.WindowBorderSize), pos + ImVec2(size.x, HeaderHeight - style.WindowBorderSize), ImGui::GetColorU32(ImGuiCol_SliderGrab));
                    pDrawList->AddLine(pos + ImVec2(0, HeaderHeight), pos + ImVec2(size.x, HeaderHeight), ImGui::GetColorU32(ImGuiCol_SliderGrab));

                    pDrawList->AddRect(pos, pos + size, ImGui::GetColorU32(ImGuiCol_SliderGrab), style.WindowRounding);
                }

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

                ImVec2 mouse_pos = io.MousePos;
                float  circle_radius = dots_pad * 10;
                float circle_radius_sq = circle_radius * circle_radius;

                for (int i = 0; i < w_dots_count + 1; i++)
                {
                    for (int j = 0; j < h_dots_count + 1; j++)
                    {
                        ImVec2 dot_pos = pos + ImVec2(
                            style.WindowBorderSize + i * dots_pad + 2,
                            HeaderHeight + style.WindowBorderSize + j * dots_pad + 1
                        );

                        ImVec2 delta = dot_pos - mouse_pos;
                        float distance_sq = delta.x * delta.x + delta.y * delta.y;

                        if (distance_sq < circle_radius_sq)
                        {
                            float distance = sqrtf(distance_sq);
                            float alpha = 1.0f - distance / circle_radius;

                            pDrawList->AddRectFilled(dot_pos, dot_pos + ImVec2(1, 1), ImGui::GetColorU32(ImGuiCol_Text, alpha));
                        }
                    }
                }
            }
            // Content
            {
                ImGui::SetCursorScreenPos(ImGui::GetWindowPos());
                ImGui::BeginChild("Header", ImVec2(ImGui::GetWindowWidth(), HeaderHeight), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoBackground);
                {
                    ImGui::SetCursorPosX(style.WindowPadding.x + 3);
                    ImGui::Image(m_pTextures->tLogo, ImVec2(LogoSize, LogoSize));

                    float fRadiosWidth = 0;

                    ImGui::PushFont(io.Fonts->Fonts[ImFont_MenuBar]);
                    {
                        for (int i = 0; i < m_vTabs.size(); i++)
                        {
                            fRadiosWidth += ImGui::CalcTextSize(m_vTabs[i].c_str()).x;
                        }

                        fRadiosWidth += style.ItemSpacing.x * (m_vTabs.size() - 1);

                        ImGui::SameLine(ImGui::GetWindowHeight() + 6);
                        ImAdd::VSeparator(style.FramePadding.y, 2.0f);
                        ImGui::SameLine();

                        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, (ImGui::GetWindowHeight() - style.WindowPadding.y * 2 - ImGui::GetFontSize()) / 2));
                        ImGui::BeginChild("Navs", ImVec2(fRadiosWidth, 0), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoBackground);
                        {
                            for (int i = 0; i < m_vTabs.size(); i++)
                            {
                                ImAdd::RadioFrameText(m_vTabs[i].c_str(), &m_iCurrentTab, i, true, true);
                                if (i < m_vTabs.size() - 1)
                                {
                                    ImGui::SameLine();
                                }
                            }
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleVar();

                        ImGui::SameLine();
                        ImAdd::VSeparator(style.FramePadding.y, 2.0f);
                        ImGui::SameLine();

                        ImGui::SetCursorPosY(style.WindowPadding.y + 3.0f);

                        ImGui::PushFont(io.Fonts->Fonts[ImFont_MenuBar2]);
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, style.Colors[ImGuiCol_ChildBg]);
                        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.0f, 6.0f));
                        ImGui::PushItemWidth(ImGui::GetWindowWidth() - LogoSize - 6 - style.WindowPadding.x * 2 - style.ItemSpacing.x * 4 - 4.0f - fRadiosWidth - 4);
                        ImAdd::InputText((ImTextureID)m_pTextures->tSearch, "search", m_szSearch, IM_ARRAYSIZE(m_szSearch));
                        ImGui::PopItemWidth();
                        ImGui::PopStyleVar();
                        ImGui::PopStyleColor();
                        ImGui::PopFont();
                    }
                    ImGui::PopFont();
                }
                ImGui::EndChild();

                ImGui::SetCursorScreenPos(ImGui::GetWindowPos() + ImVec2(style.WindowBorderSize, HeaderHeight + style.WindowBorderSize * 2));
                ImGui::BeginChild("Main", ImVec2(ImGui::GetWindowWidth() - style.WindowBorderSize * 3, ImGui::GetWindowHeight() - HeaderHeight - style.WindowBorderSize * 2), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoBackground);
                {
                    float fGroupWidth = std::round((ImGui::GetWindowWidth() - style.WindowPadding.x * 3) / 2);

                    if (m_iCurrentTab == 0)
                    {
                        ImGui::BeginGroup();
                        {
                            static int iCurrentSubTab1 = 0;

                            ImAdd::BeginTabChild(
                                "aimbot",
                                &iCurrentSubTab1,
                                { "test1", "test2", "test3" },
                                ImVec2(
                                    fGroupWidth,
                                    /* This is the most important part of making a group/child : calculating the height of it, its not hard as it seems, just follow the commends and u will get it */
                                    ImGui::GetFrameHeight() +   // the child header/title frame
                                    style.WindowPadding.y * 2 + // our top and bottom padding
                                    ImGui::GetFontSize() * 4 +  // both checkbox & color picker have a height = to font size, we got 4 
                                    style.ItemInnerSpacing.x * 3     // our total spacing between each widget, in general its items_count - 1, so in this case its 4 - 1 = 3, also we use ItemInnerSpacing.x instead of ItemSpacing.y
                                )
                            );
                            {
                                static bool bEnabled = true;
                                static int iEnableKey = 0;
                                static int iEnableType = 0;

                                static bool bDrawFoVCircle = false;
                                static ImVec4 v4FoVCC = ImVec4(1, 1, 1, 0.5f);
                                static bool bSyncWithSilent = false;
                                static bool bAutoScope = false;

                                ImGui::PushItemWidth(ImGui::CalcItemSize(ImVec2(-0.1f, 0), 0, 0).x - style.ItemSpacing.x * 2 - ImGui::GetFontSize() - ImGui::CalcTextSize(szKeyNames[iEnableKey]).x - style.CellPadding.x * 2);
                                {
                                    ImAdd::CheckBox("enable aimbot", &bEnabled);
                                }
                                ImGui::PopItemWidth();
                                ImGui::SameLine();
                                ImAdd::KeyBindEx("enable_aimbot", &iEnableKey, &iEnableType, ImGui::CalcTextSize(szKeyNames[iEnableKey]).x + style.CellPadding.x * 2);
                                ImGui::SameLine();
                                ImAdd::PopupButton("PopupTest2");
                                if (ImAdd::BeginPopup("PopupTest2"))
                                {
                                    ImGui::Text("Hello World !!!");
                                    ImAdd::EndPopup();
                                }

                                ImGui::PushItemWidth(ImGui::CalcItemSize(ImVec2(-0.1f, 0), 0, 0).x - style.ItemSpacing.x - ImGui::GetFontSize() * 2);
                                {
                                    ImAdd::CheckBox("draw fov circle", &bDrawFoVCircle);
                                    ImGui::SameLine();
                                    ImAdd::ColorEdit4("##FoVCC", (float*)&v4FoVCC);
                                }
                                ImGui::PopItemWidth();

                                ImGui::PushItemWidth(ImGui::CalcItemSize(ImVec2(-0.1f, 0), 0, 0).x);
                                {
                                    ImAdd::CheckBox("sync with silent", &bSyncWithSilent);
                                }
                                ImGui::PopItemWidth();

                                ImGui::PushItemWidth(ImGui::CalcItemSize(ImVec2(-0.1f, 0), 0, 0).x  - style.ItemSpacing.x - ImGui::GetFontSize());
                                {
                                    ImAdd::CheckBox("auto scope", &bAutoScope);
                                    ImGui::SameLine();
                                    ImAdd::PopupButton("PopupNameTest");

                                    if (ImAdd::BeginPopup("PopupNameTest")) 
                                    {
                                        ImAdd::BeginChild("Title Here", ImVec2(200, 160), true, true, true);
                                        {
                                            ImGui::PushItemWidth(ImGui::CalcItemSize(ImVec2(-0.1f, 0), 0, 0).x);
                                            {
                                                static float fSlider = 3.0f;
                                                static int iSlider = 7;

                                                ImAdd::SliderFloat("Slider Float", &fSlider, 0.0f, 100.0f);
                                                ImAdd::SliderInt("Slider Int", &iSlider, 0, 100);
                                            }
                                            ImGui::PopItemWidth();
                                        }
                                        ImAdd::EndChild();
                                        ImAdd::EndPopup();
                                    }
                                }
                                ImGui::PopItemWidth();

                            }
                            ImAdd::EndTabChild();

                            ImAdd::BeginChild("misc", ImVec2(fGroupWidth, 0));
                            {
                                static int iAimbotMode = 0;
                                static ImVec4 v4MatC1 = ImVec4(1, 0, 0, 0.5f);
                                static ImVec4 v4MatC2 = ImVec4(1, 0, 1, 0.5f);
                                static std::vector<int> vFilter = { 0, 0, 0, 0 };
                                static int iKey = VK_INSERT;
                                static int iKeyMethod = 0;

                                ImAdd::Label("example combo", ImVec2(ImGui::CalcItemSize(ImVec2(-0.1f, 0), 0, 0).x - style.ItemSpacing.x - 80.0f, ImGui::GetFontSize() + style.CellPadding.y * 2), ImVec2(0.0f, 0.5f));
                                ImGui::SameLine();
                                ImAdd::Combo("aimbotmode", &iAimbotMode, "smooth\0" "lock\0", 80.0f);

                                ImGui::Text("example color pickers");
                                ImGui::SameLine(ImGui::CalcItemSize(ImVec2(-0.1f, 0), 0, 0).x - ImGui::GetFontSize() * 4 - style.ItemSpacing.x + style.WindowPadding.x);
                                ImAdd::ColorEdit4("col1", (float*)&v4MatC1);
                                ImGui::SameLine();
                                ImAdd::ColorEdit4("col2", (float*)&v4MatC2);

                                ImAdd::Label("example multi combo", ImVec2(ImGui::CalcItemSize(ImVec2(-0.1f, 0), 0, 0).x - style.ItemSpacing.x - 80.0f, ImGui::GetFontSize() + style.CellPadding.y * 2), ImVec2(0.0f, 0.5f));
                                ImGui::SameLine();
                                ImAdd::MultiCombo("filter", &vFilter, { "team", "dead", "visible", "self" }, 80.0f);
                            }
                            ImAdd::EndChild();
                        }
                        ImGui::EndGroup();
                        ImGui::SameLine();
                        ImGui::BeginGroup();
                        {
                            ImAdd::BeginChild("configs", ImVec2(0, 0));
                            {
                                static float fHitChance = 1.0f;
                                static float fHumanize = 1.0f;
                                static float fFoV = 90.0f;

                                ImGui::PushItemWidth(ImGui::CalcItemSize(ImVec2(-0.1f, 0), 0, 0).x);
                                {
                                    ImAdd::SliderFloat("hit Chance", &fHitChance, 0.0f, 100.0f, "%.0f%%");
                                    ImAdd::SliderFloat("humanize", &fHumanize, 0.0f, 100.0f, "%.0f%%");
                                    ImAdd::SliderFloat("field of view", &fFoV, 0.0f, 180.0f, "%.0f deg");
                                }
                                ImGui::PopItemWidth();
                            }
                            ImAdd::EndChild();
                        }
                        ImGui::EndGroup();
                    }
                    else if (m_iCurrentTab == 1)
                    {

                    }
                    else if (m_iCurrentTab == 2)
                    {

                    }
                    else if (m_iCurrentTab == 3)
                    {
                        ImGui::BeginGroup();
                        {
                            ImAdd::BeginChild("menu", ImVec2(fGroupWidth, ImGui::GetFrameHeight() + style.WindowPadding.y * 2 + ImGui::GetFontSize()));
                            {
                                static int iMenuKey = VK_INSERT;
                                
                                ImGui::Text("menu key");
                                ImGui::SameLine(ImGui::CalcItemSize(ImVec2(-0.1f, 0.0f), 0.0f, 0.0f).x - ImGui::CalcTextSize(szKeyNames[iMenuKey]).x - style.CellPadding.x * 2 + style.WindowPadding.x);
                                ImAdd::KeyBind("enable_aimbot", &iMenuKey, ImGui::CalcTextSize(szKeyNames[iMenuKey]).x + style.CellPadding.x * 2);
                            }
                            ImAdd::EndChild();
                        }
                        ImGui::EndGroup();
                        ImGui::SameLine();
                        ImGui::BeginGroup();
                        {
                            ImAdd::BeginChild("configs", ImVec2(0, 0));
                            {
                                ImGui::Text("list");
                                ImGui::BeginChild("Configs##List", ImVec2(0, ImGui::GetWindowHeight() - ImGui::GetFontSize() - ImGui::GetFrameHeight() * 2 - style.ItemSpacing.y * 3 - style.WindowPadding.y * 2), ImGuiChildFlags_Border);
                                {

                                }
                                ImGui::EndChild();
                                ImGui::PushItemWidth(ImGui::GetWindowWidth() - ImGui::CalcTextSize("create").x - style.FramePadding.x * 2 - style.ItemSpacing.x - style.WindowPadding.x * 2);
                                static char szNewCfg[32] = "";
                                ImAdd::InputText(0, "new config", szNewCfg, IM_ARRAYSIZE(szNewCfg));
                                ImGui::PopItemWidth();
                                ImGui::SameLine();
                                ImAdd::Button("create");

                                float fBtnWidth = std::round((ImGui::GetWindowWidth() - style.WindowPadding.x * 3) / 2);
                                ImAdd::Button("save", ImVec2(fBtnWidth, 0.0f));
                                ImGui::SameLine();
                                ImAdd::Button("load", ImVec2(-0.1f, 0.0f));
                            }
                            ImAdd::EndChild();
                        }
                        ImGui::EndGroup();
                    }
                    else if (m_iCurrentTab == 4)
                    {
                        float fGroupWidth = 138;

                        ImGui::BeginChild("SideBar", ImVec2(fGroupWidth, 0), ImGuiChildFlags_None, ImGuiWindowFlags_NoBackground);
                        {
							static char szScriptSearch[64] = { 0 };

                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.FramePadding.x, style.FramePadding.y / 2));
                            ImGui::PushItemWidth(ImGui::GetWindowWidth());
                            {
                                ImAdd::InputText((ImTextureID)m_pTextures->tSearch, "search", szScriptSearch, IM_ARRAYSIZE(szScriptSearch));
                                
                                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, style.FramePadding.y));
                                ImGui::BeginChild("ScriptsList", ImVec2(0, ImGui::GetWindowHeight() - ImGui::GetFrameHeight() * 2 - style.ItemSpacing.y * 2), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoBackground);
                                // Background Shadow
                                {
                                    ImVec2 pos = ImGui::GetWindowPos();
                                    ImVec2 size = ImGui::GetWindowSize();
                                    ImDrawList* pDrawList = ImGui::GetForegroundDrawList();

                                    if (ImGui::GetCurrentWindow()->ScrollbarY > 0)
                                    {
                                        pDrawList->AddRectFilledMultiColor(
                                            pos, pos + ImVec2(size.x, style.WindowPadding.y),
                                            ImGui::GetColorU32(ImGuiCol_WindowBg), ImGui::GetColorU32(ImGuiCol_WindowBg),
                                            ImGui::GetColorU32(ImGuiCol_WindowBg, 0.0f), ImGui::GetColorU32(ImGuiCol_WindowBg, 0.0f)
                                        );
                                    }

                                    pDrawList->AddRectFilledMultiColor(
                                        pos + ImVec2(0.0f, size.y - style.WindowPadding.y), pos + size,
                                        ImGui::GetColorU32(ImGuiCol_WindowBg, 0.0f), ImGui::GetColorU32(ImGuiCol_WindowBg, 0.0f),
                                        ImGui::GetColorU32(ImGuiCol_WindowBg), ImGui::GetColorU32(ImGuiCol_WindowBg)
                                    );
                                }
                                // Content
                                {
                                    for (int i = 0; i < Scripts.m_vScripts.size(); i++)
                                    {
										std::string script_name = "  " + Scripts.m_vScripts[i];

                                        //                                                                          instead of true, check if script is running
                                        if (ImAdd::SelectableLabel(ImGui::GetWindowDrawList(), script_name.c_str(), true, ImVec2(ImGui::GetWindowWidth() - ImGui::GetFontSize() * 2 - style.ItemSpacing.x * 2, ImGui::GetFontSize())))
                                        {

                                        }

                                        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                                        {
                                            ImGui::SameLine();
                                            if (ImAdd::ButtonIcon(std::string("##Edit_" + std::to_string(i)).c_str(), (ImTextureID)m_pTextures->tEdit, ImVec2(ImGui::GetFontSize(), ImGui::GetFontSize()), 0, 0, false))
                                            {

                                            }

                                            ImGui::SameLine();
                                            if (ImAdd::ButtonIcon(std::string("##Remove_" + std::to_string(i)).c_str(), (ImTextureID)m_pTextures->tTrash, ImVec2(ImGui::GetFontSize(), ImGui::GetFontSize()), 0, 0, false))
                                            {

                                            }
                                        }
                                        ImGui::PopStyleVar();
                                    }
                                }
                                ImGui::EndChild();
                                ImGui::PopStyleVar();

                                ImGui::BeginChild("Controls", ImVec2(ImGui::GetWindowWidth(), ImGui::GetFrameHeight()), ImGuiChildFlags_None, ImGuiWindowFlags_NoBackground);
                                {
                                    ImAdd::ButtonIcon("  Import", (ImTextureID)m_pTextures->tUpload, ImVec2(ImGui::GetWindowWidth() - style.ItemSpacing.x * 2 - 1.0f - ImGui::GetFrameHeight(), ImGui::GetFrameHeight()));
                                    ImGui::SameLine();
                                    ImAdd::VSeparator(style.FrameRounding, 1.0f);
                                    ImGui::SameLine();
                                    if (ImAdd::ButtonIcon("##Create", (ImTextureID)m_pTextures->tPlus, ImVec2(ImGui::GetFrameHeight(), ImGui::GetFrameHeight())))
                                    {
										ImGui::OpenPopup("CreateScriptPopup");
                                    }

                                    if (ImGui::BeginPopup("CreateScriptPopup", ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove))
                                    {
                                        ImVec2 PopupSize = ImVec2(260, ImGui::GetFrameHeight() * 2 + style.WindowPadding.y * 2) + style.WindowPadding * 2;

                                        ImGui::SetWindowSize(PopupSize, ImGuiCond_Always);
                                        ImGui::SetWindowPos(io.DisplaySize / 2 - PopupSize / 2, ImGuiCond_Always);

										ImGui::PushStyleColor(ImGuiCol_ChildBg, style.Colors[ImGuiCol_WindowBg]);
                                        ImAdd::BeginChild("New Script", PopupSize - style.WindowPadding * 2);
                                        ImGui::PopStyleColor();
                                        {
											static char szNewScriptName[64] = { 0 };
                                            
                                            ImGui::PushItemWidth(ImGui::GetWindowWidth() - style.WindowPadding.x * 2 - style.ItemSpacing.x - ImGui::CalcTextSize(" create ").x - style.FramePadding.x * 2);
                                            {
                                                ImAdd::InputText(0, "Script Name", szNewScriptName, IM_ARRAYSIZE(szNewScriptName));
											}

                                            ImGui::SameLine();

                                            if (ImAdd::Button(" create "))
                                            {
                                                if (strlen(szNewScriptName) > 0)
                                                {
                                                    Scripts.m_vScripts.push_back(szNewScriptName);
                                                    memset(szNewScriptName, 0, sizeof(szNewScriptName));
                                                }
                                                
                                                ImGui::CloseCurrentPopup();
                                            }
                                        }
                                        ImAdd::EndChild();

                                        ImGui::EndPopup();
									}
                                }
                                ImGui::EndChild();
                            }
                            ImGui::PopItemWidth();
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::SameLine();
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, style.Colors[ImGuiCol_Separator]);
                        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, style.Colors[ImGuiCol_Separator]);
                        ImAdd::VSeparator(style.FrameRounding, 1.0f);
                        ImGui::PopStyleColor(2);
                        ImGui::SameLine();
                        ImGui::BeginChild("Editor", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_NoBackground);
                        {
                            /*
                            ImGui::BeginChild("Editor::Controls##Top", ImVec2(0, ImGui::GetFrameHeight()), ImGuiChildFlags_None, ImGuiWindowFlags_NoBackground);
                            {
                                ImAdd::ButtonIcon("##Undo", m_pTextures->tUndo);
                                ImGui::SameLine();
                                ImAdd::ButtonIcon("##Redo", m_pTextures->tRedo);
                                ImGui::SameLine();
                                ImAdd::VSeparator(style.FrameRounding, 1.0f);
                                ImGui::SameLine();
                                ImAdd::ButtonIcon("##cut", m_pTextures->tCut);
                                ImGui::SameLine();
                                ImAdd::ButtonIcon("##copy", m_pTextures->tCopy);
                                ImGui::SameLine();
                                ImAdd::ButtonIcon("##paste", m_pTextures->tPaste);
                                ImGui::SameLine();
                                ImAdd::VSeparator(style.FrameRounding, 1.0f);
                                ImGui::SameLine();
                                ImAdd::ButtonIcon("##save", m_pTextures->tSave);
                            }
                            ImGui::EndChild();
                            */
                            ImGui::BeginChild("Editor::EditorFrame", ImVec2(0, ImGui::GetWindowHeight() - ImGui::GetFrameHeight() - style.ItemSpacing.y), ImGuiChildFlags_None, ImGuiWindowFlags_NoBackground);
                            {
                                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, ImGui::GetFontSize() / 2));
                                ImGui::PushFont(io.Fonts->Fonts[ImFont_Code]);
                                m_pTextEditor->Render("CodeEditor", false, ImVec2(0, 0), true);
                                ImGui::PopFont();
                                ImGui::PopStyleVar();
                            }
                            ImGui::EndChild();

                            ImGui::BeginChild("Editor::Controls##Bottom", ImVec2(0, ImGui::GetFrameHeight()), ImGuiChildFlags_None, ImGuiWindowFlags_NoBackground);
                            {
                                //ImAdd::ButtonIcon(" Export  ", m_pTextures->tDownload);
                                //ImGui::SameLine();
                                ImAdd::ButtonIcon(" Save  ", (ImTextureID)m_pTextures->tSave);

								ImGui::SameLine(ImGui::GetWindowWidth() - style.FramePadding.x * 3 - ImGui::CalcTextSize(" Run  ").x - ImGui::GetFontSize());
                                ImAdd::ButtonIcon(" Run  ", (ImTextureID)m_pTextures->tPlay);
                            }
                            ImGui::EndChild();
                        }
                        ImGui::EndChild();
                    }
                }
                ImGui::EndChild();
            }
            ImGui::End();
        }

        if (fConsoleProgress >= 0.1f)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
            {
                //ImGui::SetNextWindowPos(g_MenuPos + ImVec2(g_MenuSize.x + style.WindowPadding.x + style.ItemInnerSpacing.x * 2, 0.0f), ImGuiCond_Always);
                ImGui::SetNextWindowPos(g_MenuPos + ImVec2((g_MenuSize.x + style.WindowPadding.x + style.ItemInnerSpacing.x * 2) * fConsoleProgress, 0.0f), ImGuiCond_Always);
                ImGui::SetNextWindowSize(ImVec2(230, g_MenuSize.y), ImGuiCond_Always);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
                ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoFocusOnAppearing);
                ImGui::PopStyleVar(2);
                // Render
                {
                    float HeaderHeight = ImGui::GetFrameHeight();

                    ImVec2 pos = ImGui::GetWindowPos();
                    ImVec2 size = ImGui::GetWindowSize();
                    ImDrawList* pDrawList = ImGui::GetWindowDrawList();

                    float fGlowAlphaT = 0.0f;
                    float fGlowAlphaB = 0.1f;

                    //pDrawList->AddRectFilled(pos,  pos + ImVec2(size.x, HeaderHeight), ImGui::GetColorU32(ImGuiCol_MenuBarBg), style.ChildRounding, ImDrawFlags_RoundCornersTop);

                    pDrawList->AddRectFilled(pos + ImVec2(0, HeaderHeight), pos + size, ImGui::GetColorU32(ImGuiCol_ChildBg), style.ChildRounding, ImDrawFlags_RoundCornersBottom);

                    pDrawList->AddRectFilledMultiColorRounded(pos, pos + ImVec2(size.x, HeaderHeight), ImGui::GetColorU32(ImGuiCol_MenuBarBg), ImGui::GetColorU32(ImGuiCol_MenuBarBg), ImGui::GetColorU32(ImGuiCol_ButtonActive), ImGui::GetColorU32(ImGuiCol_ButtonActive), style.ChildRounding, ImDrawFlags_RoundCornersTop);
                
                    pDrawList->AddText(pos + style.FramePadding, ImGui::GetColorU32(ImGuiCol_Text), "Console");

                    if (style.ChildBorderSize > 0)
                    {
                        pDrawList->AddLine(pos + ImVec2(0, HeaderHeight - style.ChildBorderSize), pos + ImVec2(size.x, HeaderHeight - style.ChildBorderSize), ImGui::GetColorU32(ImGuiCol_SliderGrab));
                        pDrawList->AddLine(pos + ImVec2(0, HeaderHeight), pos + ImVec2(size.x, HeaderHeight), ImGui::GetColorU32(ImGuiCol_SliderGrabActive));

                        pDrawList->AddRect(pos, pos + size, ImGui::GetColorU32(ImGuiCol_SliderGrab), style.ChildRounding);
                    }

                    //if (dots_bg)
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
                }
                // Content
                {
                    ImGui::SetCursorScreenPos(ImGui::GetWindowPos() + ImVec2(0, ImGui::GetFrameHeight()));
                    ImGui::BeginChild("Console##Group", ImVec2(), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoBackground);
                    {
                        ImGui::BeginChild("Console##Prints", ImVec2(0, ImGui::GetWindowHeight() - ImGui::GetFrameHeight() - style.WindowPadding.y * 2 - style.ItemSpacing.y), ImGuiChildFlags_Border);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, style.ItemInnerSpacing);
                            {
                                ImGui::Text("Hello World !");
                                ImGui::TextColored(ImVec4(0.9, 0.1, 0.1, 1), "Some error maybe");
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        
                        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::GetFrameHeight() - style.WindowPadding.x);
                        ImAdd::ButtonIcon("##Clear", (ImTextureID)m_pTextures->tClear, ImVec2(ImGui::GetFrameHeight(), ImGui::GetFrameHeight()));
                    }
                    ImGui::EndChild();
                }
                ImGui::End();
            }
            ImGui::PopStyleVar();
        }
    }
    ImGui::Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool CGui::MsgProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    return ImGui_ImplWin32_WndProcHandler(hwnd, umsg, wparam, lparam);
}
