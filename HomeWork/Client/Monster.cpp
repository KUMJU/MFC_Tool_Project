#include "pch.h"
#include "Monster.h"
#include"TextureMgr.h"
#include"Device.h"

CMonster::CMonster()
{
}

CMonster::~CMonster()
{
}

HRESULT CMonster::Initialize(void)
{

	return S_OK;
}

int CMonster::Update(void)
{
	return S_OK;
}

void CMonster::Late_Update(void)
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

    const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), static_cast<int>(m_tFrame.fFrame));

    float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
    float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

    D3DXVECTOR3 vCenter(fCenterX, fCenterY, 0.f);
    CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
        nullptr,
        &vCenter,
        nullptr,
        D3DCOLOR_ARGB(m_spriteInfo.iA, m_spriteInfo.iR, m_spriteInfo.iG, m_spriteInfo.iB));
}

void CMonster::Render(void)
{
}

void CMonster::Release(void)
{
}
