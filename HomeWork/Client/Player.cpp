#include "pch.h"
#include "Player.h"
#include "Device.h"

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
	m_MulTex.Insert_Texture(L"../Texture/Player/STAND_DOWN/STAND_DOWN%d.png", L"STAND_DOWN", 9);
	m_MulTex.Insert_Texture(L"../Texture/Player/STAND_LDOWN/STAND_LDOWN%d.png", L"STAND_LDOWN", 9);
	m_MulTex.Insert_Texture(L"../Texture/Player/STAND_LEFT/STAND_LEFT%d.png", L"STAND_LEFT", 9);
	m_MulTex.Insert_Texture(L"../Texture/Player/STAND_LUP/STAND_LUP%d.png", L"STAND_LUP", 9);
	m_MulTex.Insert_Texture(L"../Texture/Player/STAND_UP/STAND_UP%d.png", L"STAND_UP", 9);

	m_MulTex.Insert_Texture(L"../Texture/Player/RUN_DOWN/RUN_DOWN%d.png", L"RUN_DOWN", 7);
	m_MulTex.Insert_Texture(L"../Texture/Player/RUN_LDOWN/RUN_LDOWN%d.png", L"RUN_LDOWN", 7);
	m_MulTex.Insert_Texture(L"../Texture/Player/RUN_LEFT/RUN_LEFT%d.png", L"RUN_LEFT", 7);
	m_MulTex.Insert_Texture(L"../Texture/Player/RUN_LUP/RUN_LUP%d.png", L"RUN_LUP", 7);
	m_MulTex.Insert_Texture(L"../Texture/Player/RUN_UP/RUN_UP%d.png", L"RUN_UP", 7);

    m_dwTime = GetTickCount();
    m_wstrStateKey = L"STAND_DOWN";
    m_tFrame.fMax = m_MulTex.Get_TexMap()[m_wstrStateKey].size()-1;
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

    float mag = D3DXVec3Length(&m_tInfo.vDir);

    // ÁÂ¿ì ¹ÝÀü
    if (mag < 1.f)
    {
        if (m_tInfo.vLook.x > 0)
            D3DXMatrixScaling(&matScale, -1.f, 1.f, 1.f);
        else
            D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
    }
    else
    {
        if (m_tInfo.vDir.x > 0)
            D3DXMatrixScaling(&matScale, -1.f, 1.f, 1.f);
        else
            D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
    }

    D3DXMatrixTranslation(&matTrans,
        m_tInfo.vPos.x,
        m_tInfo.vPos.y,
        m_tInfo.vPos.z);

    matWorld = matScale * matTrans;

    RECT    rc{};
    GetClientRect(g_hWnd, &rc);
    float fX = WINCX / float(rc.right - rc.left);
    float fY = WINCY / float(rc.bottom - rc.top);
    Set_Ratio(&matWorld, fX, fY);

    CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

    const TEXINFO* pTexInfo = m_MulTex.Get_Texture(m_wstrStateKey.c_str(),m_tFrame.fFrame);

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
    m_MulTex.Release();
}

void CPlayer::Key_Input()
{
    if (GetAsyncKeyState(VK_RBUTTON))
    {
        m_vDstPos = Get_Mouse();
    }
}

void CPlayer::Update_Move()
{
    m_tInfo.vDir = m_vDstPos - m_tInfo.vPos;
    float mag = D3DXVec3Length(&m_tInfo.vDir);
                
    if (mag < 1.f)
    {
        m_tInfo.vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
        m_eCurState = PLAYER_STATE::IDLE;
        return;
    }

    D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
    m_tInfo.vLook = m_tInfo.vDir;

    m_tInfo.vPos += m_tInfo.vDir * 2.f; 
}

void CPlayer::Update_State()
{
    float mag = D3DXVec3Length(&m_tInfo.vDir);

    if (mag >= 0.1f)
    {
        float angleRad = m_tInfo.vDir.y > 0 ? 2 * D3DX_PI - acosf(m_tInfo.vDir.x / mag) : acosf(m_tInfo.vDir.x / mag);
        float angleDeg = D3DXToDegree(angleRad);
        int iState = (angleDeg + 22.5f) / 45;

        switch (iState)
        {
        case 0:
        case 4:
            if (m_ePrevState != PLAYER_STATE::LEFT_RUN)
            {
                m_ePrevState = PLAYER_STATE::LEFT_RUN;
                m_wstrStateKey = L"RUN_LEFT";
                m_tFrame.fFrame = 0;
                m_tFrame.fMax = m_MulTex.Get_TexMap()[m_wstrStateKey].size() - 1;
            }
            break;
        case 1:
        case 3:
            if (m_ePrevState != PLAYER_STATE::LU_RUN)
            {
                m_ePrevState = PLAYER_STATE::LU_RUN;
                m_wstrStateKey = L"RUN_LUP";
                m_tFrame.fFrame = 0;
                m_tFrame.fMax = m_MulTex.Get_TexMap()[m_wstrStateKey].size() - 1;
            }
            break;
        case 2:
            if (m_ePrevState != PLAYER_STATE::UP_RUN)
            {
                m_ePrevState = PLAYER_STATE::UP_RUN;
                m_wstrStateKey = L"RUN_UP";
                m_tFrame.fFrame = 0;
                m_tFrame.fMax = m_MulTex.Get_TexMap()[m_wstrStateKey].size() - 1;
            }
            break;
        case 5:
        case 7:
            if (m_ePrevState != PLAYER_STATE::LD_RUN)
            {
                m_ePrevState = PLAYER_STATE::LD_RUN;
                m_wstrStateKey = L"RUN_LDOWN";
                m_tFrame.fFrame = 0;
                m_tFrame.fMax = m_MulTex.Get_TexMap()[m_wstrStateKey].size() - 1;
            }
            break;
        case 6:
            if (m_ePrevState != PLAYER_STATE::DOWN_RUN)
            {
                m_ePrevState = PLAYER_STATE::DOWN_RUN;
                m_wstrStateKey = L"RUN_DOWN";
                m_tFrame.fFrame = 0;
                m_tFrame.fMax = m_MulTex.Get_TexMap()[m_wstrStateKey].size() - 1;
            }
            break;
        }
    }
    else
    {
        float angleRad = m_tInfo.vLook.y > 0 ? 2 * D3DX_PI - acosf(m_tInfo.vLook.x) : acosf(m_tInfo.vLook.x);
        float angleDeg = D3DXToDegree(angleRad);
        int iState = (angleDeg + 22.5f) / 45;

        switch (iState)
        {
        case 0:
        case 4:
            if (m_wstrStateKey != L"STAND_LEFT")
            {
                m_wstrStateKey = L"STAND_LEFT";
                m_tFrame.fFrame = 0;
                m_tFrame.fMax = m_MulTex.Get_TexMap()[m_wstrStateKey].size() - 1;
            }
            break;
        case 1:
        case 3:
            if (m_wstrStateKey != L"STAND_LUP")
            {
                m_wstrStateKey = L"STAND_LUP";
                m_tFrame.fFrame = 0;
                m_tFrame.fMax = m_MulTex.Get_TexMap()[m_wstrStateKey].size() - 1;
            }
            break;
        case 2:
            if (m_wstrStateKey != L"STAND_UP")
            {
                m_wstrStateKey = L"STAND_UP";
                m_tFrame.fFrame = 0;
                m_tFrame.fMax = m_MulTex.Get_TexMap()[m_wstrStateKey].size() - 1;
            }
            break;
        case 5:
        case 7:
            if (m_wstrStateKey != L"STAND_LDOWN")
            {
                m_wstrStateKey = L"STAND_LDOWN";
                m_tFrame.fFrame = 0;
                m_tFrame.fMax = m_MulTex.Get_TexMap()[m_wstrStateKey].size() - 1;
            }
            break;
        case 6:
            if (m_wstrStateKey != L"STAND_DOWN")
            {
                m_wstrStateKey = L"STAND_DOWN";
                m_tFrame.fFrame = 0;
                m_tFrame.fMax = m_MulTex.Get_TexMap()[m_wstrStateKey].size() - 1;
            }
            break;
        }
    }


}
