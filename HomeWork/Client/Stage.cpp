#include "pch.h"
#include "Stage.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "Player.h"
#include "MyTerrain.h"
#include "Device.h"

CStage::CStage()
	:pPlayer(nullptr)
{
}

CStage::~CStage()
{
	Release_Scene();
}

HRESULT CStage::Ready_Scene()
{
	// 지형
	CObjMgr::Get_Instance()->Change_CurTerrain(L"Deongeon");

	// 플레이어
	pPlayer = new CPlayer;
	if (nullptr == pPlayer)
		return E_FAIL;

	pPlayer->Initialize();

	CObjMgr::Get_Instance()->Add_Object(CObjMgr::PLAYER, pPlayer);
	
	return S_OK;
}

void CStage::Update_Scene()
{
	CObjMgr::Get_Instance()->Update();
	pPlayer->Update();
}

void CStage::Late_Update_Scene()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CStage::Render_Scene()
{
	CDevice::Get_Instance()->Render_Begin();

	CObjMgr::Get_Instance()->Render();

	CDevice::Get_Instance()->Render_End();
}

void CStage::Release_Scene()
{
}
