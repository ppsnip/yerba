#include "CTextures.h"

#include "../Resources/Textures/image_logo.h"
#include "../Resources/Textures/image_logo_small.h"
#include "../Resources/Textures/image_search.h"
#include "../Resources/Textures/image_upload.h"
#include "../Resources/Textures/image_shop.h"
#include "../Resources/Textures/image_copy.h"
#include "../Resources/Textures/image_cut.h"
#include "../Resources/Textures/image_paste.h"
#include "../Resources/Textures/image_play.h"
#include "../Resources/Textures/image_plus.h"
#include "../Resources/Textures/image_redo.h"
#include "../Resources/Textures/image_undo.h"
#include "../Resources/Textures/image_save.h"
#include "../Resources/Textures/image_trash.h"
#include "../Resources/Textures/image_edit.h"
#include "../Resources/Textures/image_download.h"
#include "../Resources/Textures/image_clear.h"

CTextures::CTextures()
{
    pump0 = nullptr;

    tLogo = nullptr;
    tLogoSmall = nullptr;
    tSearch = nullptr;
    tUpload = nullptr;
    tShop = nullptr;
    tCopy = nullptr;
    tCut = nullptr;
    tPaste = nullptr;
    tPlay = nullptr;
    tPlus = nullptr;
    tRedo = nullptr;
    tUndo = nullptr;
    tSave = nullptr;
    tTrash = nullptr;
    tEdit = nullptr;
    tDownload = nullptr;
    tClear = nullptr;
}

CTextures::~CTextures()
{
}

bool CTextures::Initialize(ID3D11Device* pDevice)
{
    // Result of the Initialization
    HRESULT hResult;

    // Setup textures
    hResult = D3DX11CreateShaderResourceViewFromMemory(pDevice, logo_bytes, sizeof(logo_bytes), &info0, pump0, &tLogo, 0);
    if (FAILED(hResult))
        return false;
    
    hResult = D3DX11CreateShaderResourceViewFromMemory(pDevice, logo_small_bytes, sizeof(logo_small_bytes), &info0, pump0, &tLogoSmall, 0);
    if (FAILED(hResult))
        return false;
    
    hResult = D3DX11CreateShaderResourceViewFromMemory(pDevice, search_bytes, sizeof(search_bytes), &info0, pump0, &tSearch, 0);
    if (FAILED(hResult))
        return false;
    

    hResult = D3DX11CreateShaderResourceViewFromMemory(pDevice, upload_bytes, sizeof(upload_bytes), &info0, pump0, &tUpload, 0);
    if (FAILED(hResult))
        return false;
    
    
    hResult = D3DX11CreateShaderResourceViewFromMemory(pDevice, shop_bytes, sizeof(shop_bytes), &info0, pump0, &tShop, 0);
    if (FAILED(hResult))
        return false;
    
    
    hResult = D3DX11CreateShaderResourceViewFromMemory(pDevice, copy_bytes, sizeof(copy_bytes), &info0, pump0, &tCopy, 0);
    if (FAILED(hResult))
        return false;
    
    
    hResult = D3DX11CreateShaderResourceViewFromMemory(pDevice, cut_bytes, sizeof(cut_bytes), &info0, pump0, &tCut, 0);
    if (FAILED(hResult))
        return false;
    
    
    hResult = D3DX11CreateShaderResourceViewFromMemory(pDevice, paste_bytes, sizeof(paste_bytes), &info0, pump0, &tPaste, 0);
    if (FAILED(hResult))
        return false;
    
    
    hResult = D3DX11CreateShaderResourceViewFromMemory(pDevice, play_bytes, sizeof(play_bytes), &info0, pump0, &tPlay, 0);
    if (FAILED(hResult))
        return false;
    
    
    hResult = D3DX11CreateShaderResourceViewFromMemory(pDevice, plus_bytes, sizeof(plus_bytes), &info0, pump0, &tPlus, 0);
    if (FAILED(hResult))
        return false;
    
    
    hResult = D3DX11CreateShaderResourceViewFromMemory(pDevice, redo_bytes, sizeof(redo_bytes), &info0, pump0, &tRedo, 0);
    if (FAILED(hResult))
        return false;
    
    
    hResult = D3DX11CreateShaderResourceViewFromMemory(pDevice, undo_bytes, sizeof(undo_bytes), &info0, pump0, &tUndo, 0);
    if (FAILED(hResult))
        return false;
    
    
    hResult = D3DX11CreateShaderResourceViewFromMemory(pDevice, save_bytes, sizeof(save_bytes), &info0, pump0, &tSave, 0);
    if (FAILED(hResult))
        return false;
    
    
    hResult = D3DX11CreateShaderResourceViewFromMemory(pDevice, trash_bytes, sizeof(trash_bytes), &info0, pump0, &tTrash, 0);
    if (FAILED(hResult))
        return false;
    
    
    hResult = D3DX11CreateShaderResourceViewFromMemory(pDevice, edit_bytes, sizeof(edit_bytes), &info0, pump0, &tEdit, 0);
    if (FAILED(hResult))
        return false;
    
    hResult = D3DX11CreateShaderResourceViewFromMemory(pDevice, download_bytes, sizeof(download_bytes), &info0, pump0, &tDownload, 0);
    if (FAILED(hResult))
        return false;
    
    
    hResult = D3DX11CreateShaderResourceViewFromMemory(pDevice, clear_bytes, sizeof(clear_bytes), &info0, pump0, &tClear, 0);
    if (FAILED(hResult))
        return false;
    
    return true;
}

void CTextures::Shutdown()
{
    if (tLogo != nullptr) {
        tLogo->Release();
        tLogo = nullptr;
    }
    
    if (tLogoSmall != nullptr) {
        tLogoSmall->Release();
        tLogoSmall = nullptr;
    }

    if (tSearch != nullptr) {
        tSearch->Release();
        tSearch = nullptr;
    }

    if (tUpload != nullptr) {
        tUpload->Release();
        tUpload = nullptr;
    }

    if (tShop != nullptr) {
        tShop->Release();
        tShop = nullptr;
    }

    if (tCopy != nullptr) {
        tCopy->Release();
        tCopy = nullptr;
    }

    if (tCut != nullptr) {
        tCut->Release();
        tCut = nullptr;
    }

    if (tPaste != nullptr) {
        tPaste->Release();
        tPaste = nullptr;
    }

    if (tPlay != nullptr) {
        tPlay->Release();
        tPlay = nullptr;
    }

    if (tPlus != nullptr) {
        tPlus->Release();
        tPlus = nullptr;
    }

    if (tRedo != nullptr) {
        tRedo->Release();
        tRedo = nullptr;
    }

    if (tUndo != nullptr) {
        tUndo->Release();
        tUndo = nullptr;
    }

    if (tSave != nullptr) {
        tSave->Release();
        tSave = nullptr;
    }

    if (tTrash != nullptr) {
        tTrash->Release();
        tTrash = nullptr;
    }

    if (tEdit != nullptr) {
        tEdit->Release();
        tEdit = nullptr;
    }

    if (tDownload != nullptr) {
        tDownload->Release();
        tDownload = nullptr;
    }

    if (tClear != nullptr) {
        tClear->Release();
        tClear = nullptr;
    }
}
