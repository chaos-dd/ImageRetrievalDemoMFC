
/************************************************************************/
/* 
 *
 * @ 文件名： FindFiles.h
 * @ 描述： 获取文件操作
 * @ 作者： 唐志辉
 * @ 时间： 2011.11.11
 * @ 版本： 1.0
 *
*/
/************************************************************************/

#pragma once

#include "allInclude.h"


#include <io.h>
#include <string>
#include <vector>
#include <list>

#include <direct.h>

using namespace std;

typedef std::vector<string> FileList;
//typedef std::string string;
//typedef std::list<std::string> FileList;

class FindFiles
{

protected:

	static void normalizeDir(string& strDir); //规范化目录

public:
	FindFiles(void);
	~FindFiles(void);

	//不包括文件路径
	static void findFilesInDir(FileList& listResltFiles , const string strDir = "." , const string strFileType = "*.*"); //获取路径下的指定类型文件
	//包括文件路径
	static void findFilesInDirAndSubdirs(FileList& listResltFiles , const string strDir = "." , const string strFileType = "*.*"); //获取路径和子文件夹中所有指定类型文件
	static void findSubdirsInDir(FileList& listResltFiles , const string strDir = ".");
	static void findFilesAndSubdirsInDir(FileList& listResltFiles , const string strDir = "." , const string strFileType = "*.*"); //获取

	static string getFatherPath(string strDir,string &folderName=string());
};
