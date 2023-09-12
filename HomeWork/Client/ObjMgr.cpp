#include "pch.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "Device.h"
#include "MyTerrain.h"

IMPLEMENT_SINGLETON(CObjMgr)

CObjMgr::CObjMgr()
	:m_pCurrentTerrain(nullptr)
{
}


CObjMgr::~CObjMgr()
{
	Release(); 
}

void CObjMgr::Change_CurTerrain(const wstring& TerrainKey)
{
	auto iter = m_mapTerrainPool.find(TerrainKey);

	if (iter == m_mapTerrainPool.end())
		return;

	m_pCurrentTerrain = iter->second;
}

void CObjMgr::Add_Object(ID eID, CObj * pObject)
{
	if (nullptr == pObject || END <= eID)
		return; 

	m_listObject[eID].emplace_back(pObject); 
}

HRESULT CObjMgr::Initialize()
{
	ReadMapData(L"../Data/MapData/Town1.dat", L"Town1");
	//ReadMapData(L"../Data/MapData/Deongeon.dat", L"Deongeon");
	//ReadMapData(L"../Data/MapData/Island.dat", L"Island");
	//ReadMapData(L"../Data/MapData/Shop.dat", L"Shop");
	//ReadMapData(L"../Data/MapData/Town.dat", L"Town");

	return S_OK;
}

void CObjMgr::Update()
{
	m_pCurrentTerrain->Update();

	for (int i = 0 ; i < END; ++i)
	{
		for (auto&& iter = m_listObject[i].begin() ; iter != m_listObject[i].end(); )
		{
			int iEvent = (*iter)->Update();

			if (OBJ_DEAD == iEvent)
			{
				if(i != EFFECT)
					Safe_Delete(*iter);

				iter = m_listObject[i].erase(iter);
			}
			else
				++iter;
		}
	}
}

void CObjMgr::Late_Update()
{
	m_pCurrentTerrain->Late_Update();

	for (int i = 0 ; i < END ; ++i)
	{
		for (auto& pObject : m_listObject[i])
			pObject->Late_Update();
	}
}

void CObjMgr::Render()
{
	m_pCurrentTerrain->Render();

	for (int i = 0; i < END; ++i)
	{
		for (auto& pObject : m_listObject[i])
		{
			pObject->Render();
		}
	}
}

void CObjMgr::Release()
{
	// 오브젝트 해제
	for (int i = 0; i < END; ++i)
	{
		for (auto& pObject : m_listObject[i])
			Safe_Delete(pObject); 

		m_listObject[i].clear();
	}

	// 터레인 풀 해제
	for_each(m_mapTerrainPool.begin(), m_mapTerrainPool.end(), [](auto& MyPair) {
		Safe_Delete(MyPair.second);
		});
	m_mapTerrainPool.clear();

}

void CObjMgr::ReadMapData(const wstring& DataPath, const wstring& TerrainKey)
{
	CMyTerrain* pTerrain = new CMyTerrain;
	if (FAILED(pTerrain->Load_Data(DataPath)))
	{
		wstring msg = TerrainKey + L" 불러오기 실패!!";
		ERR_MSG(msg.c_str());
		return;
	}

	m_mapTerrainPool.insert({ TerrainKey, pTerrain });
}
