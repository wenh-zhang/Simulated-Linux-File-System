/*
simdisk.cpp
执行程序
*/
#include"simdisk.h"
#include"func.h"
#include<sstream>

#define BUF_SIZE 1024*1024*100+1
char input[] = "input";    // 共享内存的名字
char output[] = "output";    // 共享内存的名字


char cur_userId[MAX_ID_LENGTH] = "000000";		//当前用户账号ID
user_type cur_utType = SYSTEM;					//当前用户类型
unsigned int cur_Idsum = 0;						//当前用户数量

char cur_dirName[MAX_NAME_LENGTH];				//当前目录名称
Dir cur_dir;									//当前目录

fstream fs;									//虚拟Linux磁盘文件
User systemUsers[MAX_USER_NUM];					//系统用户数组

SuperBlock sbGroups[BLOCK_GROUPS_NUM];			//数据块组（超级块组）
Inode indTable[INODES_NUM];						//i-结点表
bmp_status bsBlockBmp[BLOCKS_NUM];				//数据块位图数组
bmp_status bsInodeBmp[INODES_NUM];				//i-结点位图数组

//主函数
int main(int argc, char* argv[]) {
	system("title simdisk");
	load();
	if (!cmd_logon())return 0;
	system("cls");
	cin.clear();
	cin.sync();

	char commandLine[MAX_COMMAND_LENGTH + MAX_PATH_LENGTH * 2 + 2];
	char cmd[MAX_COMMAND_LENGTH], strArgv1[MAX_PATH_LENGTH], strArgv2[MAX_PATH_LENGTH];
	stringstream ss;
	while (1) {
		cout << '\n' << cur_dirName << '>';
		cin.getline(commandLine, MAX_COMMAND_LENGTH + MAX_PATH_LENGTH * 2 + 2);
		ss << commandLine;
		ss >> cmd >> strArgv1 >> strArgv2;
		mytolower(cmd);
		if (strlen(cmd)==0) {
			ss.clear();
			continue;
		}
		//sstream未读到最后，即参数过多
		if (!ss.eof()) {
			error(TOO_MANY_ARGS);
			ss.str("");//清空数据
			ss.clear();//重置sstream的标志状态
			continue;
		}
		//根据接收参数调用函数
		ss.clear();
		cmd_execute(cmd, strArgv1, strArgv2);
	}
}
