#include "pch.h"
#include "TextureMgr.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
{
}

CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEXINFO* CTextureMgr::Get_Texture(const TCHAR* pObjKey, const TCHAR* pStateKey, const int& iCnt)
{
	auto iter = m_mapTex.find(pObjKey);

	if(iter == m_mapTex.end())
		return nullptr;

	return iter->second->Get_Texture(pStateKey, iCnt);
}

int CTextureMgr::Get_MulTexCount(const TCHAR* pObjKey, const TCHAR* pStateKey)
{
	auto iter = m_mapTex.find(pObjKey);

	if (iter == m_mapTex.end())
		return -1;

	return iter->second->Get_TexCount(pStateKey);
}

HRESULT CTextureMgr::Insert_Texture(TEXTYPE eType, const TCHAR* pFilePath, const TCHAR* pObjKey, const TCHAR* pStateKey, const int& iCnt)
{
	auto iter = m_mapTex.find(pObjKey);

	if (iter == m_mapTex.end())
	{
		CTexture* pTexture = nullptr;

		switch (eType)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;
		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if (FAILED(pTexture->Insert_Texture(pFilePath, pStateKey, iCnt)))
		{
			Safe_Delete(pTexture);
			AfxMessageBox(pFilePath);
			return E_FAIL;
		}

		m_mapTex.insert({ pObjKey,pTexture });
	}
	else {
		if (TEX_MULTI == eType) {
			iter->second->Insert_Texture(pFilePath, pStateKey, iCnt);
		}
	}

	return S_OK;
}

HRESULT CTextureMgr::ReadImgPath(const wstring& wstrPath, map<CString, vector<SPRITEINFO*>>* _infoList)
{
	wifstream fin;
	fin.open(wstrPath, ios::in);

	if (!fin.fail())
	{
		TCHAR	szObjKey[MAX_STR] = L"";
		TCHAR   szStateKey[MAX_STR] = L"";
		TCHAR	szCount[MAX_STR] = L"";
		TCHAR	szPath[MAX_PATH] = L"";

		while (true)
		{
			fin.getline(szObjKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MAX_STR, '|');
			fin.getline(szPath, MAX_PATH);

			if (fin.eof())
				break;

			int iCount = _ttoi(szCount);

			if (FAILED(Insert_Texture(TEX_MULTI, szPath, szObjKey, szStateKey, iCount)))
			{
				AfxMessageBox(L"MultiImgTxt Insert Failed");
				return E_FAIL;
			}
		}

		fin.close();
	}

	return S_OK;
}

void CTextureMgr::Release()
{
	for_each(m_mapTex.begin(), m_mapTex.end(), [](auto& MyPair) {

		Safe_Delete(MyPair.second);
		});

	m_mapTex.clear();
}
