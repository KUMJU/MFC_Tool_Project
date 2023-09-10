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
	
	void	Tile_Change(const D3DXVECTOR3& vPos, int iDrawID, int iDrawOption);
	int		Get_TileIndex(const D3DXVECTOR3& vPos);
	bool	Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	bool	Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);

	void	SetBackGroundImgCount(int bgImgCount) { m_iBackImgCount = bgImgCount; }

public:
	HRESULT		Initialize(int iBgCount, int iTileCntX, int iTileCntY);
	void		Update();
	void		Render();
	void		Mini_Render();
	void		Release();

public:
	HRESULT		Save_Data(const wstring& DataSavePath);
	HRESULT		Load_Data(const wstring& wstrMapDataPath);

private:
	vector<TILE*>	m_vecTile;
	CToolView*		m_pMainView;

	int	m_iBackImgCount = 0;
	int m_iTileCntX = 0;
	int m_iTileCntY = 0;
};

