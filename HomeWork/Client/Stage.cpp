#include "pch.h"
#include "Stage.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "MyTerrain.h"
#include "Device.h"
#include "Obj.h"

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
	// ÁöÇü
	CObjMgr::Get_Instance()->Change_CurTerrain(L"TempDeongeon");
	pPlayer = CObjMgr::Get_Instance()->Get_Player();

	D3DXVECTOR3 InitPos = {1050.f,700.f,0.f};
	pPlayer->Set_Pos(InitPos);

	return S_OK;
}

void CStage::Update_Scene()
{
	CObjMgr::Get_Instance()->Update();
}

void CStage::Late_Update_Scene()
{
	__super::UpdateScroll();

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
