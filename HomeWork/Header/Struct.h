#pragma once

typedef struct tagInfo
{
	D3DXVECTOR3		vPos;
	D3DXVECTOR3		vDir;
	D3DXVECTOR3		vLook;
	D3DXVECTOR3		vSize;

	D3DXMATRIX		matWorld;
}INFO;

typedef	struct tagFrame
{
	float		fFrame; // �������� ���� ���� ����
	float		fMax;	// �ִ� �̹����� ������ ��

}FRAME;

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9		pTexture;   // �� ��ü
	D3DXIMAGE_INFO			tImgInfo;	// ����ü

}TEXINFO;


//��������Ʈ(����) ������ �������ִ� ����ü 
struct SPRITEINFO {
	int		iCount;
	float	fSpeed;
	CString strObjKey;
	CString strStateKey;
	int iR;
	int iG;
	int iB;
	int iA;
};

typedef struct tagTile
{
	D3DXVECTOR3	vPos;			// Ÿ���� ���� ��ǥ
	D3DXVECTOR3 vSize;			// Ÿ���� ����, ���� ������

	BYTE		byOption;		// 0, 1��(��ֹ�), 2��(��Ż)
	BYTE		byDrawID;		// �� �� Ÿ�� �̹���

}TILE;

typedef	struct tagUnitData
{
#ifdef _AFX
	CString	strName;
#else
	wstring strName;
#endif
	int		iAttack;
	int		iHp;

	BYTE	byJobIndex;
	BYTE	byItem;

}UNITDATA;

typedef struct tagTexturePath
{
	wstring		wstrObjKey		= L"";
	wstring		wstrStateKey	= L"";
	wstring		wstrPath = L"";
	int			iCount = 0;

}IMGPATH;


static D3DXVECTOR3		Get_Mouse()
{
	POINT	Pt{};

	GetCursorPos(&Pt);
	ScreenToClient(g_hWnd, &Pt);

	return D3DXVECTOR3((float)Pt.x, (float)Pt.y, 0.f);
}

static void	Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY)
{
	pOut->_11 *= fRatioX;
	pOut->_21 *= fRatioX;
	pOut->_31 *= fRatioX;
	pOut->_41 *= fRatioX;

	pOut->_12 *= fRatioY;
	pOut->_22 *= fRatioY;
	pOut->_32 *= fRatioY;
	pOut->_42 *= fRatioY;
}
