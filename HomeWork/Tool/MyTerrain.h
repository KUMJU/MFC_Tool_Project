#pragma once
#include "Include.h"

class CToolView;

class CMyTerrain
{
public:
	CMyTerrain();
	~CMyTerrain();

public:
	void	Set_MainVeiw(CToolView* pMainView) { m_pMainView = pMainView; }
	
	void	Tile_Change(const D3DXVECTOR3& vPos, const int& iDrawID);
	int		Get_TileIndex(const D3DXVECTOR3& vPos);
	bool	Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	bool	Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);

public:
	HRESULT		Initialize();
	void		Update();
	void		Render();
	void		Mini_Render();
	void		Release();

private:
	vector<TILE*>	m_vecTile;
	CToolView*		m_pMainView;


};

