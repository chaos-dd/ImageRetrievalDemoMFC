/************************************************************************/
/* 
*
* @ 文件名： FindFiles.cpp
* @ 描述： 获取文件操作
* @ 作者： 唐志辉
* @ 时间： 2011.11.11
* @ 版本： 1.0
*
*/
/************************************************************************/
#include "stdafx.h"
#include "FindFiles.h"

FindFiles::FindFiles(void)
{
}

FindFiles::~FindFiles(void)
{
}


/************************************************************************/
/* 
 *
 * @ 函数名： getFatherPath
 * @ 描述： 获取给定目录的上级目录，foldername返回本级目录的名字
 * @ 作者： 
 * @ 时间： 2011.12.3
 *
*/
/************************************************************************/

string FindFiles::getFatherPath(string strDir,string &folderName)
{
	if (-1==_access(strDir.c_str(),0))
	{
		return "";
	}
	//string::npos
	int l_nPos1 = strDir.find_last_of("\\");
	int l_nPos2 = strDir.find_last_of("/");
	int l_nStrLen = strDir.size();
	if (l_nStrLen - 1 != l_nPos1)
	{
		folderName=strDir.substr(l_nPos1+1);
		return string(strDir.substr(0,l_nPos1-0));
	}
	else if (l_nStrLen - 1 != l_nPos2)
	{
		folderName=strDir.substr(l_nPos2+1);
		return string(strDir.substr(0,l_nPos2-1));
	}
	else
	{
		folderName=strDir;
		return strDir;
	}
}


/************************************************************************/
/* 
 *
 * @ 函数名： normalizeDir
 * @ 描述： 规范化目录，使目录结尾变为"\\"
 * @ 作者： 唐志辉
 * @ 时间： 2011.12.3
 *
*/
/************************************************************************/


void FindFiles::normalizeDir( string& strDir )
{
	int l_nPos1 = strDir.find_last_of("\\");
	int l_nPos2 = strDir.find_last_of("/");
	int l_nStrLen = strDir.size();
	if ((l_nStrLen - 1 != l_nPos1) && (l_nStrLen - 1 != l_nPos2))
	{
		strDir += "\\";
	}
}

/************************************************************************/
/* 
 *
 * @ 函数名： findFilesInDir
 * @ 描述： 获取目录strDir下的所有类型为strFileType的文件（不包括文件夹）
 * @ 作者： 唐志辉
 * @ 时间： 2011.11.11
 *
*/
/************************************************************************/

void FindFiles::findFilesInDir( FileList& listResltFiles , const string strDir /*= "."*/ , const string strFileType /*= "*.*"*/ )
{
	_finddata_t l_fileInfo;
	unsigned long l_hFile;
	string l_strDir = strDir;

	//规范目录格式
	if (!(strDir.find_last_of("\\") == strDir.size() - 1))
	{
		l_strDir += "\\";
	}
	l_strDir += strFileType;

	//查找第一个文件
	l_hFile = _findfirst(l_strDir.c_str() , &l_fileInfo);
	if (-1L == l_hFile)
	{
		//没有找到
		return;
	}

	if ("*.*" == strFileType)
	{
		//查找所有文件
		do 
		{
			if ((string(l_fileInfo.name) == ".") || (string(l_fileInfo.name) == ".."))
			{
				//上级文件夹，不处理
			}
			else if (_A_SUBDIR == l_fileInfo.attrib)
			{
				//文件夹，不处理
			}
			else
			{
				//要查找的文件，保存
				listResltFiles.push_back(string(l_fileInfo.name));
			}
		} while (0 ==_findnext(l_hFile , &l_fileInfo));

	}
	else
	{
		//查找指定格式的文件
		do 
		{
			//要查找的文件，保存
			listResltFiles.push_back(string(l_fileInfo.name));
		} while (0 ==_findnext(l_hFile , &l_fileInfo));
	}

	_findclose(l_hFile);
}

/************************************************************************/
/* 
*
* @ 函数名： findFilesInDirAndSubdirs
* @ 描述： 获取目录strDir及其子目录下的所有类型为strFileType的文件（不包括文件夹）
* @ 作者： 唐志辉
* @ 时间： 2011.11.11
*
*/
/************************************************************************/

void FindFiles::findFilesInDirAndSubdirs( FileList& listResltFiles , const string strDir /*= "." */, const string strFileType /*= "*.*"*/ )
{
	_finddata_t l_fileInfo;
	unsigned long l_hFile;
	string l_strDir = strDir;
	string l_strFileType = strFileType;

	//规范目录格式
// 	if (!(strDir.find_last_of("\\") == strDir.size() - 1))
// 	{
// 		l_strDir += "\\";
// 	}
	FindFiles::normalizeDir(l_strDir);
	l_strDir += "*.*";

	//查找第一个文件
	l_hFile = _findfirst(l_strDir.c_str() , &l_fileInfo);
	if (-1L == l_hFile)
	{
		//没有找到
		return;
	}

	//重新规范目录格式
	l_strDir = strDir;
// 	if (!(strDir.find_last_of("\\") == strDir.size() - 1))
// 	{
// 		l_strDir +=  "\\";
// 	}
	FindFiles::normalizeDir(l_strDir);

	//处理子文件夹文件夹
	FileList l_strSubDirs;
	findSubdirsInDir(l_strSubDirs , l_strDir);

	FileList::iterator l_iterator = l_strSubDirs.begin();
	while(l_iterator != l_strSubDirs.end())
	{
		findFilesInDirAndSubdirs(listResltFiles , l_strDir + (*l_iterator) , strFileType);
		l_iterator ++;
	}

	string l_strFileName = string(l_fileInfo.name);

	if ("*.*" == strFileType)
	{
		do
		{
			l_strFileName = string(l_fileInfo.name);
			if ((l_strFileName == ".") || (l_strFileName == ".."))
			{
				//上级文件夹，不处理
			}
			else if (_A_SUBDIR == l_fileInfo.attrib)
			{
				//文件夹前边已经处理，此处不再处理
			}
			else
			{
				//找到文件，保存
				listResltFiles.push_back(string(l_strDir + l_fileInfo.name));
			}
		}while(0 == _findnext(l_hFile , &l_fileInfo));
	}
	else
	{
		_findclose(l_hFile);
		l_hFile = -1L;
		l_hFile = _findfirst(string(l_strDir + strFileType).c_str() , &l_fileInfo);
		if (-1L == l_hFile)
		{
			return;
		}
		do 
		{
			listResltFiles.push_back(l_strDir + string(l_fileInfo.name));
		} while (0 == _findnext(l_hFile , &l_fileInfo));
	}

	_findclose(l_hFile);

}

/************************************************************************/
/* 
*
* @ 函数名： findSubdirsInDir
* @ 描述： 获取目录strDir下的所有文件夹
* @ 作者： 唐志辉
* @ 时间： 2011.11.11
*
*/
/************************************************************************/


void FindFiles::findSubdirsInDir( FileList& listResltFiles , const string strDir /* = "."*/ )
{
	_finddata_t l_fileInfo;
	unsigned long l_hFile;
	string l_strDir = strDir;

	listResltFiles.clear();

	//规范目录格式
	if (!(strDir.find_last_of("\\") == strDir.size() - 1))
	{
		l_strDir += "\\";
	}
	l_strDir += "*.*";

	//查找第一个文件
	l_hFile = _findfirst(l_strDir.c_str() , &l_fileInfo);
	if (-1L == l_hFile)
	{
		//没有找到
		return;
	}
	do 
	{
		string l_strFileName = string(l_fileInfo.name);
		if ((l_strFileName == ".") || (l_strFileName == ".."))
		{
			//上级文件夹，不处理
		}
		else if (_A_SUBDIR == l_fileInfo.attrib)
		{
			//找到文件夹，保存
			listResltFiles.push_back(string(l_fileInfo.name));
		}
	} while (0 == _findnext(l_hFile , &l_fileInfo));
	_findclose(l_hFile);
}


/************************************************************************/
/* 
*
* @ 函数名： findFilesAndSubdirsInDir
* @ 描述： 获取目录strDir下的所有文件夹和文件
* @ 作者： 唐志辉
* @ 时间： 2011.11.11
*
*/
/************************************************************************/


void FindFiles::findFilesAndSubdirsInDir( FileList& listResltFiles , const string strDir /*= "." */, const string strFileType /*= "*.*"*/ )
{
	findSubdirsInDir(listResltFiles , strDir);
	findFilesInDir(listResltFiles , strDir , strFileType);

}
