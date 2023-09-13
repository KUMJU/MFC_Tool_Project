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

        //CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(), // ��������Ʈ ��ü
        //    szBuf,	// ����� ���ڿ�
        //    lstrlen(szBuf), // ���ڿ��� ����
        //    nullptr, //����� ��Ʈ�� �ּ�
        //    0, // ���� �ɼ�
        //    D3DCOLOR_ARGB(255, 0, 255, 255)); // ����� ��Ʈ ����
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

			// ���� ���
            // i : Ȧ -> ¦ TileCntX  ����
            // i : ¦ -> Ȧ TileCntX + 1 ����
			
			// �� ���� �ƴϰų� �� ������ �ƴϰų�
            if ((0 != i) && (0 != j))
            {
                int isEven = i % 2;
                int tempIndex = (isEven == 0) ? iIndex - (m_iTileCntX + 1) : iIndex - m_iTileCntX;

                // ��ֹ��� �ƴ϶�� �־��ֱ�
                int iTemp = m_vecTile[tempIndex]->byOption;
                if (iTemp == 2)
                {
                    iTemp;
                }
                if ((int)(m_vecTile[tempIndex]->byOption) != 1)
                    m_vecAdj[iIndex].push_back(m_vecTile[tempIndex]);
            }

   			// ������ ���
            // i : Ȧ -> ¦ TileCntX -1 ����
            // i : ¦ -> Ȧ TileCntX ����
			
            // �� ���� �ƴϰų� �� �������� �ƴϰų�
            if ((0 != i) && (m_iTileCntX - 1 != j))
            {
                int isEven = i % 2;
                int tempIndex = (isEven == 0) ? iIndex - m_iTileCntX : iIndex - (m_iTileCntX - 1);

                if (m_vecTile[tempIndex]->byOption != 1)
                    m_vecAdj[iIndex].push_back(m_vecTile[tempIndex]);
            }

            // ���� �ϴ�
            // i : Ȧ -> ¦ TileCntX  ����
            // i : ¦ -> Ȧ TileCntX - 1 ����
			
			// �� �Ʒ��� �ƴϰų� �� ������ �ƴϰų�
            if ((m_iTileCntY - 1 != i) && (0 != j))
            {
                int isEven = i % 2;
                int tempIndex = (isEven == 0) ? iIndex + (m_iTileCntX - 1) : iIndex + m_iTileCntX;

                if(m_vecTile[tempIndex]->byOption != 1)
                    m_vecAdj[iIndex].push_back(m_vecTile[tempIndex]);
            }

   			// ������ �ϴ�
            // i : Ȧ -> ¦ TileCntX + 1 ����
            // i : ¦ -> Ȧ TileCntX ����
			
            // �� �Ʒ��� �ƴϰų� �� ������ �ƴϰų�
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

