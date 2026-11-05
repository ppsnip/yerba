#pragma once

#include <d3d11.h>
#include <vector>
#include <string>

#include "CTextures.h"

enum ImFonts_ : int
{
	ImFont_Main = 0,
	ImFont_MenuBar,
	ImFont_MenuBar2,
	ImFont_Code
};

struct stKeyBind
{
	std::string label;
	int iMode;
	int iKey;
};

class CTextEditor;
class Particles;

class CGui
{
private:
	int m_iCurrentTab;
	std::vector<std::string> m_vTabs;
	std::vector<stKeyBind> m_vKeys;

public:
	CGui();
	~CGui();

	bool Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context);
	void Shutdown();
	void Render(const char* title, POINTS size, bool* b_open = 0);

	bool MsgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:
	ID3D11Device* GetDevice()				{ return m_pDevice; };
	ID3D11DeviceContext* GetDeviceContext() { return m_pDeviceContext; };
	HWND GetWindow()						{ return m_hWnd; };

private:
	HWND					m_hWnd;
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pDeviceContext;

private:
	CTextures* m_pTextures;
	Particles* m_pParticles;
	CTextEditor* m_pTextEditor;
	char m_szSearch[32];

private:
	bool bDrawMenu = true;
	float fFadeAlpha = 0.0f;
	bool bDrawConsole = false;
	float fConsoleProgress = 0.0f;

	struct
	{
		std::vector<std::string> m_vScripts;
	} Scripts; // temp
};
