#pragma once
#include "Include.h"
#include "Obj.h"

class CMyTerrain:
	public CObj
{
public:
	CMyTerrain();
	~CMyTerrain();

public:
	int		Get_TileIndex(const D3DXVECTOR3& vPos);
	bool	Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	bool	Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);

	int		Get_TileCntX() { return m_iTileCntX; }
	int		Get_TileCntY() { return m_iTileCntY; }

public:
	HRESULT			Initialize(void) override;
	virtual int		Update() override;
	virtual void	Late_Update(void) override;
	virtual void	Render() override;
	virtual void	Release();

public:
	HRESULT		Load_Data(const wstring& wstrMapDataPath);

private:
	vector<TILE*>	m_vecTile;

	int	m_iBackImgCount = 0;
	int m_iTileCntX = 0;
	int m_iTileCntY = 0;

};

