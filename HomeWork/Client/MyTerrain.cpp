#include "pch.h"
#include "MyTerrain.h"
#include "TextureMgr.h"
#include "Device.h" 
#include "TimeMgr.h"

CMyTerrain::CMyTerrain()
{
}

CMyTerrain::~CMyTerrain()
{
    Release();
}

HRESULT CMyTerrain::Initialize()
{
    return E_NOTIMPL;
}

int CMyTerrain::Update()
{
    D3DXVECTOR3			vMouse = Get_Mouse();

    if (0.f > vMouse.x)
        m_vScroll.x += 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

    if (WINCX < vMouse.x)
        m_vScroll.x -= 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

    if (0.f > vMouse.y)
        m_vScroll.y += 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

    if (WINCY < vMouse.y)
        m_vScroll.y -= 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

    return 0;
}

void CMyTerrain::Late_Update()
{
}

void CMyTerrain::Render()
{
    {
        D3DXMATRIX  matWorld, matScale, matTrans;

        D3DXMatrixIdentity(&matWorld);
        D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
        D3DXMatrixTranslation(&matTrans,
            m_vScroll.x,
            m_vScroll.y,
            0.f);

        matWorld = matScale * matTrans;

        CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

        const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Map", L"map", m_iBackImgCount);

        float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
        float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
        D3DXVECTOR3 vCenter = { 0.f,0.f,0.f };

        CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
            nullptr,
            &vCenter,
            nullptr,
            D3DCOLOR_ARGB(255, 255, 255, 255));

    }

    D3DXMATRIX  matWorld, matScale, matTrans;
    TCHAR		szBuf[MIN_STR] = L"";
    int			iIndex = 0;

    for (auto& iter : m_vecTile)
    {
        if (iter->vPos.x + m_vScroll.x < TILECX * 0.5f * -1.f ||
            iter->vPos.y + m_vScroll.y < TILECY * 0.5f * -1.f ||
            iter->vPos.x + m_vScroll.x > TILECX * 0.5f + WINCX  ||
            iter->vPos.y + m_vScroll.y > TILECY * 0.5f + WINCY)
            continue;

        D3DXMatrixIdentity(&matWorld);
        D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
        D3DXMatrixTranslation(&matTrans,
            iter->vPos.x + m_vScroll.x,
            iter->vPos.y + m_vScroll.y,
            iter->vPos.z);

        matWorld = matScale * matTrans;

        CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

        const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"TILE", L"Tile", iter->byDrawID);

        float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
        float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

        D3DXVECTOR3 vCenter = { fCenterX,fCenterY, 0.f };
        CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
            nullptr,
            &vCenter,
            nullptr,
            D3DCOLOR_ARGB(255, 255, 255, 255));

        //swprintf_s(szBuf, L"%d", iIndex);

        //CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(), // 스프라이트 객체
        //    szBuf,	// 출력할 문자열
        //    lstrlen(szBuf), // 문자열의 길이
        //    nullptr, //출력학 렉트의 주소
        //    0, // 정렬 옵션
        //    D3DCOLOR_ARGB(255, 0, 255, 255)); // 출력할 폰트 색상
        //++iIndex;

    }

}

void CMyTerrain::Release()
{
    for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
    m_vecTile.clear();
    m_vecTile.shrink_to_fit();
}

HRESULT CMyTerrain::Load_Data(const wstring& wstrMapDataPath)
{
    HANDLE hFile = CreateFile(wstrMapDataPath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (INVALID_HANDLE_VALUE == hFile)
        return E_FAIL;

    DWORD dwByte(0);
    TILE* pTile = nullptr;

    ReadFile(hFile, &m_iBackImgCount, sizeof(int), &dwByte, nullptr);
    ReadFile(hFile, &m_iTileCntX, sizeof(int), &dwByte, nullptr);
    ReadFile(hFile, &m_iTileCntY, sizeof(int), &dwByte, nullptr);

    m_vecTile.reserve(m_iTileCntX * m_iTileCntY);

    while (true)
    {
        pTile = new TILE;

        ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);

        if (0 == dwByte)
        {
            Safe_Delete(pTile);
            break;
        }

        m_vecTile.push_back(pTile);
    }

    CloseHandle(hFile);

    if (FAILED(Ready_Adj()))
    {
        ERR_MSG(L"Ready_Adj Failed");
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CMyTerrain::Ready_Adj()
{
    m_vecAdj.resize(m_vecTile.size());

    for (int i = 0; i < m_iTileCntY; ++i)
    {
        for (int j = 0; j < m_iTileCntX; ++j)
        {
            int iIndex = i * m_iTileCntX + j;

			// 왼쪽 상단
            // i : 홀 -> 짝 TileCntX  감소
            // i : 짝 -> 홀 TileCntX + 1 감소
			
			// 맨 윗줄 아니거나 맨 왼쪽줄 아니거나
            if ((0 != i) && (0 != j))
            {
                int isEven = i % 2;
                int tempIndex = (isEven == 0) ? iIndex - (m_iTileCntX + 1) : iIndex - m_iTileCntX;

                // 장애물이 아니라면 넣어주기
                int iTemp = m_vecTile[tempIndex]->byOption;
                if (iTemp == 2)
                {
                    iTemp;
                }
                if ((int)(m_vecTile[tempIndex]->byOption) != 1)
                    m_vecAdj[iIndex].push_back(m_vecTile[tempIndex]);
            }

   			// 오른쪽 상단
            // i : 홀 -> 짝 TileCntX -1 감소
            // i : 짝 -> 홀 TileCntX 감소
			
            // 맨 윗줄 아니거나 맨 오른쪽줄 아니거나
            if ((0 != i) && (m_iTileCntX - 1 != j))
            {
                int isEven = i % 2;
                int tempIndex = (isEven == 0) ? iIndex - m_iTileCntX : iIndex - (m_iTileCntX - 1);

                if (m_vecTile[tempIndex]->byOption != 1)
                    m_vecAdj[iIndex].push_back(m_vecTile[tempIndex]);
            }

            // 왼쪽 하단
            // i : 홀 -> 짝 TileCntX  증가
            // i : 짝 -> 홀 TileCntX - 1 증가
			
			// 맨 아랫줄 아니거나 맨 왼쪽줄 아니거나
            if ((m_iTileCntY - 1 != i) && (0 != j))
            {
                int isEven = i % 2;
                int tempIndex = (isEven == 0) ? iIndex + (m_iTileCntX - 1) : iIndex + m_iTileCntX;

                if(m_vecTile[tempIndex]->byOption != 1)
                    m_vecAdj[iIndex].push_back(m_vecTile[tempIndex]);
            }

   			// 오른쪽 하단
            // i : 홀 -> 짝 TileCntX + 1 증가
            // i : 짝 -> 홀 TileCntX 증가
			
            // 맨 아랫줄 아니거나 맨 오른줄 아니거나
            if ((m_iTileCntY - 1 != i) && (m_iTileCntX - 1 != j))
            {
                int isEven = i % 2;
                int tempIndex = (isEven == 0) ? iIndex + (m_iTileCntX + 1) : iIndex + m_iTileCntX;

                if (m_vecTile[tempIndex]->byOption != 1)
                    m_vecAdj[iIndex].push_back(m_vecTile[tempIndex]);
            }

        }
    }

    return S_OK;
}

