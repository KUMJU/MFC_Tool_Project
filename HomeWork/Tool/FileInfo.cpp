#include "pch.h"
#include "FileInfo.h"

CString CFileInfo::ConvertRelativePath(CString strFullPath)
{
	TCHAR szRelativePath[MAX_PATH] = L"";
	TCHAR szCurDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szCurDirPath);

	// PathRelativePathTo : szCurDirPath에서 strFullPath로 가는 상대경로를 구해서 szRelativePath에 저장하는 함수
	// 단, 같은 드라이브 내에서만 변경 가능

	PathRelativePathTo(szRelativePath,
						szCurDirPath,
						FILE_ATTRIBUTE_DIRECTORY,
						strFullPath.GetString(),
						FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}
