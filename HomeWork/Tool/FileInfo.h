#pragma once
#include "Include.h"

class CFileInfo
{
public:
	CFileInfo() = default;
	~CFileInfo() = default;

public:
	// 절대경로를 상대경로로 바꾸어주는 함수
	static CString ConvertRelativePath(CString strFullPath);

};

