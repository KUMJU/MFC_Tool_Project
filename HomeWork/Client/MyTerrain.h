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
	int		Get_TileCntX() { return m_iTileCntX; }
	int		Get_TileCntY() { return m_iTileCntY; }

	vector<TILE*>&			Get_VecTile() { return m_vecTile; }
	vector<list<TILE*>>&	Get_VecAdj() { return m_vecAdj; }

public:
	HRESULT			Initialize(void) override;
	virtual int		Update() override;
	virtual void	Late_Update(void) override;
	virtual void	Render() override;
	virtual void	Release();

public:
	HRESULT		Load_Data(const wstring& wstrMapDataPath);
	HRESULT		Ready_Adj();

private:
	int	m_iBackImgCount = 0;
	int m_iTileCntX = 0;
	int m_iTileCntY = 0;

	vector<TILE*>		m_vecTile;
	vector<list<TILE*>>	m_vecAdj;
};

