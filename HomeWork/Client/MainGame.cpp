#include "pch.h"
#include "MainGame.h"
#include "Device.h"
#include "ObjMgr.h"
#include "TextureMgr.h"
#include "SceneMgr.h"
#include "TimeMgr.h"
#include "Player.h"

CMainGame::CMainGame():m_pPlayer(nullptr)
{
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	CTimeMgr::Get_Instance()->Initialize();
	CDevice::Get_Instance()->Init_Device();	
	CTextureMgr::Get_Instance()->ReadImgPath(L"../Data/ImgPath.txt");
	CTextureMgr::Get_Instance()->Initialize();
	CObjMgr::Get_Instance()->Initialize();

	// 플레이어
	m_pPlayer = new CPlayer;
	m_pPlayer->Initialize();
	CObjMgr::Get_Instance()->Add_Object(CObjMgr::PLAYER, m_pPlayer);

	CSceneMgr::Get_Instance()->Change_SceneMgr(CSceneMgr::TOWN);
}

void CMainGame::Update()
{
	CTimeMgr::Get_Instance()->Update();
	CObjMgr::Get_Instance()->Update();
	CSceneMgr::Get_Instance()->Update_SceneMgr();
}

void CMainGame::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();
	CSceneMgr::Get_Instance()->Late_Update_SceneMgr();
}

void CMainGame::Render()
{
	CSceneMgr::Get_Instance()->Render_SceneMgr();
}

void CMainGame::Release()
{
	CObjMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();
	CTextureMgr::Destroy_Instance();
	CDevice::Destroy_Instance();
	CTimeMgr::Destroy_Instance();
}
