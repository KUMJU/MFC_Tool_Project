#include "pch.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "Device.h"
#include "MyTerrain.h"

#include"Monster.h"
#include"Environment.h"

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

void CObjMgr::SetObject(const wstring& _sceneKey)
{
	auto iter = m_ObjList.find(_sceneKey);

	if (m_ObjList.end() == iter)
		return;

	CObj* newObj;

	if (m_ObjList.end() != iter) {
		for (auto& it : iter->second) {
			if (it.strObjKey == L"Object") {
				newObj = new CEnvironment;
				newObj->SetKeyName(it.strObjKey, it.strStateKey);
				newObj->Initialize();
				dynamic_cast<CEnvironment*>(newObj)->SetImgCount(it.iCount);
				newObj->Set_Pos(it.vPos);
				Add_Object(CObjMgr::UI, newObj);

			}
			else {
				newObj = new CMonster;
				newObj->SetKeyName(it.strObjKey, it.strStateKey);
				newObj->Set_Pos(it.vPos);
				newObj->Initialize();
				Add_Object(CObjMgr::MONSTER, newObj);
			}
		}
	}


}

void CObjMgr::DeleteObj()
{
	for (auto&& iter = m_listObject[MONSTER].begin(); iter != m_listObject[MONSTER].end(); ++iter )
	{
		Safe_Delete<CObj*>(*iter);
		
	}

	m_listObject[MONSTER].clear();

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
	ReadMapData(L"../Data/MapData/TempDeongeon.dat", L"TempDeongeon");
	//ReadMapData(L"../Data/MapData/Deongeon.dat", L"Deongeon");
	//ReadMapData(L"../Data/MapData/Island.dat", L"Island");
	//ReadMapData(L"../Data/MapData/Shop.dat", L"Shop");
	//ReadMapData(L"../Data/MapData/Town.dat", L"Town");

	ReadObjectData(L"../Data/ObjectData/Town1.dat", L"Town1");
	ReadObjectData(L"../Data/ObjectData/TempDeongeon.dat", L"TempDeongeon");

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

void CObjMgr::ReadObjectData(const wstring& DataPath , const wstring& KeyName)
{
	HANDLE hFile = CreateFile(DataPath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwByte(0), dwStrByte(0);
	
	OBJINFO objInfo;
	list<OBJINFO> tempList;
	while (true)
	{
		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		TCHAR* pName = new TCHAR[dwStrByte];
		ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);
		objInfo.strObjKey = pName;

		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		TCHAR* pName2 = new TCHAR[dwStrByte];
		ReadFile(hFile, pName2, dwStrByte, &dwByte, nullptr);
		objInfo.strStateKey = pName2;

		ReadFile(hFile, &objInfo.IsReverse, sizeof(bool), &dwByte, nullptr);
		ReadFile(hFile, &objInfo.iCount, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &objInfo.vPos, sizeof(D3DXVECTOR3), &dwByte, nullptr);

		if (0 == dwByte)
		{
			delete[] pName;
			delete[] pName2;
			break;
		}

		tempList.push_back(objInfo);
	}

	if(!tempList.empty())
		m_ObjList.insert({ KeyName , tempList });


	CloseHandle(hFile);


}



