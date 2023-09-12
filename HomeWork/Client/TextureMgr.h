#pragma once
#include "SingleTexture.h"
#include "MultiTexture.h"

class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

private:
	CTextureMgr();
	~CTextureMgr();

public:
	const TEXINFO* Get_Texture(const TCHAR* pObjKey, const TCHAR* pStateKey = L"", const int& iCnt = 0);
	int	Get_TextureCnt(const TCHAR* pObjKey, const TCHAR* pStateKey);

public:
	HRESULT		Insert_Texture(TEXTYPE eType, const TCHAR* pFilePath, const TCHAR* pObjKey, const TCHAR* pStateKey = L"", const int& iCnt = 0);
	HRESULT		ReadImgPath(const wstring& wstrPath);
	void		Release();

private:
	map<wstring, CTexture*>			m_mapTex;

};

