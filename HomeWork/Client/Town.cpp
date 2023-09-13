#include "pch.h"
#include "Town.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "Device.h"
#include "AstarMgr.h"
#include "MyTerrain.h"
#include "SceneMgr.h"
#include "Player.h"

CTown::CTown()
    :m_pPlayer(nullptr)
{
}

CTown::~CTown()
{
    Release_Scene();
}

HRESULT CTown::Ready_Scene()
{
    // ÁöÇü
    CObjMgr::Get_Instance()->Change_CurTerrain(L"Town1");
    m_pPlayer = CObjMgr::Get_Instance()->Get_Player();
    CObjMgr::Get_Instance()->SetObject(L"Town1");


    D3DXVECTOR3 InitPos = { 1000.f,700.f,0.f };
    m_pPlayer->Set_Pos(InitPos);

    return S_OK;
}

void CTown::Update_Scene()
{
    CObjMgr::Get_Instance()->Update();
}

void CTown::Late_Update_Scene()
{
    __super::UpdateScroll();

    CObjMgr::Get_Instance()->Late_Update();

    D3DXVECTOR3 playerPos = m_pPlayer->Get_Info().vPos;
    CMyTerrain* pTerrain = CObjMgr::Get_Instance()->Get_CurTerrain();
    int iIndex = CAstarMgr::Get_Instance()->Get_TileIdx(playerPos);
    const TILE* curTile = pTerrain->Get_Tile(iIndex);
    if (curTile->byOption == 2)
    {
        dynamic_cast<CPlayer*>(m_pPlayer)->Stop_Astar();
        CSceneMgr::Get_Instance()->Change_SceneMgr(CSceneMgr::STAGE);
    }
}

void CTown::Render_Scene()
{
    CDevice::Get_Instance()->Render_Begin();
       
    CObjMgr::Get_Instance()->Render();

    CDevice::Get_Instance()->Render_End();
}

void CTown::Release_Scene()
{
}
