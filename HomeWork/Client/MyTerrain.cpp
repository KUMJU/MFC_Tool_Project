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

int CMyTerrain::Get_TileIndex(const D3DXVECTOR3& vPos)
{
    for (size_t index = 0; index < m_vecTile.size(); ++index)
    {
        if (Picking_Dot(vPos, index))
        {
            return index;
        }
    }

    return -1;
}

bool CMyTerrain::Picking(const D3DXVECTOR3& vPos, const int& iIndex)
{
    // y = ax + b
    // 12 -> 3 -> 6 - > 9

    float fGradient[4]{
        (TILECY * 0.5f) / (TILECX * 0.5f) * -1.f,
        (TILECY * 0.5f) / (TILECX * 0.5f),
        (TILECY * 0.5f) / (TILECX * 0.5f) * -1.f,
        (TILECY * 0.5f) / (TILECX * 0.5f)
    };

    // y = ax + b
    // b = y - ax

    D3DXVECTOR3 vPoint[4]{
        {m_vecTile[iIndex]->vPos.x,m_vecTile[iIndex]->vPos.y + (TILECY * 0.5f), 0.f},
        {m_vecTile[iIndex]->vPos.x + (TILECX * 0.5f),m_vecTile[iIndex]->vPos.y, 0.f},
        {m_vecTile[iIndex]->vPos.x,m_vecTile[iIndex]->vPos.y - (TILECY * 0.5f), 0.f},
        {m_vecTile[iIndex]->vPos.x - (TILECX * 0.5f),m_vecTile[iIndex]->vPos.y, 0.f}
    };

    float fB[4]{
        vPoint[0].y - fGradient[0] * vPoint[0].x,
        vPoint[1].y - fGradient[1] * vPoint[1].x,
        vPoint[2].y - fGradient[2] * vPoint[2].x,
        vPoint[3].y - fGradient[3] * vPoint[3].x
    };

    // 0 == ax + b - y
    // 0 > ax + b - y(위)
    // 0 < ax + b - y(아래) 

    bool bCheck[4]{ false };

    if (0 < fGradient[0] * vPos.x + fB[0] - vPos.y)
        bCheck[0] = true;

    if (0 >= fGradient[1] * vPos.x + fB[1] - vPos.y)
        bCheck[1] = true;

    if (0 >= fGradient[2] * vPos.x + fB[2] - vPos.y)
        bCheck[2] = true;

    if (0 < fGradient[3] * vPos.x + fB[3] - vPos.y)
        bCheck[3] = true;

    return bCheck[0] && bCheck[1] && bCheck[2] && bCheck[3];
}

bool CMyTerrain::Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex)
{
    D3DXVECTOR3		vPoint[4]{
        { m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
        { m_vecTile[iIndex]->vPos.x + (TILECX / 2.f), m_vecTile[iIndex]->vPos.y , 0.f },
        { m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
        { m_vecTile[iIndex]->vPos.x - (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f }
    };

    D3DXVECTOR3     vDir[4]{
        {vPoint[1] - vPoint[0]},
        {vPoint[2] - vPoint[1]},
        {vPoint[3] - vPoint[2]},
        {vPoint[0] - vPoint[3]},
    };

    // 각 포인트에서 뻗어나가는 직선의 법선벡터
    D3DXVECTOR3     vNormal[4]{
        {-vDir[0].y , vDir[0].x, 0.f},
        {-vDir[1].y , vDir[1].x, 0.f},
        {-vDir[2].y , vDir[2].x, 0.f},
        {-vDir[3].y , vDir[3].x, 0.f}
    };

    D3DXVECTOR3     vMouseDir[4]{
        vPos - vPoint[0],
        vPos - vPoint[1],
        vPos - vPoint[2],
        vPos - vPoint[3]
    };

    for (int i = 0; i < 4; ++i)
    {
        D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
        D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);

        // 예각일 경우 내적의 값이 0보다 크므로 false 반환
        if (0.f < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
            return false;
    }

    return true;
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
    D3DXVECTOR3 vCenter = { fCenterX,fCenterY,0.f };

    CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
        nullptr,
        &vCenter,
        nullptr,
        D3DCOLOR_ARGB(255, 255, 255, 255));
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

    return S_OK;
}

