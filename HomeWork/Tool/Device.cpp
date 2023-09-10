#include "pch.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDevice)

CDevice::CDevice()
    :m_pSDK(nullptr), m_pDevice(nullptr)
{
}

CDevice::~CDevice()
{
    Release();
}

HRESULT CDevice::Init_Device()
{
    // 장치를 조사할 객체 생성
    m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

    // 장치 조사
    D3DCAPS9    DeviceCaps;
    ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

    // 장치에 대한 수준을 파악하여 DeviceCaps에 값 채워줌
    if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
    {
        AfxMessageBox(L"GetDeviceCaps Failed");
        return E_FAIL;
    }

    DWORD   vp = 0;

    // 장치를 파악하여 버텍스 프로세싱을 하드웨어(그래픽카드)에서 할지 소프트웨어(CPU)에서 할지 결정해줌
    if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else
        vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

    Set_Parameters(d3dpp);

    // 장치를 제어할 객체 생성
    // (어떤 그래픽 카드를 제어할 것인가, 
    // 어떤 정보로 장치에 접근할 것인가, 
    // 장치를 사용할 윈도우 핸들, 
    // 동작 방식, 
    // 사용환경, 
    // 결과값)
    if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, vp, &d3dpp, &m_pDevice)))
    {
        AfxMessageBox(L"CreateDevice Failed");
        return E_FAIL;
    }

    // 스프라이트 컴객체 생성
    if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
    {
        AfxMessageBox(L"Create Sprite Failed");
        return E_FAIL;
    }

    // 폰트 객체 생성
    D3DXFONT_DESCW      tFontInfo;
    ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

    tFontInfo.Height = 20;
    tFontInfo.Weight = 10;
    tFontInfo.Weight = FW_HEAVY;
    tFontInfo.CharSet = HANGEUL_CHARSET;
    lstrcpy(tFontInfo.FaceName, L"고딕");

    if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
    {
        AfxMessageBox(L"Create Sprite sss");
        return E_FAIL;
    }

    return S_OK;
}

void CDevice::Render_Begin()
{
    m_pDevice->Clear(0, // 렉트의 개수	
        nullptr,        // 렉트의 첫 번째 주소	
        D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,	// 지울 버퍼 종류
        D3DCOLOR_ARGB(255, 255, 255, 255), // 백버퍼 색상
        1.f,	// 깊이 버퍼 초기화 값
        0);		// 스텐실 버퍼 초기화 값

    // 후면버퍼에 그리기 시작
    m_pDevice->BeginScene();
    m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
    
}

void CDevice::Render_End(HWND hWnd)
{
    m_pSprite->End();
    m_pDevice->EndScene();
    m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);

    // D3DSWAPEFFECT_COPY일 때, 1, 2, 4 매개 변수가 값을 사용 가능
}

void CDevice::Release()
{
    Safe_Release(m_pFont);
    Safe_Release(m_pSprite);
    Safe_Release(m_pDevice);
    Safe_Release(m_pSDK);
}

void CDevice::Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp)
{
    d3dpp.BackBufferWidth = WINCX;
    d3dpp.BackBufferHeight = WINCY;
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dpp.BackBufferCount = 1;

    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3dpp.MultiSampleQuality = 0;


    //D3DSWAPEFFECT_DISCARD : 스왑 체인 방식
    //D3DSWAPEFFECT_FLIP : 버퍼 하나를 뒤집으면서 사용하는 방식
    //D3DSWAPEFFECT_COPY : 더블 버퍼링과 유사한 복사 방식

    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

    d3dpp.hDeviceWindow = g_hWnd;

    d3dpp.Windowed = TRUE;		// 창 모드 실행(FALSE 인 경우, 전체화면 모드 실행)

    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

    // 전체화면 모드일 경우 설정하는 멤버 변수

    // 모니터 재생률을 토대로 갱신 주기를 설정
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

    // 재생률과 시연의 간격 설정
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}
