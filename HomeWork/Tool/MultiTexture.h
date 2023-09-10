#pragma once
#include "Texture.h"
class CMultiTexture : public CTexture
{
public:
	CMultiTexture();
	virtual ~CMultiTexture();

public:
	virtual const TEXINFO* Get_Texture(const TCHAR* pStateKey = L"", const int& iCnt = 0) override;
	virtual HRESULT Insert_Texture(const TCHAR* pFilePath, const TCHAR* pStateKey = L"", const int& iCnt = 0) override;
	virtual void Release() override;
	virtual int Get_TexCount(const TCHAR* pStateKey) override;

private:
	map<wstring, vector<TEXINFO*>>		m_mapMultiTex;
};

