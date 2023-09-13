#include "pch.h"
#include "Player.h"
#include "Device.h"
#include "TextureMgr.h"
#include "AstarMgr.h"
#include "TimeMgr.h"

CPlayer::CPlayer()
    :m_eCurState(PLAYER_STATE::NONE),m_ePrevState(PLAYER_STATE::NONE)
{
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
    m_tInfo.vPos = { 700,500,0 };
    m_tInfo.vDir = { 0,0,0 };
    m_tInfo.vLook = { 0,-1,0 };

    m_spriteInfo = CTextureMgr::Get_Instance()-> GetSpriteInfo(m_wstrObjKey, m_wstrStateKey);
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
        D3DCOLOR_ARGB(m_spriteInfo.iA, m_spriteInfo.iR, m_spriteInfo.iG, m_spriteInfo.iB));

}

void CPlayer::Release(void)
{
}

void CPlayer::Key_Input()
{
    if (GetAsyncKeyState(VK_LBUTTON))
    {
        D3DXVECTOR3	vMouse = Get_Mouse() - m_vScroll;
        CAstarMgr::Get_Instance()->Start_Astar(m_tInfo.vPos, vMouse);
    }

    Move_Astar();
}

void CPlayer::Update_Move()
{
}

void CPlayer::Update_State()
{


}

void CPlayer::Move_Astar()
{
    list<TILE*>& BestList = CAstarMgr::Get_Instance()->Get_BestList();

    if (!BestList.empty())
    {
        D3DXVECTOR3 vDir = BestList.front()->vPos - m_tInfo.vPos;
        
        float fDist = D3DXVec3Length(&vDir);
        D3DXVec3Normalize(&vDir, &vDir);

        m_tInfo.vPos += vDir * 50.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

        if (3.f > fDist)
            BestList.pop_front();
    }
}
