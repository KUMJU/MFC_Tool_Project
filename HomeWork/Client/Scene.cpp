#include "pch.h"
#include "Scene.h"
#include "MyTerrain.h"
#include "TextureMgr.h"
#include "ObjMgr.h"

CScene::CScene()
{
}


CScene::~CScene()
{
}

void CScene::UpdateScroll()
{
	CMyTerrain* pTerrain = CObjMgr::Get_Instance()->Get_CurTerrain();
	CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player();

	if (!pTerrain || !pPlayer)
		return;

	D3DXVECTOR3 playerPos = pPlayer->Get_Info().vPos;
	D3DXVECTOR3 BgSize = pTerrain->Get_BackGroundSize();

	D3DXVECTOR3 vScroll = { playerPos.x - WINCX * 0.5f, playerPos.y - WINCY * 0.5f, 0.f };

	if (vScroll.x < 0.f)
		vScroll.x = 0.f;
	else if (vScroll.x > BgSize.x - WINCX)
		vScroll.x = (BgSize.x - WINCX);

	if (vScroll.y < 0.f)
		vScroll.y = 0.f;
	else if (vScroll.y > BgSize.y - WINCY)
		vScroll.y = (BgSize.y - WINCY);

	CObj::Set_Scroll(-vScroll);
}
