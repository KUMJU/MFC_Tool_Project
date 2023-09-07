#include "pch.h"
#include "SingleTexture.h"
#include "Device.h"

CSingleTexture::CSingleTexture()
    :m_pTexInfo(nullptr)
{
}

CSingleTexture::~CSingleTexture()
{
    Release();
}

const TEXINFO* CSingleTexture::Get_Texture(const TCHAR* pStateKey, const int& iCnt)
{
    return m_pTexInfo;
}

HRESULT CSingleTexture::Insert_Texture(const TCHAR* pFilePath, const TCHAR* pStateKey, const int& iCnt)
{
    m_pTexInfo = new TEXINFO;
    ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

    if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &(m_pTexInfo->tImgInfo))))
    {
        Safe_Release(m_pTexInfo->pTexture);
        Safe_Delete(m_pTexInfo);
        AfxMessageBox(L"single D3DXGetImageInfoFromFile Failed");
        return E_FAIL;
    }

   	//D3DPOOL_DEFAULT = 0,	// 가장 적합한 메모리, 보통 그래픽 카드 메모리를 말함
	//D3DPOOL_MANAGED = 1,	// DX 3D에 의해 자원을 관리, 그래픽 메모리를 사용하지만 RAM에 백업
	//D3DPOOL_SYSTEMMEM = 2,	// 시스템 메모리(RAM)를 사용
	//D3DPOOL_SCRATCH = 3,	// 시스템 메모리를 사용하지만 DX 장치로 접근 불가

    if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(),
        pFilePath,
        m_pTexInfo->tImgInfo.Width,
        m_pTexInfo->tImgInfo.Height,
        m_pTexInfo->tImgInfo.MipLevels,
        0, // 버퍼의 사용 방식
        m_pTexInfo->tImgInfo.Format,
        D3DPOOL_MANAGED,
        D3DX_DEFAULT,
        D3DX_DEFAULT,
        0,
        nullptr,
        nullptr,
        &(m_pTexInfo->pTexture))))
    {
        Safe_Release(m_pTexInfo->pTexture);
        Safe_Delete(m_pTexInfo);
        AfxMessageBox(L"single D3DXCreateTextureFromFileEx Failed");
        return E_FAIL;
    }

    return S_OK;
}

void CSingleTexture::Release()
{
    m_pTexInfo->pTexture->Release();
    Safe_Delete(m_pTexInfo);
}
