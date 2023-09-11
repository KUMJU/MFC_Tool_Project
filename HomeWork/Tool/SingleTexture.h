#pragma once
#include "Texture.h"
class CSingleTexture :
    public CTexture
{
public:
    CSingleTexture();
    virtual ~CSingleTexture();

public:
    const TEXINFO* Get_Texture(const TCHAR* pStateKey, const int& iCnt) override;
    HRESULT Insert_Texture(const TCHAR* pFilePath, const TCHAR* pStateKey, const int& iCnt) override;
    void Release() override;
    int Get_TexCount(const TCHAR* pStateKey) override;

private:
    TEXINFO*    m_pTexInfo;
};

