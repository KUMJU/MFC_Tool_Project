#pragma once
#include "Include.h"

class CFileInfo
{
public:
	CFileInfo() = default;
	~CFileInfo() = default;

public:
	// �����θ� ����η� �ٲپ��ִ� �Լ�
	static CString ConvertRelativePath(CString strFullPath);

};

