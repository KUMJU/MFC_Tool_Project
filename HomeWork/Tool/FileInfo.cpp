#include "pch.h"
#include "FileInfo.h"

CString CFileInfo::ConvertRelativePath(CString strFullPath)
{
	TCHAR szRelativePath[MAX_PATH] = L"";
	TCHAR szCurDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szCurDirPath);

	// PathRelativePathTo : szCurDirPath���� strFullPath�� ���� ����θ� ���ؼ� szRelativePath�� �����ϴ� �Լ�
	// ��, ���� ����̺� �������� ���� ����

	PathRelativePathTo(szRelativePath,
						szCurDirPath,
						FILE_ATTRIBUTE_DIRECTORY,
						strFullPath.GetString(),
						FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}
