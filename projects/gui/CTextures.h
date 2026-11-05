#pragma once

// DirectX
#include <D3DX11tex.h>

class CTextures
{
public:
	CTextures();
	~CTextures();

	bool Initialize(ID3D11Device* pDevice);
	void Shutdown();

public:
	ID3D11ShaderResourceView* tLogo;
	ID3D11ShaderResourceView* tLogoSmall;
	ID3D11ShaderResourceView* tSearch;
	ID3D11ShaderResourceView* tUpload;
	ID3D11ShaderResourceView* tShop;
	ID3D11ShaderResourceView* tCopy;
	ID3D11ShaderResourceView* tCut;
	ID3D11ShaderResourceView* tPaste;
	ID3D11ShaderResourceView* tPlay;
	ID3D11ShaderResourceView* tPlus;
	ID3D11ShaderResourceView* tRedo;
	ID3D11ShaderResourceView* tUndo;
	ID3D11ShaderResourceView* tSave;
	ID3D11ShaderResourceView* tTrash;
	ID3D11ShaderResourceView* tEdit;
	ID3D11ShaderResourceView* tDownload;
	ID3D11ShaderResourceView* tClear;

private:
	D3DX11_IMAGE_LOAD_INFO info0;
	ID3DX11ThreadPump* pump0;
};
