#pragma once
#pragma once
/*
ipc.h
���ڹ����ڴ�ͨ�ŵĺ�������
*/
#include <windows.h>
#include <process.h>
#include<iostream>
using namespace std;

#define _CRT_SECURE_NO_WARNINGS

constexpr auto BUF_SIZE = 1024;

extern stringstream sstream;
extern wstring   m_cstrSzName[2];//�����ڴ�����һ��������һ����A�����ݣ���Bȡ���ݣ�һ����B�����ݣ���Aȡ���ݣ�
extern wstring   m_ReadSemaphoreName[2]; //�����ݵ��ź�����
extern wstring   m_WriteSemaphoreName[2];//д���ݵ��ź�����
extern HANDLE  m_hMapFile[2];//�����ڴ���
extern char* m_pBuf[2]; //��ȡ���ݻ���
extern char szInfo[BUF_SIZE];//�ȴ洢ϵͳ������Ϣ�����ٴ���m_pBuf[0]
extern char getInfo[BUF_SIZE];//�洢�ӹ����ڴ��ȡ������
extern HANDLE	m_Read[2];//�����ݵ��ź������
extern HANDLE  m_Write[2];//д���ݵ��ź������

void CloseFileMap();
void CreateFileMap();
unsigned int __stdcall WriteSharedData(void* pPM);
unsigned int __stdcall ReadSharedData(void* pPM);

