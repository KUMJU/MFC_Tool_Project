// UnitTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "UnitTool.h"


// CUnitTool 대화 상자

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CUnitTool, pParent)
{

}

CUnitTool::~CUnitTool()
{
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strName);
	DDX_Text(pDX, IDC_EDIT2, m_iHp);
	DDX_Text(pDX, IDC_EDIT3, m_iAttack);
	DDX_Text(pDX, IDC_EDIT6, m_strSearchName);
	DDX_Control(pDX, IDC_LIST2, m_ListBox);
	DDX_Control(pDX, IDC_CHECK1, m_Check[0]);
	DDX_Control(pDX, IDC_CHECK2, m_Check[1]);
	DDX_Control(pDX, IDC_CHECK3, m_Check[2]);
	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[2]);
	DDX_Control(pDX, IDC_BUTTON3, m_Bitmap);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
//	ON_BN_CLICKED(IDC_BUTTON1, &CUnitTool::OnAddPlayer)
//	ON_LBN_SELCHANGE(IDC_LIST2, &CUnitTool::OnListBox)
//	ON_BN_CLICKED(IDC_BUTTON2, &CUnitTool::OnBnClickedButton2)
	ON_WM_DESTROY()
//	ON_BN_CLICKED(IDC_BUTTON6, &CUnitTool::SaveUnitData)
//	ON_BN_CLICKED(IDC_BUTTON5, &CUnitTool::LoadUnitData)
//	ON_BN_CLICKED(IDC_BUTTON7, &CUnitTool::DeleteUnit)
END_MESSAGE_MAP()


// CUnitTool 메시지 처리기
//void CUnitTool::OnBnClickedButton2()
//{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// UpdateData(TRUE) : 다이얼로그 박스로부터 입력된 값들을 얻어옴
//	UpdateData(TRUE);
//
//	m_ListBox.SetCurSel(m_ListBox.FindString(0,m_strSearchName));
//
	// UpdateData(FALSE) : 변수에 저장된 값들을 다이얼로그 박스에 반영
//	UpdateData(FALSE);
//}


//void CUnitTool::OnAddPlayer()
//{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	UpdateData(TRUE);
//
//	UNITDATA* pUnit = new UNITDATA;
//
//	pUnit->strName = m_strName;
//	pUnit->iAttack = m_iAttack;
//	pUnit->iHp = m_iHp;
//
//	for (int i = 0; i < 3; ++i)
//	{
//		if (m_Radio[i].GetCheck())
//		{
//			pUnit->byJobIndex = i;
//			break;
//		}
//	}
//
	// 아이템 or연산으로 중복체크
//	pUnit->byItem = 0x00;
//
//	if (m_Check[0].GetCheck())
//		pUnit->byItem |= RUBY;
//	if (m_Check[1].GetCheck())
//		pUnit->byItem |= DIAMOND;
//	if (m_Check[2].GetCheck())
//		pUnit->byItem |= SAPPHIRE;
//
//	m_ListBox.AddString(pUnit->strName);
//
//	m_mapUnitData.insert({ pUnit->strName, pUnit });
//
//	UpdateData(FALSE);
//}


//void CUnitTool::OnListBox()
//{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

//	UpdateData(TRUE);
//
//	CString strFindName;
//
	// GetCurSel : 선택된 셀의 인덱스 값을 반환
//	int iIndex = m_ListBox.GetCurSel();
//
//	if (LB_ERR == iIndex)
//		return;
//
	// GetText : 현재 인덱스에 해당하는 문자열을 리스트 박스로부터 얻어옴
//	m_ListBox.GetText(iIndex, strFindName);
//
//	auto iter = m_mapUnitData.find(strFindName);
//
//	if (iter == m_mapUnitData.end())
//		return;
//
//	m_strName = iter->second->strName;
//	m_iHp = iter->second->iHp;
//	m_iAttack = iter->second->iAttack;
//	
//	for (int i = 0; i < 3; ++i)
//	{
//		m_Radio[i].SetCheck(FALSE);
//		m_Check[i].SetCheck(FALSE);
//	}
//
//	m_Radio[iter->second->byJobIndex].SetCheck(TRUE);
//
//	if (iter->second->byItem & RUBY)
//		m_Check[0].SetCheck(TRUE);
//	if (iter->second->byItem & DIAMOND)
//		m_Check[1].SetCheck(TRUE);
//	if (iter->second->byItem & SAPPHIRE)
//		m_Check[2].SetCheck(TRUE);
//
//
//	UpdateData(FALSE);
//}


void CUnitTool::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	for_each(m_mapUnitData.begin(), m_mapUnitData.end(), CDeleteMap());
	m_mapUnitData.clear();
}


BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	HBITMAP	hBitmap = (HBITMAP)LoadImage(nullptr, L"../Texture/JusinLogo1.bmp", IMAGE_BITMAP, 100, 50, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	m_Bitmap.SetBitmap(hBitmap);

	return TRUE; 
}


//void CUnitTool::SaveUnitData()
//{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	UpdateData(TRUE);
//
//	CFile file;
//	file.Open(L"../Data/UnitData/Units.txt", CFile::modeCreate | CFile::modeWrite, NULL);
//
//	size_t iCount = m_mapUnitData.size();
//	file.Write(&iCount, sizeof(size_t));
//
//	for_each(m_mapUnitData.begin(), m_mapUnitData.end(), [&](auto& MyPair)
//		{
//			UNITDATA* data = MyPair.second;
//			
//			TCHAR	szName[MAX_PATH];
//			lstrcpy(szName, data->strName.GetString());
//			file.Write(szName, MAX_PATH * sizeof(TCHAR));
//			file.Write(&(data->iHp), sizeof(int));
//			file.Write(&(data->iAttack), sizeof(int));
//			file.Write(&(data->byJobIndex), sizeof(BYTE));
//			file.Write(&(data->byItem), sizeof(BYTE));
//		});
//
//	file.Close();
//	AfxMessageBox(L"저장완료");
//
//
//	UpdateData(FALSE);
//}


//void CUnitTool::LoadUnitData()
//{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	UpdateData(TRUE);
//
//	size_t iCnt = 0;
//
//	CFile file;
//	file.Open(L"../Data/UnitData/Units.txt", CFile::modeRead, NULL);
//	file.Read(&iCnt, sizeof(size_t));
//
//	UNITDATA* pUnit = nullptr;
//	for (size_t i = 0; i < iCnt; ++i)
//	{
//		pUnit = new UNITDATA;
//		TCHAR	szName[MAX_PATH];
//		file.Read(szName, MAX_PATH * sizeof(TCHAR));
//		pUnit->strName = szName;
//		file.Read(&(pUnit->iHp), sizeof(int));
//		file.Read(&(pUnit->iAttack), sizeof(int));
//		file.Read(&(pUnit->byJobIndex), sizeof(BYTE));
//		file.Read(&(pUnit->byItem), sizeof(BYTE));
//
//		m_ListBox.AddString(pUnit->strName);
//		m_mapUnitData.insert({ pUnit->strName,pUnit });
//		pUnit = nullptr;
//	}
//
//	file.Close();
//	AfxMessageBox(L"불러오기 완료");
//
//	UpdateData(FALSE);
//}


//void CUnitTool::DeleteUnit()
//{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	UpdateData(TRUE);
//
//	CString strFindName;
//
	// GetCurSel : 선택된 셀의 인덱스 값을 반환
//	int iIndex = m_ListBox.GetCurSel();
//
//	if (LB_ERR == iIndex)
//		return;
//
	// GetText : 현재 인덱스에 해당하는 문자열을 리스트 박스로부터 얻어옴
//	m_ListBox.GetText(iIndex, strFindName);
//
//	auto iter = m_mapUnitData.find(strFindName);
//
//	if (iter == m_mapUnitData.end())
//		return;
//
//	m_strName = L"";
//	m_iHp = 0;
//	m_iAttack = 0;
//
//	m_ListBox.DeleteString(iIndex);
//	Safe_Delete(iter->second);
//	m_mapUnitData.erase(iter);
//	UpdateData(FALSE);
//}
