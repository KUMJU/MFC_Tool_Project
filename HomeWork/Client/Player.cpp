#include "pch.h"
#include "Player.h"
#include "Device.h"
#include "TextureMgr.h"

CPlayer::CPlayer()
    :m_eCurState(PLAYER_STATE::NONE),m_ePrevState(PLAYER_STATE::NONE)
{
    m_vDstPos = { 400.f,300.f,0.f };
}

CPlayer::~CPlayer()
{
    Release();
}

HRESULT CPlayer::Initialize(void)
{
    m_dwTime = GetTickCount();
    m_wstrObjKey = L"Player";
    m_wstrStateKey = L"STAND_LDOWN";
    m_tFrame.fMax = CTextureMgr::Get_Instance()->Get_TextureCnt(m_wstrObjKey.c_str(), m_wstrStateKey.c_str());
    m_tInfo.vPos = { 400,300,0 };
    m_tInfo.vDir = { 0,0,0 };
    m_tInfo.vLook = { 0,-1,0 };
	return S_OK;
}

int CPlayer::Update(void)
{
    Key_Input();
    Update_Move();
    Update_State();
	return OBJ_NOEVENT;
}

void CPlayer::Late_Update(void)
{
    Move_Frame();
}

void CPlayer::Render(void)
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
        D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CPlayer::Release(void)
{
}

void CPlayer::Key_Input()
{
}

void CPlayer::Update_Move()
{
}

void CPlayer::Update_State()
{


}
