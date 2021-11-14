/*
simidisk.h
类结构定义头文件
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
#define MAX_USER_NUM		10		//用户总数
#define MAX_PWD_LENGTH		20		//密码最大长度
#define MAX_ID_LENGTH		10		//ID最大长度

#define MAX_PATH_LENGTH		256		//路径最大长度
#define MAX_NAME_LENGTH		128		//文件名/目录名最大长度

#define MAX_SUBITEM_NUM		256		//目录包含文件最大数量

#define MAX_COMMAND_LENGTH	128		//命令最大长度
#define COMMAND_COUNT		18		//命令数量

#define BLOCK_SIZE			1024	//盘块大小：1KB
#define DIR_SIZE			sizeof(Dir) / BLOCK_SIZE+1  //目录尺寸

#define BLOCKS_EACH			1024	//每个数据块组的盘块数
#define INODES_EACH			1024	//每个数据块组的i-结点数
#define BLOCK_GROUPS_NUM	100		//数据块组的数目
#define BLOCKS_NUM			(BLOCKS_EACH * BLOCK_GROUPS_NUM)		//总的盘块数
#define INODES_NUM			(INODES_EACH * BLOCK_GROUPS_NUM)		//总的i-结点数
//数据区首地址
#define DATA_AREA_ADDR		(sizeof(SuperBlock) * BLOCK_GROUPS_NUM + sizeof(bmp_status) * (INODES_NUM + BLOCKS_NUM) + sizeof(Inode) * INODES_NUM)

enum error_num {
	LOGON_FAILED,			//登录失败
	WRONG_PWD,				//密码错误
	TOO_MANY_ARGS,			//参数过多
	INVILID_CMD,			//无效命令

	NO_PRIVILEGE,			//没有权限
	DIR_READONLY,			//目录只读
	FILE_READONLY,			//文件只读
	DIR_WRITEONLY,			//目录只写
	FILE_WRITEONLY,			//文件只写

	READ_FAILED,			//无法读取虚拟磁盘
	WRITE_FAILED,			//无法写入虚拟磁盘
	PATH_NOT_FOUND,			//找不到路径
	FILE_EXIST,				//文件已存在
	FILE_NOT_EXIST,			//文件不存在
	DIR_NOT_EXIST,			//目录不存在
	SPACE_NOT_ENOUGH,		//磁盘空间不足
	INODE_ALLOC_FAILED,		//i-结点分配失败

	CD_FAILED,				//切换目录失败
	DIR_FAILED,				//显示目录失败
	MD_FAILED,				//创建目录失败
	MD_MAX_FAILED,			//创建目录失败，子目录数已达最大值

	RD_FAILED,				//删除目录失败
	NEW_FILE_FAILED,		//创建文件失败
	CAT_FAILED,				//显示文件失败
	DEL_FAILED,				//删除文件失败
	COPY_FAILED,			//复制文件失败
	ERROR_PWD,				//错误密码
	
	NOT_BOTH_HOST,			//不能均为宿主机文件
	HOST_FILE_NOT_EXIST,	//宿主机文件不存在
	HOST_FILE_WRITE_FAILED,	//宿主机文件写入失败

	NO_DEL_CUR,				//不能删除当前目录
	
	WRONG_ARGS,				//参数错误
	USER_EXIST,				//用户已存在
	USER_NOT_EXIST,			//用户不存在
	PROTECTED_USER,			//不能删除当前用户
	NO_PRIVILEGE_USERDEL,   //没有资格删除用户
	USERS_MAX				//用户达到最大数量
};

//用户类型
enum  user_type {
	USER = 0,				//普通用户
	ADMIN = 1,				//管理员
	SYSTEM = 2				//系统
};

//用户类
class User {
private:
	char userId[MAX_ID_LENGTH];							//用户账号最长 10 位
	char userPassword[MAX_PWD_LENGTH];	//用户密码 最长 20 位
	user_type utType;						//用户

public:
	
	void set_user_info(const char* userId, const char* userPassword, user_type utType);//设置用户信息
	user_type get_user_type();					//获取用户类型
	const char* get_user_id();							//获取用户账号
	bool cmp_pwd(const char* strPassword);		//比较判断密码是否正确
};

//时间类
class Time {
	tm modify_time;
public:
	void set_time(tm tmTime);
	tm get_time();
};

// 文件属性
enum file_attrib
{
	PROTECTED,	//系统
	READ_WRITE,	//读写
	READ_ONLY,	//只读
	WRITE_ONLY,	//只写
	SHARE		//共享
};

//文件类型
enum file_type
{
	TYPE_FILE = 1,	//文件
	TYPE_DIR = 2	//目录
};

//目录类
class Dir
{
public:
	unsigned int num_Inode;						//i-结点号
	unsigned int num_child;						//子文件+子目录总数
	unsigned int num_SubInode[MAX_SUBITEM_NUM];	//子项目i-结点

	bool have_child(const char* strDirName,unsigned int& index,file_type type);				//是否有名为 strDirName 子目录/子文件
	bool isFull();//该目录是否已满最大子目录数
};

//i-节点类
class Inode {
public:
	char fileName[MAX_NAME_LENGTH];		//i-节点对应的目录或文件名
	file_attrib fileAttr;					//文件属性（读/写/系统/共享/读写）
	file_type   fileType;					//文件类型（文件/目录）
	Time		fileMfyTime;				//文件最近修改时间
	char		userId[MAX_ID_LENGTH];						//文件拥有者
	unsigned long fileSize;					//大小
	unsigned long BlockAddr;				//磁盘块起始地址
	unsigned int haveBlocks;				//占用磁盘块数
};

//超级块类   每个数据块组拥有一个数据块，共100个
class SuperBlock
{
public:
	unsigned int allFreeBlocks;		//空闲块总数
	unsigned int allFreeInodes;		//空闲i-结点总数
	unsigned long BlockAddr;		//所在数据区地址
	unsigned int FreeBlocksNum;		//空闲块的数目
	unsigned int FreeInodesNum;		//空闲i-结点的数目
};

enum bmp_status	// 位图状态
{
	NOT_USED = 0,		//未被使用
	USED = 1			//已被使用
};

// 保存所有命令
static const char* strCommands[COMMAND_COUNT] =
{
	"info","cd", "dir", "md", "rd", "newfile",
	"cat","copy", "del","check","init","cls",
	"help","logon","useradd", "userdel", "exit"
};

//头文件中不允许定义变量，因此定义到simdisk.cpp，再通过extern放入simdisk.h中
extern char cur_userId[MAX_ID_LENGTH];		//当前用户账号ID
extern user_type cur_utType;					//当前用户类型
extern unsigned int cur_Idsum;						//当前用户数量

extern char cur_dirName[MAX_NAME_LENGTH];				//当前目录名称
extern Dir cur_dir;									//当前目录

extern fstream fs;									//虚拟Linux磁盘文件
extern User systemUsers[MAX_USER_NUM];					//系统用户数组

extern SuperBlock sbGroups[BLOCK_GROUPS_NUM];			//数据块组（超级块组）
extern Inode indTable[INODES_NUM];						//i-结点表
extern bmp_status bsBlockBmp[BLOCKS_NUM];				//数据块位图数组
extern bmp_status bsInodeBmp[INODES_NUM];				//i-结点位图数组