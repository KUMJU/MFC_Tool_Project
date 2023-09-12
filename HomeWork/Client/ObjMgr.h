#pragma once

#include "Include.h"


class CObj; 
class CMyTerrain;

class CObjMgr final 
{
	DECLARE_SINGLETON(CObjMgr)

public:
	enum ID { MONSTER, PLAYER, EFFECT, UI, END };

private:
	CObjMgr();
	~CObjMgr();

public:
	void Change_CurTerrain(const wstring& TerrainKey);
	CMyTerrain* Get_CurTerrain() { return m_pCurrentTerrain; }

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

private:
	list<CObj*>		m_listObject[END];
	CMyTerrain*		m_pCurrentTerrain;

	map<wstring,CMyTerrain*>	m_mapTerrainPool;
};

