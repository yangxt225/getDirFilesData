// ZJJFilesProject.cpp : 定义控制台应用程序的入口点。
// @Author：yxtxiaotian@163.com
// @Date：2016-11-25

#include "stdafx.h"
#include <iostream>
#include <io.h>
#include <string>
#include<stdlib.h>
using namespace std;

#define SRC_FILES_PATH	"D:\\workspace\\ZJJFilesOpt\\ZJJfiles\\"
#define CUT_CHAR_NUM	8
#define  LINE_MAX_SIZE	512
#define PATH_MAX_SIZE 256

// 函数声明，提供默认参数
void GetDirFiles(string DirPath=SRC_FILES_PATH, int cutNum=CUT_CHAR_NUM);

/*
* 函数功能：递归遍历目录下所有文本文件，并依次截取文件每一行开头cutNum个字节，结果存入ZJJOutFile.txt
* DirPath：文件的目录路径
* cutNum：每一行截取的字节数
*/
void GetDirFiles(string DirPath, int cutNum)
{
	long hFile = 0;
	struct _finddata_t fileInfo;
	string pathName, SubPathName;
	char fullPath[PATH_MAX_SIZE] = {0};
	memcpy(fullPath, DirPath.c_str(), DirPath.length());
	FILE *fin = NULL, *fout = NULL;
	char buf[LINE_MAX_SIZE] = {0};
	memset(buf, 0, sizeof(buf));

	// 使用IO库进行文件目录操作,"*" 代表要遍历所有的类型
	if ((hFile = _findfirst(pathName.assign(DirPath).append("*").c_str(), &fileInfo)) == -1) 
	{
		cout << "[ERROR]Find files fail" << endl;
		return;
	}
	// 输出文件流指针，追加方式打开文件
	memcpy(fullPath+DirPath.length(), "ZJJOutFile.txt", strlen("ZJJOutFile.txt"));
	fopen_s(&fout, fullPath, "a+");

	do {
		if (!strcmp(fileInfo.name, "ZJJOutFile.txt"))
		{
			continue;
		}
		cout << fileInfo.name << (fileInfo.attrib & _A_SUBDIR? "[folder]":"[file]") << endl;
		if ( fileInfo.attrib & _A_SUBDIR )
		{
			// 递归处理子文件夹
			if (strcmp(fileInfo.name, ".") && strcmp(fileInfo.name, ".."))
			{
				GetDirFiles(SubPathName.assign(DirPath).append(fileInfo.name).append("\\"));
			}
		}
		else
		{
			memcpy(fullPath+DirPath.length(), fileInfo.name, strlen(fileInfo.name));
			fullPath[DirPath.length()+strlen(fileInfo.name)] = '\0';
			fin = NULL;
			fopen_s(&fin, fullPath, "r");
			if (fin != NULL)
			{
				// fgets读取直到指定读取长度，文件尾或者'\n'; 文本每行数据不超过512Bytes.
				while((fgets(buf, LINE_MAX_SIZE, fin))!=NULL)
				{
					buf[cutNum] = '\n';
					// fputs 写入字符到文件，直到遇到'\0'，'\0'不会被写入.
					buf[cutNum+1] = '\0';
					// 写入前9个字符到文件，包含了1个换行符
					fputs(buf, fout);
				}
				fflush(fout);
				fclose(fin);
			}
			else
			{
				cout << "[ERROR]Can not open file: " << fileInfo.name << endl;
				break;
			}
		}
	} while (_findnext(hFile, &fileInfo) == 0);
	fclose(fout);
	_findclose(hFile);
	return;
}

int main(int argc,char *argv[])
{
	string filesPath = "";
	int cutNum = 0;
	// 没有做参数检查和错误处理
	for(int i=0; i<argc; i++)
	{
		if (strcmp(argv[i], "-p")==0)
		{
			// 文件路径
			filesPath = argv[++i];
		}
		else if(strcmp(argv[i], "-n")==0)
		{
			// 截取字符数
			cutNum = atoi(argv[++i]);
		}
	}
	GetDirFiles(filesPath, cutNum);
	system("pause");
	return 0;
}