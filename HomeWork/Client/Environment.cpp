#include "pch.h"
#include "Environment.h"
#include"Device.h"
#include"TextureMgr.h"

CEnvironment::CEnvironment()
{
}

CEnvironment::~CEnvironment()
{
}

HRESULT CEnvironment::Initialize(void)
{
    m_iImgCount = 0;
    m_tFrame.fSpeed = m_spriteInfo.fSpeed;

	return S_OK;
}

int CEnvironment::Update(void)
{
	return S_OK;
}

void CEnvironment::Late_Update(void)
{
}

void CEnvironment::Render(void)
{
    D3DXMATRIX  matWorld, matScale, matTrans;

    D3DXMatrixIdentity(&matWorld);

    D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

    D3DXMatrixTranslation(&matTrans,
        m_tInfo.vPos.x + m_vScroll.x,
        m_tInfo.vPos.y + m_vScroll.y,
        m_tInfo.vPos.z);

    matWorld = matScale * matTrans;

    CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

    const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), m_iImgCount);

    float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
    float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

    D3DXVECTOR3 vCenter(fCenterX, fCenterY, 0.f);
    CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
        nullptr,
        &vCenter,
        nullptr,
        D3DCOLOR_ARGB(m_spriteInfo.iA, m_spriteInfo.iR, m_spriteInfo.iG, m_spriteInfo.iB));
}

void CEnvironment::Release(void)
{
}
