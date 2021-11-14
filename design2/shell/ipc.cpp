#include"ipc.h"
void CloseFileMap()
{
	for (int i = 0; i < 2; i++)
	{
		//4.撤销文件视图UnMapViewOfFile()
		UnmapViewOfFile(m_pBuf[i]);
		//5.关闭映射文件句柄CloseHandle()
		CloseHandle(m_hMapFile[i]);
	}
}
void CreateFileMap()
{
	m_cstrSzName[0] = L"NameOfMappingObject0";//共享内存0的名字，程序A和B中对该变量的命名必须相同！
	m_cstrSzName[1] = L"NameOfMappingObject1";//共享内存1的名字，程序A和B中对该变量的命名必须相同！
	m_ReadSemaphoreName[0] = L"ReadSemaphoreName0";//读信号量0的名字，程序A和B中对该变量的命名必须相同！
	m_ReadSemaphoreName[1] = L"ReadSemaphoreName1";//读信号量1的名字，程序A和B中对该变量的命名必须相同！
	m_WriteSemaphoreName[0] = L"WriteSemaphoreName0";//写信号量0的名字，程序A和B中对该变量的命名必须相同！
	m_WriteSemaphoreName[1] = L"WriteSemaphoreName1";//写信号量1的名字，程序A和B中对该变量的命名必须相同！
	for (int i = 0; i < 2; i++)
	{
		//1.创建共享文件句柄 hMapFile，CreateFileMapping()函数创建一个文件映射内核对象
		m_hMapFile[i] = CreateFileMapping(
			INVALID_HANDLE_VALUE,     //物理文件句柄，设为INVALID_HANDLE_VALUE（无效句柄）以创建一个进程间共享的对象
			NULL,				      //默认安全级别
			PAGE_READWRITE,           //权限可读可写
			0,						  //高位文件大小
			BUF_SIZE,				  //低位文件大小
			(LPCWSTR)m_cstrSzName[i].c_str()			  //共享内存名
		);
		//2.获取指向文件视图的指针 pBuf，MapViewOfFile()函数负责把文件数据映射到进程的地址空间
		m_pBuf[i] = (char*)MapViewOfFile(
			m_hMapFile[i],				  //共享内存的句柄
			FILE_MAP_ALL_ACCESS,      //可读写
			0,
			0,
			BUF_SIZE
		);
	}
	m_Read[0] = CreateSemaphore(NULL, 0, 1, (LPCWSTR)m_ReadSemaphoreName[0].c_str());
	m_Write[0] = CreateSemaphore(NULL, 1, 1, (LPCWSTR)m_WriteSemaphoreName[0].c_str());
	m_Read[1] = CreateSemaphore(NULL, 0, 1, (LPCWSTR)m_ReadSemaphoreName[1].c_str());
	m_Write[1] = CreateSemaphore(NULL, 1, 1, (LPCWSTR)m_WriteSemaphoreName[1].c_str());
}
unsigned int __stdcall WriteSharedData(void* pPM)
{
	//3.将数据放到共享内存
	while (true)
	{
		//通过m_pBuf[0]向共享内存m_hMapFile[0]发送数据
		WaitForSingleObject(m_Write[1], INFINITE);
		char szInfo[BUF_SIZE] = { 0 };
		gets_s(szInfo, BUF_SIZE);//c++11中用gets_s() 代替gets()
		memcpy(m_pBuf[1], szInfo, BUF_SIZE - 1);
		system("cls");
		ReleaseSemaphore(m_Read[1], 1, NULL);

	}
	return true;
}
unsigned int __stdcall ReadSharedData(void* pPM)
{
	//3.从共享内存中读取数据
	while (true)
	{
		WaitForSingleObject(m_Read[0], INFINITE);//m_Read[1]信号量由程序B增加
		cout << m_pBuf[0] ;
		ReleaseSemaphore(m_Write[0], 1, NULL);
	}
	return true;
}