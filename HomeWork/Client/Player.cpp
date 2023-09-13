#include "pch.h"
#include "Player.h"
#include "Device.h"
#include "TextureMgr.h"
#include "AstarMgr.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "MyTerrain.h"

CPlayer::CPlayer()
    :m_ePrevState(STAND_DOWN), m_eCurrentState(STAND_DOWN)
{
}

CPlayer::~CPlayer()
{
    Release();
}

HRESULT CPlayer::Initialize(void)
{
    m_wstrObjKey = L"Player";
    m_wstrStateKey = L"STAND_LDOWN";
    m_tFrame.fMax = CTextureMgr::Get_Instance()->Get_TextureCnt(m_wstrObjKey.c_str(), m_wstrStateKey.c_str());
    m_tInfo.vPos = { 700,500,0 };
    m_tInfo.vDir = { 0,0,0 };
    m_tInfo.vLook = { 1,-1,0 };

    m_spriteInfo = CTextureMgr::Get_Instance()-> GetSpriteInfo(m_wstrObjKey, m_wstrStateKey);
    m_tFrame.fFrame = 0;
    m_tFrame.fMax = m_spriteInfo.iCount;
    m_tFrame.fSpeed = m_spriteInfo.fSpeed;

    return S_OK;
}

int CPlayer::Update(void)
{
    Key_Input();

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update(void)
{
    Update_State();
    Move_Frame();
}

void CPlayer::Render(void)
{
    D3DXMATRIX  matWorld, matScale, matTrans;

    D3DXMatrixIdentity(&matWorld);

    if (m_tInfo.vLook.x < 0)
        D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
    else
        D3DXMatrixScaling(&matScale, -1.f, 1.f, 1.f);

    D3DXMatrixTranslation(&matTrans,
        m_tInfo.vPos.x + m_vScroll.x,
        m_tInfo.vPos.y + m_vScroll.y,
        m_tInfo.vPos.z);

    matWorld = matScale * matTrans;

    CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

    const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_wstrObjKey.c_str(), m_wstrStateKey.c_str(),
        static_cast<int>(m_tFrame.fFrame));

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
        m_vDstPos = Get_Mouse() - m_vScroll;
        CAstarMgr::Get_Instance()->Start_Astar(m_tInfo.vPos, m_vDstPos);
    }

    Move_Astar();
}

void CPlayer::Update_State()
{
    float magnitude = D3DXVec3Length(&m_tInfo.vDir);

    // 이동 시
    if (magnitude > 0.1f)
    {
            // 위 대각
            if (m_tInfo.vLook.y < 0)
            {
                if (m_ePrevState == RUN_UP)
                    return;

                m_ePrevState = RUN_UP;

                m_wstrStateKey = L"RUN_LUP";
                m_spriteInfo = CTextureMgr::Get_Instance()->GetSpriteInfo(m_wstrObjKey, m_wstrStateKey);
                m_tFrame.fFrame = 0;
                m_tFrame.fMax = m_spriteInfo.iCount;
                m_tFrame.fSpeed = m_spriteInfo.fSpeed;
            }
            // 아래 대각
            else if (m_tInfo.vLook.y > 0)
            {
                if (m_ePrevState == RUN_DOWN)
                    return;
                m_ePrevState = RUN_DOWN;

                m_wstrStateKey = L"RUN_LDOWN";
                m_spriteInfo = CTextureMgr::Get_Instance()->GetSpriteInfo(m_wstrObjKey, m_wstrStateKey);
                m_tFrame.fFrame = 0;
                m_tFrame.fMax = m_spriteInfo.iCount;
                m_tFrame.fSpeed = m_spriteInfo.fSpeed;
            }
    }
    // 정지 시
    else
    {
        // 위 대각
        if (m_tInfo.vLook.y < 0)
        {
            if (m_ePrevState == STAND_UP)
                return;
            m_ePrevState = STAND_UP;

            m_wstrStateKey = L"STAND_LUP";
            m_spriteInfo = CTextureMgr::Get_Instance()->GetSpriteInfo(m_wstrObjKey, m_wstrStateKey);
            m_tFrame.fFrame = 0;
            m_tFrame.fMax = m_spriteInfo.iCount;
            m_tFrame.fSpeed = m_spriteInfo.fSpeed;
        }
        // 아래 대각
        else if (m_tInfo.vLook.y > 0)
        {
            if (m_ePrevState == STAND_DOWN)
                return;
            m_ePrevState = STAND_DOWN;

            m_wstrStateKey = L"STAND_LDOWN";
            m_spriteInfo = CTextureMgr::Get_Instance()->GetSpriteInfo(m_wstrObjKey, m_wstrStateKey);
            m_tFrame.fFrame = 0;
            m_tFrame.fMax = m_spriteInfo.iCount;
            m_tFrame.fSpeed = m_spriteInfo.fSpeed;
        }
    }
}

void CPlayer::Move_Astar()
{
    list<TILE*>& BestList = CAstarMgr::Get_Instance()->Get_BestList();

    if (!BestList.empty())
    {
        m_tInfo.vDir = BestList.front()->vPos - m_tInfo.vPos;
        
        float fDist = D3DXVec3Length(&m_tInfo.vDir);
        D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

        m_tInfo.vLook = m_tInfo.vDir;
        m_tInfo.vPos += m_tInfo.vDir * 100.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

        if (3.f > fDist)
            BestList.pop_front();
    }
    else
    {
        m_tInfo.vDir = { 0.f,0.f,0.f };
    }
}

void CPlayer::Stop_Astar()
{
    CAstarMgr::Get_Instance()->End_Astar();
}
