// UnitTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "UnitTool.h"
#include"FileInfo.h"
#include"TimeMgr.h"
#include"TextureMgr.h"


// CUnitTool 대화 상자

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CUnitTool, pParent)
	, m_iA(0)
	, m_iR(0)
	, m_iG(0)
	, m_iB(0)
	, fSpeed(0)
	, m_ObjKey(_T(""))
	, m_fAnimTime(0)
	, m_iImgCount(0)
	, m_strImgSize(_T(""))
	, m_strTexType(_T(""))
{

}

CUnitTool::~CUnitTool()
{
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TREE1, m_Tree);
	DDX_Text(pDX, IDC_EDIT1, m_iA);
	DDX_Text(pDX, IDC_EDIT2, m_iR);
	DDX_Text(pDX, IDC_EDIT3, m_iG);
	DDX_Text(pDX, IDC_EDIT4, m_iB);
	DDX_Text(pDX, IDC_EDIT8, fSpeed);
	DDX_Control(pDX, IDC_LIST3, m_ListCtrl);
	DDX_Text(pDX, IDC_EDIT5, m_ObjKey);
	DDX_Control(pDX, IDC_COMBO1, m_ComboBox);
	DDX_Text(pDX, IDC_TEXT4, m_fAnimTime);
	DDX_Text(pDX, IDC_TEXT3, m_iImgCount);
	DDX_Text(pDX, IDC_TEXT2, m_strImgSize);
	DDX_Text(pDX, IDC_TEXT1, m_strTexType);
	DDX_Control(pDX, IDC_BUTTON3, m_PlayButton);
	DDX_Control(pDX, IDC_EDIT8, m_SpeedControl);
	DDX_Control(pDX, IDC_SLIDER1, m_SliderA);
	DDX_Control(pDX, IDC_SLIDER2, m_SliderR);
	DDX_Control(pDX, IDC_SLIDER3, m_SliderG);
	DDX_Control(pDX, IDC_SLIDER4, m_SliderB);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CUnitTool::OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_BUTTON1, &CUnitTool::OnBnClickedButton1)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_LIST3, &CUnitTool::OnSelectList)
	ON_BN_CLICKED(IDC_BUTTON3, &CUnitTool::OnAnimPlay)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CUnitTool::OnDropDownSelChange)
	ON_EN_CHANGE(IDC_EDIT8, &CUnitTool::OnSpeedChange)
	ON_EN_CHANGE(IDC_EDIT1, &CUnitTool::OnAlphaChange)
	ON_EN_CHANGE(IDC_EDIT2, &CUnitTool::OnRedChange)
	ON_EN_CHANGE(IDC_EDIT3, &CUnitTool::OnGreenChange)
	ON_EN_CHANGE(IDC_EDIT4, &CUnitTool::OnBlueChange)
//	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


void CUnitTool::OnDestroy()
{
	//맵 할당 delete
	for (auto& iter : m_SpriteMap) {

		for (auto& it : iter.second) {
			Safe_Delete<SPRITEINFO*>(it);
			it = nullptr;
		}
	}


	m_SpriteMap.clear();
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	//트리뷰 세팅

	HTREEITEM root;
	root = m_Tree.InsertItem(TEXT("Texture"), 0, 0, TVI_ROOT, TVI_LAST);

	CString tpath = _T("../Texture");
	SetTreeItems(tpath, 0, root);

	//리스트 컨트롤러 세팅
	CRect rc;
	m_ListCtrl.GetClientRect(&rc);

	m_ListCtrl.InsertColumn(0, L"StateKey", LVCFMT_LEFT, 150);
	m_ListCtrl.InsertColumn(1, L"StateKey2", LVCFMT_LEFT, rc.Width() - 150);

	//애니메이션 view 창 동적 할당
	CRuntimeClass* pObject;
	pObject = RUNTIME_CLASS(CMiniAnimView);
	m_pAnimView = (CMiniAnimView*)pObject->CreateObject();

	CRect rc2;
	GetClientRect(&rc2);

	if (!m_pAnimView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(rc2.right- rc2.right *0.4, rc2.top+ 20.f , rc2.right-20.f, rc.top + rc2.right * 0.4), this, AFX_IDW_PANE_FIRST, NULL))
	{
		return -1;
	}

	m_SliderA.SetRange(0, 255, 0);
	m_SliderR.SetRange(0, 255, 0);
	m_SliderG.SetRange(0, 255, 0);
	m_SliderB.SetRange(0, 255, 0);

	m_SliderA.SetLineSize(1);
	m_SliderR.SetLineSize(1);
	m_SliderG.SetLineSize(1);
	m_SliderB.SetLineSize(1);

	m_SliderA.SetTicFreq(10);
	m_SliderR.SetTicFreq(10);
	m_SliderG.SetTicFreq(10);
	m_SliderB.SetTicFreq(10);

	m_SliderA.SetPageSize(10);
	m_SliderR.SetPageSize(10);
	m_SliderG.SetPageSize(10);
	m_SliderB.SetPageSize(10);

	return TRUE;
}

void CUnitTool::SetTreeItems(CString _path, int iCount, HTREEITEM _item)
{
	CFileFind Find;
	_path += "\\*.*";
	BOOL IsWorking = Find.FindFile(_path);
	CString DirPath;

	while (IsWorking) {

		IsWorking = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		if (Find.IsDirectory()) {

			CString fileName;

			DirPath = Find.GetFilePath();
			fileName = Find.GetFileName();

			CFileFind Find2;
			BOOL IsWorking2 = Find2.FindFile(DirPath + "\\*.*");
			BOOL IsDirFin = true;

			while (IsWorking2)
			{
				IsWorking2 = Find2.FindNextFile();

				if (Find2.IsDots())
					continue;

				if (Find2.IsDirectory()) {
					IsDirFin = false;
					IsWorking2 = false;
				}
			}


			if (!IsDirFin) {
				HTREEITEM treeItem = m_Tree.InsertItem(fileName, 0, 0, _item, TVI_LAST);
				SetTreeItems(DirPath, ++iCount, treeItem);
			}
			else {
				if(m_SpriteMap.end() == m_SpriteMap.find(m_Tree.GetItemText(_item)))
					SettingStateList(m_Tree.GetItemText(_item), _item);
			}

			continue;
		}
	}
}

void CUnitTool::OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM currentItem = m_Tree.GetSelectedItem();

	if (!m_Tree.ItemHasChildren(currentItem)) {
		//최하단 디렉토리 = 오브젝트키가 될 아이템을 선택했을 때
		CString ItemName = m_Tree.GetItemText(currentItem);
		m_ObjKey = m_Tree.GetItemText(currentItem);
		SetStateKeyList(ItemName, currentItem);
	}

	*pResult = 0;
}

void CUnitTool::SetStateKeyList(CString _objKey, HTREEITEM _item)
{
	auto iter = m_SpriteMap.find(_objKey);
	if (m_SpriteMap.end() == iter) {
		return;
	}

	m_ListCtrl.DeleteAllItems();

	for (auto& it : iter->second) {
		int num = m_ListCtrl.GetItemCount();

		m_ListCtrl.InsertItem(num, it->strStateKey);
		m_ComboBox.AddString(it->strStateKey);

	}
}

//텍스쳐 추가
void CUnitTool::SettingStateList(CString _objKey, HTREEITEM _item)
{
	BOOL IsRoot = false;
	CString filePath = _T("\\") + _objKey + "\\*.*";
	HTREEITEM originItem = _item;

	while (!IsRoot) {

		HTREEITEM parentItem = m_Tree.GetParentItem(originItem);
		CString Parentpath = m_Tree.GetItemText(parentItem);

		//최상단 root
		if ("Texture" == Parentpath) {
			filePath = _T("../Texture") + filePath;
			IsRoot = true;
		}
		else {

			Parentpath = _T("\\") + Parentpath;
			filePath = Parentpath + filePath;
			originItem = parentItem;
		}
	}


	CFileFind Finder;
	BOOL IsWorking = Finder.FindFile(filePath);
	vector<SPRITEINFO*> m_tempVec = {};

	while (IsWorking) {
		IsWorking = Finder.FindNextFile();

		if (Finder.IsDots())
			continue;

		SPRITEINFO* newSprite = new SPRITEINFO;
		newSprite->fSpeed = 0.1f;
		newSprite->iA = 255;
		newSprite->iR = 255;
		newSprite->iG = 255;
		newSprite->iB = 255;
		newSprite->strStateKey = Finder.GetFileName();
		newSprite->strObjKey = _objKey;

		m_tempVec.push_back(newSprite);

		CString filePath = CFileInfo::ConvertRelativePath(Finder.GetFilePath());
		newSprite->iCount=SetStateImg(filePath, Finder.GetFileName() ,_objKey);
	}

	m_SpriteMap.insert({ _objKey , m_tempVec });

}

int CUnitTool::SetStateImg(CString _path, CString _stateKey, CString _objKey)
{
	CFileFind Find;
	CString OriginPath = _path;

	_path += "\\*.*";
	bool isWorking = Find.FindFile(_path);
	int iCount = 0;
	CString fileName = L"";

	list<CString> temp;

	while (isWorking) {
		isWorking = Find.FindNextFile();

		if (Find.IsDirectory() || Find.IsDots()) {
			continue;
		}

		if(L"" == fileName)
			fileName = Find.GetFileName();

		++iCount;
		temp.push_back(CFileInfo::ConvertRelativePath(Find.GetFilePath()));

	}

	fileName.Replace(L"0.png", L"");
	CString newPath = OriginPath +"\\" + fileName + "%d.png";

	CTextureMgr::Get_Instance()->Insert_Texture(TEX_MULTI, newPath, _objKey, _stateKey, iCount);

	return iCount;

}

//스프라이트 저장하기 버튼
void CUnitTool::OnBnClickedButton1()
{



}


void CUnitTool::OnSelectList(NMHDR* pNMHDR, LRESULT* pResult)
{
	UpdateData(TRUE);

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int currentCol = pNMLV->iItem;

	CString SelectItem = m_ListCtrl.GetItemText(currentCol, 0);

	CString ObjKey = m_Tree.GetItemText(m_Tree.GetSelectedItem());

	auto iter = m_SpriteMap.find(ObjKey);
	int comboCount = m_ComboBox.FindString(0, SelectItem);
	m_ComboBox.SetCurSel(comboCount);

	for (auto& it : iter->second) {
		if (SelectItem == it->strStateKey) {
			m_iA = it->iA;
			m_iR = it->iR;
			m_iG = it->iG;
			m_iB = it->iB;
			fSpeed = it->fSpeed;
			m_ObjKey = ObjKey;

			m_SliderA.SetPos(m_iA);
			m_SliderR.SetPos(m_iR);
			m_SliderG.SetPos(m_iG);
			m_SliderB.SetPos(m_iB);

			m_iImgCount = it->iCount;
			m_fAnimTime = it->fSpeed;

			if (1 == m_iImgCount) {
				m_strTexType = "SINGLE";
			}
			else {
				m_strTexType = "MULTI";
			}

			m_currentFrame.fMax = (float)m_iImgCount;
			m_currentFrame.fFrame = 0.f;
			m_pAnimView->SetAnimInfo(*it);

			break;
		}
	}
	m_pAnimView->SelectSprite(true);

	*pResult = 0;

	UpdateData(FALSE);

}


void CUnitTool::OnAnimPlay()
{
	m_pAnimView->PlayAnimation();
	
	CString tempStr;

	m_PlayButton.GetWindowText(tempStr);

	if (L"❚❚" == tempStr) {
		m_PlayButton.SetWindowText(L"▶");
		m_SpeedControl.EnableWindow(TRUE);
	}
	else {
		m_PlayButton.SetWindowText(L"❚❚");
		m_SpeedControl.EnableWindow(FALSE);
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CUnitTool::OnDropDownSelChange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString tempStr;
	m_ComboBox.GetLBText(m_ComboBox.GetCurSel(), tempStr);
	
	CString objKey = m_ObjKey;
	if (L"" == objKey)
		return;
	

	auto iter = m_SpriteMap.find(objKey);

	for (auto& it : iter->second) {
		if (tempStr == it->strStateKey) {
			m_iA = it->iA;
			m_iR = it->iR;
			m_iG = it->iG;
			m_iB = it->iB;
			fSpeed = it->fSpeed;
			m_ObjKey = objKey;

			m_SliderA.SetPos(m_iA);
			m_SliderR.SetPos(m_iR);
			m_SliderG.SetPos(m_iG);
			m_SliderB.SetPos(m_iB);

			m_iImgCount = it->iCount;
			m_fAnimTime = it->fSpeed;

			if (1 == m_iImgCount) {
				m_strTexType = "SINGLE";
			}
			else {
				m_strTexType = "MULTI";
			}

			m_currentFrame.fMax = (float)m_iImgCount;
			m_currentFrame.fFrame = 0.f;
			m_pAnimView->SetAnimInfo(*it);

			break;
		}
	}
	m_pAnimView->SelectSprite(true);

	UpdateData(FALSE);
	
}


void CUnitTool::OnSpeedChange()
{
	CString strValue;
	GetDlgItemText(IDC_EDIT8, strValue);
	
	float value = _ttof(strValue);
	
	CString objKey = m_ObjKey;
	int currentCol = m_ComboBox.GetCurSel();
	CString stateKey;
	m_ComboBox.GetLBText(currentCol, stateKey);

	auto iter = m_SpriteMap.find(objKey);
	
	for (auto& it : iter->second) {
		if (it->strStateKey == stateKey) {
			it->fSpeed = value;
			m_pAnimView->SetAnimInfo(*it);
		}
	}

}


void CUnitTool::OnAlphaChange()
{
	CString strValue;
	GetDlgItemText(IDC_EDIT1, strValue);

	int value = _ttoi(strValue);

	CString objKey = m_ObjKey;
	int currentCol = m_ComboBox.GetCurSel();
	CString stateKey;
	m_ComboBox.GetLBText(currentCol, stateKey);

	auto iter = m_SpriteMap.find(objKey);

	for (auto& it : iter->second) {
		if (it->strStateKey == stateKey) {
			it->iA = value;
			m_pAnimView->SetAnimInfo(*it);
		}
	}
	
}


void CUnitTool::OnRedChange()
{
	CString strValue;
	GetDlgItemText(IDC_EDIT2, strValue);

	int value = _ttoi(strValue);

	CString objKey = m_ObjKey;
	int currentCol = m_ComboBox.GetCurSel();
	CString stateKey;
	m_ComboBox.GetLBText(currentCol, stateKey);

	auto iter = m_SpriteMap.find(objKey);

	for (auto& it : iter->second) {
		if (it->strStateKey == stateKey) {
			it->iR = value;
			m_pAnimView->SetAnimInfo(*it);
		}
	}
}


void CUnitTool::OnGreenChange()
{
	CString strValue;
	GetDlgItemText(IDC_EDIT3, strValue);

	int value = _ttoi(strValue);

	CString objKey = m_ObjKey;
	int currentCol = m_ComboBox.GetCurSel();
	CString stateKey;
	m_ComboBox.GetLBText(currentCol, stateKey);

	auto iter = m_SpriteMap.find(objKey);

	for (auto& it : iter->second) {
		if (it->strStateKey == stateKey) {
			it->iG = value;
			m_pAnimView->SetAnimInfo(*it);
		}
	}
}


void CUnitTool::OnBlueChange()
{
	CString strValue;
	GetDlgItemText(IDC_EDIT4, strValue);

	int value = _ttoi(strValue);

	CString objKey = m_ObjKey;
	int currentCol = m_ComboBox.GetCurSel();
	CString stateKey;
	m_ComboBox.GetLBText(currentCol, stateKey);

	auto iter = m_SpriteMap.find(objKey);

	for (auto& it : iter->second) {
		if (it->strStateKey == stateKey) {
			it->iB = value;
			m_pAnimView->SetAnimInfo(*it);
		}
	}
}

//RGBA 슬라이더
void CUnitTool::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int iCurPos = 0;
	UpdateData(TRUE);

	CString objKey = m_ObjKey;
	int currentCol = m_ComboBox.GetCurSel();
	CString stateKey;
	m_ComboBox.GetLBText(currentCol, stateKey);

	auto iter = m_SpriteMap.find(objKey);

	if ((CScrollBar*)&m_SliderA == pScrollBar) {

		iCurPos = m_SliderA.GetPos();
		m_iA = iCurPos;

		for (auto& it : iter->second) {
			if (it->strStateKey == stateKey) {
				it->iA = iCurPos;
				m_pAnimView->SetAnimInfo(*it);
			}
		}
	}
	else if ((CScrollBar*)&m_SliderR == pScrollBar) {
		iCurPos = m_SliderR.GetPos();
		m_iR = iCurPos;

		for (auto& it : iter->second) {
			if (it->strStateKey == stateKey) {
				it->iR = iCurPos;
				m_pAnimView->SetAnimInfo(*it);
			}
		}
	}
	else if ((CScrollBar*)&m_SliderG == pScrollBar) {
		iCurPos = m_SliderG.GetPos();
		m_iG = iCurPos;

		for (auto& it : iter->second) {
			if (it->strStateKey == stateKey) {
				it->iG = iCurPos;
				m_pAnimView->SetAnimInfo(*it);
			}
		}
	}
	else if ((CScrollBar*)&m_SliderB == pScrollBar) {
		iCurPos = m_SliderB.GetPos();
		m_iB = iCurPos;

		for (auto& it : iter->second) {
			if (it->strStateKey == stateKey) {
				it->iB = iCurPos;
				m_pAnimView->SetAnimInfo(*it);
			}
		}
	}


	UpdateData(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
