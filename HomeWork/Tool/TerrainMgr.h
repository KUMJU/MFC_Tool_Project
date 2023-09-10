#pragma once
#include "Include.h"
#include "MyTerrain.h"

class CToolView;

class CTerrainMgr
{
	DECLARE_SINGLETON(CTerrainMgr)

private:
	CTerrainMgr();
	~CTerrainMgr();

public:
	void		Initialize(CToolView* pMainView);
	void		Release();

	HRESULT		CreateTerrain(const wstring& mapKey, int iBgImgCnt, int iTileCntX, int iTileCntY);

	CMyTerrain* Get_Terrain(const wstring& mapKey);
	const vector<wstring>* const Get_TerrainKeys() { return &m_vecTerrainKey; }

private:
	void		ReadMapData();

private:
	map<wstring, CMyTerrain*>			m_mapTerrain;
	vector<wstring>						m_vecTerrainKey;


};

