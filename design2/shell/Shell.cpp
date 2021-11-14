// Shell.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include"ipc.h"

#define BUF_SIZE 1024

wstring   m_cstrSzName[2];//共享内存名（一共两个，一个给A放数据，由B取数据；一个给B放数据，由A取数据）
wstring   m_ReadSemaphoreName[2];//读数据的信号量名
wstring   m_WriteSemaphoreName[2];//写数据的信号量名
HANDLE  m_hMapFile[2];//共享内存句柄
char* m_pBuf[2];//读取数据的缓存
HANDLE	m_Read[2];//读数据的信号量句柄
HANDLE  m_Write[2];//写数据的信号量句柄

int main()
{
	CreateFileMap();//创建共享内存
	UINT threadId;
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, WriteSharedData, 0, 0, &threadId);//创建写数据的线程
	if (hThread == NULL)
	{
		cout << "Starting WriteSharedData Thread  Failed!" << endl;
	}
	HANDLE hThread2 = (HANDLE)_beginthreadex(NULL, 0, ReadSharedData, 0, 0, &threadId);//创建读数据的线程
	if (hThread2 == NULL)
	{
		cout << "Starting ReadSharedData Thread  Failed!" << endl;
	}
	Sleep(1000000);//程序开始运行后1000s后自动关闭
	CloseFileMap();
	return 0;
}

