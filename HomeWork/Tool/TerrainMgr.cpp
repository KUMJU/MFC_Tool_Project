#include "pch.h"
#include "TerrainMgr.h"
#include "ToolView.h"
#include "MainFrm.h"

IMPLEMENT_SINGLETON(CTerrainMgr)

CTerrainMgr::CTerrainMgr()
{
}

CTerrainMgr::~CTerrainMgr()
{
	Release();
}

void CTerrainMgr::Initialize(CToolView* pMainView)
{
	ReadMapData();

	for (auto& iter : m_mapTerrain)
	{
		m_vecTerrainKey.push_back(iter.first);
		iter.second->Set_MainVeiw(pMainView);
	}
}

void CTerrainMgr::Release()
{
	for_each(m_mapTerrain.begin(), m_mapTerrain.end(), [](auto& MyPair) {
		Safe_Delete(MyPair.second);
		});

	m_mapTerrain.clear();
}

HRESULT CTerrainMgr::CreateTerrain(const wstring& mapKey, int iBgImgCnt, int iTileCntX, int iTileCntY)
{
	auto iter = m_mapTerrain.find(mapKey);

	if (iter == m_mapTerrain.end())
	{
		CMyTerrain* pTerrain = new CMyTerrain;
		pTerrain->Initialize(iBgImgCnt, iTileCntX, iTileCntY);

		CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		CToolView* pMainView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
		pTerrain->Set_MainVeiw(pMainView);

		m_mapTerrain.insert({ mapKey,pTerrain });
		m_vecTerrainKey.push_back(mapKey);

		return S_OK;
	}

	return E_FAIL;
}

CMyTerrain* CTerrainMgr::Get_Terrain(const wstring& mapKey)
{
	auto& iter = m_mapTerrain.find(mapKey);

	if (iter == m_mapTerrain.end())
		return nullptr;

	return iter->second;
}

void CTerrainMgr::ReadMapData()
{
	CFileFind Find;

	BOOL bContinue = Find.FindFile(L"../Data/MapData/*.*");

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;
		else if (Find.IsSystem())
			continue;
		else
		{
			wstring filePath = Find.GetFilePath().GetString();
			CMyTerrain* pTerrain = new CMyTerrain;
			if (FAILED(pTerrain->Load_Data(filePath)))
			{
				AfxMessageBox(L"데이터 불러오기 실패!!");
				return;
			}

			wstring mapKey = Find.GetFileTitle().GetString();
			m_mapTerrain.insert({ mapKey, pTerrain });
		}
	}
}
