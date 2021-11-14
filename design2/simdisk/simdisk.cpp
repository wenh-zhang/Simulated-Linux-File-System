/*
simdisk.cpp
ִ�г���
*/
#include"simdisk.h"
#include"func.h"
#include<sstream>

#define BUF_SIZE 1024*1024*100+1
char input[] = "input";    // �����ڴ������
char output[] = "output";    // �����ڴ������


char cur_userId[MAX_ID_LENGTH] = "000000";		//��ǰ�û��˺�ID
user_type cur_utType = SYSTEM;					//��ǰ�û�����
unsigned int cur_Idsum = 0;						//��ǰ�û�����

char cur_dirName[MAX_NAME_LENGTH];				//��ǰĿ¼����
Dir cur_dir;									//��ǰĿ¼

fstream fs;									//����Linux�����ļ�
User systemUsers[MAX_USER_NUM];					//ϵͳ�û�����

SuperBlock sbGroups[BLOCK_GROUPS_NUM];			//���ݿ��飨�������飩
Inode indTable[INODES_NUM];						//i-����
bmp_status bsBlockBmp[BLOCKS_NUM];				//���ݿ�λͼ����
bmp_status bsInodeBmp[INODES_NUM];				//i-���λͼ����

//������
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
		//sstreamδ������󣬼���������
		if (!ss.eof()) {
			error(TOO_MANY_ARGS);
			ss.str("");//�������
			ss.clear();//����sstream�ı�־״̬
			continue;
		}
		//���ݽ��ղ������ú���
		ss.clear();
		cmd_execute(cmd, strArgv1, strArgv2);
	}
}
