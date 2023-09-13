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

int CTextureMgr::Get_TextureCnt(const TCHAR* pObjKey, const TCHAR* pStateKey)
{
	wstring ObjKey(pObjKey);

	return m_mapTex[ObjKey]->Get_TextureCnt(pStateKey);
}

void CTextureMgr::Initialize()
{
	//animation 재생에 필요한 spriteinfo 세팅

	wstring filePath = L"..\\Data\\UnitData\\UnitData.dat";

	HANDLE hFile = CreateFile(filePath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwByte(0), dwStrByte(0);
	SPRITEINFO newSpriteInfo{};

	while (true)
	{
		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		TCHAR* pName = new TCHAR[dwStrByte];
		ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);
		newSpriteInfo.strObjKey = pName;

		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		TCHAR* pName2 = new TCHAR[dwStrByte];
		ReadFile(hFile, pName2, dwStrByte, &dwByte, nullptr);
		newSpriteInfo.strStateKey = pName2;

		ReadFile(hFile, &newSpriteInfo.fSpeed, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &newSpriteInfo.iA, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &newSpriteInfo.iR, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &newSpriteInfo.iG, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &newSpriteInfo.iB, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &newSpriteInfo.iCount, sizeof(int), &dwByte, nullptr);

		if (0 == dwByte)
		{
			delete[] pName;
			delete[] pName2;
			break;
		}

		auto iter = m_SpriteInfo.find(newSpriteInfo.strObjKey);

		if (m_SpriteInfo.end() != iter) {
			//이미 키값있음			
			iter->second.push_back(newSpriteInfo);
		}else{
			
			vector<SPRITEINFO> tempVec;
			tempVec.push_back(newSpriteInfo);

			m_SpriteInfo.insert({ newSpriteInfo.strObjKey, tempVec });
		}
	}

	CloseHandle(hFile);

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
			ERR_MSG(pFilePath);
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

HRESULT CTextureMgr::ReadImgPath(const wstring& wstrPath)
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
				ERR_MSG(L"MultiImgTxt Insert Failed");
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

SPRITEINFO CTextureMgr::GetSpriteInfo(wstring _objKey, wstring _stateKey)
{
	auto iter = m_SpriteInfo.find(_objKey);

	if (m_SpriteInfo.end() != iter) {

		for (auto& it : iter->second) {
			if (_stateKey == it.strStateKey) {
				return it;
			}
		}
	}

	SPRITEINFO tempSprite{};

	return tempSprite;
}
