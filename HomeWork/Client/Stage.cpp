#include "pch.h"
#include "Stage.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "Player.h"
#include "MyTerrain.h"
#include "Device.h"

CStage::CStage()
	:pPlayer(nullptr), m_pTerrain(nullptr)
{
}

CStage::~CStage()
{
	Release_Scene();
}

HRESULT CStage::Ready_Scene()
{
	// ����
	m_pTerrain = new CMyTerrain;
	if (nullptr == m_pTerrain) return E_FAIL;

	m_pTerrain->Initialize();

	// �÷��̾�
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
	m_pTerrain->Update();
}

void CStage::Late_Update_Scene()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CStage::Render_Scene()
{
	CDevice::Get_Instance()->Render_Begin();

	m_pTerrain->Render();
	CObjMgr::Get_Instance()->Render();

	CDevice::Get_Instance()->Render_End();
}

void CStage::Release_Scene()
{
	m_pTerrain->Release();
}
