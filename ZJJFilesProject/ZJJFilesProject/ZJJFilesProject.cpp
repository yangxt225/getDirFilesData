// ZJJFilesProject.cpp : �������̨Ӧ�ó������ڵ㡣
// @Author��yxtxiaotian@163.com
// @Date��2016-11-25

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

// �����������ṩĬ�ϲ���
void GetDirFiles(string DirPath=SRC_FILES_PATH, int cutNum=CUT_CHAR_NUM);

/*
* �������ܣ��ݹ����Ŀ¼�������ı��ļ��������ν�ȡ�ļ�ÿһ�п�ͷcutNum���ֽڣ��������ZJJOutFile.txt
* DirPath���ļ���Ŀ¼·��
* cutNum��ÿһ�н�ȡ���ֽ���
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

	// ʹ��IO������ļ�Ŀ¼����,"*" ����Ҫ�������е�����
	if ((hFile = _findfirst(pathName.assign(DirPath).append("*").c_str(), &fileInfo)) == -1) 
	{
		cout << "[ERROR]Find files fail" << endl;
		return;
	}
	// ����ļ���ָ�룬׷�ӷ�ʽ���ļ�
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
			// �ݹ鴦�����ļ���
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
				// fgets��ȡֱ��ָ����ȡ���ȣ��ļ�β����'\n'; �ı�ÿ�����ݲ�����512Bytes.
				while((fgets(buf, LINE_MAX_SIZE, fin))!=NULL)
				{
					buf[cutNum] = '\n';
					// fputs д���ַ����ļ���ֱ������'\0'��'\0'���ᱻд��.
					buf[cutNum+1] = '\0';
					// д��ǰ9���ַ����ļ���������1�����з�
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
	// û�����������ʹ�����
	for(int i=0; i<argc; i++)
	{
		if (strcmp(argv[i], "-p")==0)
		{
			// �ļ�·��
			filesPath = argv[++i];
		}
		else if(strcmp(argv[i], "-n")==0)
		{
			// ��ȡ�ַ���
			cutNum = atoi(argv[++i]);
		}
	}
	GetDirFiles(filesPath, cutNum);
	system("pause");
	return 0;
}