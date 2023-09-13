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

	void Initialize();

public:
	HRESULT		Insert_Texture(TEXTYPE eType, const TCHAR* pFilePath, const TCHAR* pObjKey, const TCHAR* pStateKey = L"", const int& iCnt = 0);
	HRESULT		ReadImgPath(const wstring& wstrPath);
	void		Release();
	SPRITEINFO		GetSpriteInfo(wstring _objKey, wstring _stateKey);

private:
	map<wstring, CTexture*>					m_mapTex;
	map<wstring, vector<SPRITEINFO>>		m_SpriteInfo;

};

