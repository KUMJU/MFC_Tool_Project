#include "pch.h"
#include "Obj.h"
//#include "TimeMgr.h"

D3DXVECTOR3 CObj::m_vScroll{};

CObj::CObj()
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	D3DXMatrixIdentity(&m_tInfo.matWorld);

	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}


CObj::~CObj()
{
}


void CObj::Move_Frame(void)
{
	if (m_dwTime + 300 < GetTickCount())
	{
		m_tFrame.fFrame++;
		if (m_tFrame.fFrame > m_tFrame.fMax)
		{
			m_tFrame.fFrame = 0;
		}

		m_dwTime = GetTickCount();
	}
}
