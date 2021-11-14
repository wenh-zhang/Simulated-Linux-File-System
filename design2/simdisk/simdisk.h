/*
simidisk.h
��ṹ����ͷ�ļ�
*/
#define _CRT_SECURE_NO_WARNINGS
#include<ctime>
#include<iostream>
#include<fstream>
#include<sstream>
#include<conio.h>
#include<Windows.h>
#include<iomanip>
#include<string>
#include<vector>
using namespace std;

#pragma once
#define MAX_USER_NUM		10		//�û�����
#define MAX_PWD_LENGTH		20		//������󳤶�
#define MAX_ID_LENGTH		10		//ID��󳤶�

#define MAX_PATH_LENGTH		256		//·����󳤶�
#define MAX_NAME_LENGTH		128		//�ļ���/Ŀ¼����󳤶�

#define MAX_SUBITEM_NUM		256		//Ŀ¼�����ļ��������

#define MAX_COMMAND_LENGTH	128		//������󳤶�
#define COMMAND_COUNT		18		//��������

#define BLOCK_SIZE			1024	//�̿��С��1KB
#define DIR_SIZE			sizeof(Dir) / BLOCK_SIZE+1  //Ŀ¼�ߴ�

#define BLOCKS_EACH			1024	//ÿ�����ݿ�����̿���
#define INODES_EACH			1024	//ÿ�����ݿ����i-�����
#define BLOCK_GROUPS_NUM	100		//���ݿ������Ŀ
#define BLOCKS_NUM			(BLOCKS_EACH * BLOCK_GROUPS_NUM)		//�ܵ��̿���
#define INODES_NUM			(INODES_EACH * BLOCK_GROUPS_NUM)		//�ܵ�i-�����
//�������׵�ַ
#define DATA_AREA_ADDR		(sizeof(SuperBlock) * BLOCK_GROUPS_NUM + sizeof(bmp_status) * (INODES_NUM + BLOCKS_NUM) + sizeof(Inode) * INODES_NUM)

enum error_num {
	LOGON_FAILED,			//��¼ʧ��
	WRONG_PWD,				//�������
	TOO_MANY_ARGS,			//��������
	INVILID_CMD,			//��Ч����

	NO_PRIVILEGE,			//û��Ȩ��
	DIR_READONLY,			//Ŀ¼ֻ��
	FILE_READONLY,			//�ļ�ֻ��
	DIR_WRITEONLY,			//Ŀ¼ֻд
	FILE_WRITEONLY,			//�ļ�ֻд

	READ_FAILED,			//�޷���ȡ�������
	WRITE_FAILED,			//�޷�д���������
	PATH_NOT_FOUND,			//�Ҳ���·��
	FILE_EXIST,				//�ļ��Ѵ���
	FILE_NOT_EXIST,			//�ļ�������
	DIR_NOT_EXIST,			//Ŀ¼������
	SPACE_NOT_ENOUGH,		//���̿ռ䲻��
	INODE_ALLOC_FAILED,		//i-������ʧ��

	CD_FAILED,				//�л�Ŀ¼ʧ��
	DIR_FAILED,				//��ʾĿ¼ʧ��
	MD_FAILED,				//����Ŀ¼ʧ��
	MD_MAX_FAILED,			//����Ŀ¼ʧ�ܣ���Ŀ¼���Ѵ����ֵ

	RD_FAILED,				//ɾ��Ŀ¼ʧ��
	NEW_FILE_FAILED,		//�����ļ�ʧ��
	CAT_FAILED,				//��ʾ�ļ�ʧ��
	DEL_FAILED,				//ɾ���ļ�ʧ��
	COPY_FAILED,			//�����ļ�ʧ��
	ERROR_PWD,				//��������
	
	NOT_BOTH_HOST,			//���ܾ�Ϊ�������ļ�
	HOST_FILE_NOT_EXIST,	//�������ļ�������
	HOST_FILE_WRITE_FAILED,	//�������ļ�д��ʧ��

	NO_DEL_CUR,				//����ɾ����ǰĿ¼
	
	WRONG_ARGS,				//��������
	USER_EXIST,				//�û��Ѵ���
	USER_NOT_EXIST,			//�û�������
	PROTECTED_USER,			//����ɾ����ǰ�û�
	NO_PRIVILEGE_USERDEL,   //û���ʸ�ɾ���û�
	USERS_MAX				//�û��ﵽ�������
};

//�û�����
enum  user_type {
	USER = 0,				//��ͨ�û�
	ADMIN = 1,				//����Ա
	SYSTEM = 2				//ϵͳ
};

//�û���
class User {
private:
	char userId[MAX_ID_LENGTH];							//�û��˺�� 10 λ
	char userPassword[MAX_PWD_LENGTH];	//�û����� � 20 λ
	user_type utType;						//�û�

public:
	
	void set_user_info(const char* userId, const char* userPassword, user_type utType);//�����û���Ϣ
	user_type get_user_type();					//��ȡ�û�����
	const char* get_user_id();							//��ȡ�û��˺�
	bool cmp_pwd(const char* strPassword);		//�Ƚ��ж������Ƿ���ȷ
};

//ʱ����
class Time {
	tm modify_time;
public:
	void set_time(tm tmTime);
	tm get_time();
};

// �ļ�����
enum file_attrib
{
	PROTECTED,	//ϵͳ
	READ_WRITE,	//��д
	READ_ONLY,	//ֻ��
	WRITE_ONLY,	//ֻд
	SHARE		//����
};

//�ļ�����
enum file_type
{
	TYPE_FILE = 1,	//�ļ�
	TYPE_DIR = 2	//Ŀ¼
};

//Ŀ¼��
class Dir
{
public:
	unsigned int num_Inode;						//i-����
	unsigned int num_child;						//���ļ�+��Ŀ¼����
	unsigned int num_SubInode[MAX_SUBITEM_NUM];	//����Ŀi-���

	bool have_child(const char* strDirName,unsigned int& index,file_type type);				//�Ƿ�����Ϊ strDirName ��Ŀ¼/���ļ�
	bool isFull();//��Ŀ¼�Ƿ����������Ŀ¼��
};

//i-�ڵ���
class Inode {
public:
	char fileName[MAX_NAME_LENGTH];		//i-�ڵ��Ӧ��Ŀ¼���ļ���
	file_attrib fileAttr;					//�ļ����ԣ���/д/ϵͳ/����/��д��
	file_type   fileType;					//�ļ����ͣ��ļ�/Ŀ¼��
	Time		fileMfyTime;				//�ļ�����޸�ʱ��
	char		userId[MAX_ID_LENGTH];						//�ļ�ӵ����
	unsigned long fileSize;					//��С
	unsigned long BlockAddr;				//���̿���ʼ��ַ
	unsigned int haveBlocks;				//ռ�ô��̿���
};

//��������   ÿ�����ݿ���ӵ��һ�����ݿ飬��100��
class SuperBlock
{
public:
	unsigned int allFreeBlocks;		//���п�����
	unsigned int allFreeInodes;		//����i-�������
	unsigned long BlockAddr;		//������������ַ
	unsigned int FreeBlocksNum;		//���п����Ŀ
	unsigned int FreeInodesNum;		//����i-������Ŀ
};

enum bmp_status	// λͼ״̬
{
	NOT_USED = 0,		//δ��ʹ��
	USED = 1			//�ѱ�ʹ��
};

// ������������
static const char* strCommands[COMMAND_COUNT] =
{
	"info","cd", "dir", "md", "rd", "newfile",
	"cat","copy", "del","check","init","cls",
	"help","logon","useradd", "userdel", "exit"
};

//ͷ�ļ��в��������������˶��嵽simdisk.cpp����ͨ��extern����simdisk.h��
extern char cur_userId[MAX_ID_LENGTH];		//��ǰ�û��˺�ID
extern user_type cur_utType;					//��ǰ�û�����
extern unsigned int cur_Idsum;						//��ǰ�û�����

extern char cur_dirName[MAX_NAME_LENGTH];				//��ǰĿ¼����
extern Dir cur_dir;									//��ǰĿ¼

extern fstream fs;									//����Linux�����ļ�
extern User systemUsers[MAX_USER_NUM];					//ϵͳ�û�����

extern SuperBlock sbGroups[BLOCK_GROUPS_NUM];			//���ݿ��飨�������飩
extern Inode indTable[INODES_NUM];						//i-����
extern bmp_status bsBlockBmp[BLOCKS_NUM];				//���ݿ�λͼ����
extern bmp_status bsInodeBmp[INODES_NUM];				//i-���λͼ����