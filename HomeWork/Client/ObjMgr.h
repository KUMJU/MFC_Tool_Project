#pragma once

#include "Include.h"


class CObj; 
class CMyTerrain;

class CObjMgr final 
{
	DECLARE_SINGLETON(CObjMgr)

public:
	enum ID { PLAYER, EFFECT, UI, MONSTER, END };

private:
	CObjMgr();
	~CObjMgr();

public:
	void Change_CurTerrain(const wstring& TerrainKey);
	CMyTerrain* Get_CurTerrain() { return m_pCurrentTerrain; }
	CObj* Get_Player() { return m_listObject[PLAYER].front(); }

	void SetObject(const wstring& _sceneKey);
	void DeleteObj();

public:
	void Add_Object(ID eID, CObj* pObject); 

public:
	HRESULT Initialize();
	void Update(); 
	void Late_Update(); 
	void Render(); 
	void Release(); 

private:
	void	ReadMapData(const wstring& DataPath, const wstring& TerrainKey);
	void	ReadObjectData(const wstring& DataPath, const wstring& KeyName);

private:
	list<CObj*>		m_listObject[END];
	CMyTerrain*		m_pCurrentTerrain;

	map<wstring,CMyTerrain*>	m_mapTerrainPool;
	map<wstring,list<OBJINFO>>	m_ObjList;
};

