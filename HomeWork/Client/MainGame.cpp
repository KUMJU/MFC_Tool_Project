#include "pch.h"
#include "MainGame.h"
#include "Device.h"
#include "ObjMgr.h"
#include "TextureMgr.h"
#include "SceneMgr.h"

CMainGame::CMainGame()
{
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	CDevice::Get_Instance()->Init_Device();	
	CSceneMgr::Get_Instance()->Change_SceneMgr(CSceneMgr::STAGE);

}

void CMainGame::Update()
{
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
	CObjMgr::Get_Instance()->Render();
	CSceneMgr::Get_Instance()->Render_SceneMgr();
}

void CMainGame::Release()
{
	CObjMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();
	CTextureMgr::Destroy_Instance();
	CDevice::Destroy_Instance();
}
