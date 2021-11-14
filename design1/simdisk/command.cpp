/*
command.cpp
ϵͳ�����ʵ��
����cmd_execute/cmd_info/cmd_help
*/
#define _CRT_SECURE_NO_WARNINGS
#include"func.h"

//ϵͳִ��ָ��
void cmd_execute(const char* commondName, const char* strArgv1, const char* strArgv2) {
	if (strcmp(strArgv1, "/?") == 0) {
		if (strArgv2[0] != '\0') {
			error(TOO_MANY_ARGS);
		}
		else {
			cmd_help(commondName);
		}
		return;
	}
	int unsigned i;
	for (i = 0;i < COMMAND_COUNT;i++) {
		if (strcmp(commondName, strCommands[i]) == 0)
			break;
	}

	switch (i)
	{
	case 0://info
		if (strArgv1[0] != '\0')
			error(TOO_MANY_ARGS);
		else
			cmd_info();
		break;
	case 1://cd
		if (strArgv2[0] != '\0')
			error(TOO_MANY_ARGS);
		else
			cmd_cd(strArgv1);
		break;
	case 2://dir
		if (strArgv2[0] != '\0')
			error(TOO_MANY_ARGS);
		else
			cmd_dir(strArgv1);
		break;
	case 3://md
		if (strArgv2[0] == '\0')
			if (strArgv1[0] == '\0')
				error(WRONG_ARGS);
			else cmd_md(strArgv1, READ_WRITE);
		else {
			file_attrib privilege;
			if (strcmp(strArgv2, "/rw") == 0) privilege = READ_WRITE;
			else if (strcmp(strArgv2, "/r") == 0)privilege = READ_ONLY;
			else if (strcmp(strArgv2, "/w") == 0)privilege = WRITE_ONLY;
			else if (strcmp(strArgv2, "/s") == 0)privilege = SHARE;
			else { error(WRONG_ARGS);return; }
			cmd_md(strArgv1, privilege);
		}
		break;
	case 4:	//rd
		if (strArgv1[0] == '\0' || strArgv2[0] != '\0')
		{
			error(WRONG_ARGS, commondName);
			return;
		}
		cmd_rd(strArgv1);
		break;
	case 5://newfile
		if (strArgv2[0] == '\0')
			if (strArgv1[0] == '\0')
				error(WRONG_ARGS);
			else cmd_newfile(strArgv1, READ_WRITE);
		else {
			file_attrib privilege;
			if (strcmp(strArgv2, "/rw") == 0) privilege = READ_WRITE;
			else if (strcmp(strArgv2, "/r") == 0)privilege = READ_ONLY;
			else if (strcmp(strArgv2, "/w") == 0)privilege = WRITE_ONLY;
			else if (strcmp(strArgv2, "/s") == 0)privilege = SHARE;
			else { error(WRONG_ARGS);return; }
			cmd_newfile(strArgv1, privilege);
		}
		break;
	case 6:	//cat
		if (strArgv1[0] == '\0' || strArgv2[0] != '\0')
		{
			error(WRONG_ARGS, commondName);
			return;
		}
		cmd_cat(strArgv1);
		break;
	case 7:	//copy
		if (strArgv2[0] == '\0')
		{
			error(WRONG_ARGS, commondName);
			return;
		}
		cmd_copy(strArgv1, strArgv2);
		break;
	case 8:	//del
		if (strArgv1[0] == '\0' || strArgv2[0] != '\0')
		{
			error(WRONG_ARGS, commondName);
			return;
		}
		cmd_del(strArgv1);
		break;
	case 9://check
		if (strArgv1[0] != '\0') {
			error(WRONG_ARGS, commondName);
			return;
		}
		cmd_check();
		break;
	case 10://init
	{
		if (strArgv1[0] != '\0') {
			error(WRONG_ARGS, commondName);
			return;
		}
		cout << "�Ƿ�ѡ���ʽ������ <Y/N> ? ";
		char init_choice = '\0';
		while (init_choice != 'y' && init_choice != 'Y' && init_choice != 'n' && init_choice != 'N') {
			init_choice = _getch();
		}
		cout << init_choice << endl;
		if (init_choice == 'y' || init_choice == 'Y')
		{
			cmd_init();
			cout << "���̸�ʽ�����\n";
			Sleep(3000);
		}
		break;
	}
	case 11://cls
		if (strArgv1[0] != '\0') {
			error(WRONG_ARGS, commondName);
			return;
		}
		system("cls");
		break;
	case 12://help
		if (strArgv2[0] != '\0') {
			error(WRONG_ARGS, commondName);
			return;
		}
		cmd_help(strArgv1);
		break;
	case 13://logon
		if (strArgv1[0] != '\0')
		{
			error(WRONG_ARGS, commondName);
			return;
		}
		system("cls");
		system("title SimDisk");
		if (!cmd_logon()) exit(0);
		cin.sync();
		system("cls");
		break;
	case 14://useradd
		if (strArgv1[0] == '\0') {
			error(WRONG_ARGS, commondName);
			return;
		}
		if (strArgv2[0] == '\0')
			cmd_useradd(strArgv1, USER);
		else {
			if (strcmp(strArgv2, "/u") == 0)
				cmd_useradd(strArgv1, USER);
			else if (strcmp(strArgv2, "/a") == 0)
				cmd_useradd(strArgv1, ADMIN);
			else error(WRONG_ARGS, commondName);
		}
		break;
	case 15://userdel
		if (strArgv1[0] == '\0'||strArgv2[0]!='\0') {
			error(WRONG_ARGS, commondName);
			return;
		}
		cmd_userdel(strArgv1);
		break;
	case 16://exit
		if (strArgv1[0] != '\0') {
			error(WRONG_ARGS, commondName);
			return;
		}
		cmd_exit();
		break;
	default:
		error(INVILID_CMD);
		break;
	}
}

/*
info:  ��ʾ����ϵͳ��Ϣ(�ο�Linux�ļ�ϵͳ��ϵͳ��Ϣ)���ļ����Ը����û����ж�д������
Ŀ¼�����ļ���֧��ȫ·���������·������·�������������á�/��������
*/
void cmd_info() {
	cout << "\tģ�� Linux �ļ�ϵͳ��Ϣ��\n\n";
	cout << "������Ϣ��\n";
	cout.width(18);
	cout << "�û�������";
	cout.width(10);
	cout << cur_Idsum;
	cout.width(3);
	cout << " ��\n";
	cout.width(18);
	cout << "����û�������";
	cout.width(10);
	cout << MAX_USER_NUM;
	cout.width(3);
	cout << " ��\n";
	cout.width(18);
	cout << "����������";
	cout.width(10);
	cout << BLOCKS_NUM * BLOCK_SIZE;
	cout.width(3);
	cout << " �ֽ�\t";
	cout.width(10);
	cout << (float)BLOCKS_NUM / 1024;
	cout.width(3);
	cout << " MB\n";
	cout.width(18);
	cout << "���ÿռ䣺";
	cout.width(10);
	cout << (BLOCKS_NUM - sbGroups[0].allFreeBlocks) * BLOCK_SIZE;
	cout.width(3);
	cout << " �ֽ�\t";
	cout.width(10);
	cout << (float)(BLOCKS_NUM - sbGroups[0].allFreeBlocks) / 1024;
	cout.width(3);
	cout << " MB\n";
	cout.width(18);
	cout << "���ÿռ䣺";
	cout.width(10);
	cout << sbGroups[0].allFreeBlocks * BLOCK_SIZE;
	cout.width(3);
	cout << " �ֽ�\t";
	cout.width(10);
	cout << (float)sbGroups[0].allFreeBlocks / 1024;
	cout.width(3);
	cout << " MB\n";
	cout.width(18);
	cout << "���ÿռ������";
	cout.width(10);
	cout << ((float)sbGroups[0].allFreeBlocks / (float)BLOCKS_NUM) * 100
		<< "%\n";
	cout << "\n��ϸ��Ϣ��\n";
	cout.width(18);
	cout << "�̿��С��";
	cout.width(10);
	cout << BLOCK_SIZE;
	cout.width(3);
	cout << " �ֽ�\n";
	cout.width(18);
	cout << "ÿ���̿�����";
	cout.width(10);
	cout << BLOCKS_EACH;
	cout.width(3);
	cout << " ��\n";
	cout.width(18);
	cout << "ÿ��i-�������";
	cout.width(10);
	cout << INODES_EACH;
	cout.width(3);
	cout << " ��\n";
	cout.width(18);
	cout << "�̿�������";
	cout.width(10);
	cout << BLOCKS_NUM;
	cout.width(3);
	cout << " ��\n";
	cout.width(18);
	cout << "i-���������";
	cout.width(10);
	cout << INODES_NUM;
	cout.width(3);
	cout << " ��\n";
	cout.width(18);
	cout << "���п�������";
	cout.width(10);
	cout << sbGroups[0].allFreeBlocks;
	cout.width(3);
	cout << " ��\n";
	cout.width(18);
	cout << "����i-���������";
	cout.width(10);
	cout << sbGroups[0].allFreeInodes;
	cout.width(3);
	cout << " ��\n";
}

void cmd_help(const char* strArg) {
	if (strArg[0] == '\0')
	{
		cout << "���������Ϣ��\n\n";
		cout << "info\t��ʾϵͳ��Ϣ��\n";
		cout << "cd\t��ʾ��ǰĿ¼����ı䵱ǰĿ¼��\n";
		cout << "dir\t��ʾĿ¼�е��ļ�����Ŀ¼�б�\n";
		cout << "md\t����Ŀ¼��\n";
		cout << "rd\tɾ��Ŀ¼��\n";
		cout << "newfile\t�����ļ���\n";
		cout << "cat\t��ʾ�ı��ļ������ݡ�\n";
		cout << "copy\t��ָ���ļ����Ƶ���һ��λ�á�\n";
		cout << "del\tɾ��ָ���ļ���\n";
		cout << "check\t��Ⲣ�ָ��ļ�ϵͳ��\n";
		cout << "init\t��ʼ�������ļ�ϵͳ��\n";
		cout << "cls\t�����Ļ���ݡ�\n";
		cout << "help\t�ṩ����İ�����Ϣ��\n";
		cout << "logon\t���µ�¼��\n";
		cout << "useradd\t������û���\n";
		cout << "userdel\tɾ���û���\n";
		cout << "exit\t������Ĳ��˳�����\n\n";
		cout << "\t�й�ĳ���������ϸ��Ϣ������� help ������������ ������ /?��\n";
		return;
	}
	int i = 0;
	for (i = 0; i < COMMAND_COUNT; i++)	// �Ƚ�����
		if (strcmp(strCommands[i], strArg) == 0)	break;
	switch (i)
	{
	case 0:		//info
		cout << "info\n˵������ʾϵͳ��Ϣ��\n��ʽ��info";
		break;
	case 1:		//cd
		cout << "cd\n˵������ʾ��ǰĿ¼����ı䵱ǰĿ¼��\n��ʽ��cd [·��]";
		cout << "\n���У�·������ʡ�Խ���ʾ��ǰ·����";
		break;
	case 2:		//dir
		cout << "dir\n˵������ʾĿ¼�е��ļ�����Ŀ¼�б�\n��ʽ��dir [·��]";
		break;
	case 3:		//md
		cout << "md\n˵��������Ŀ¼��\n��ʽ��md ·�� [����]";
		cout << "\n���У�����ֵ�����������κ�ֵ֮һ��\n";
		cout << "/a = ��д\t/r = ֻ��\n/w = ֻд\t/s = ����";
		cout << "\n���Բ���ʡ�Խ�Ĭ��Ϊ����д�����ԡ�";
		break;
	case 4:		//rd
		cout << "rd\n˵����ɾ��Ŀ¼��\n��ʽ��rd ·��";
		break;
	case 5:		//newfile
		cout << "newfile\n˵���������ļ���\n��ʽ��newfile [·��]�ļ��� [����]";
		cout << "\n���У�����ֵ�����������κ�ֵ֮һ��\n";
		cout << "/rw = ��д\t/r = ֻ��\n/w = ֻд\t/s = ����";
		cout << "\n���Բ���ʡ�Խ�Ĭ��Ϊ����д�����ԡ�";
		break;
	case 6:		//cat
		cout << "cat\n˵������ʾ�ı��ļ������ݡ�\n��ʽ��cat [·��]�ļ���";
		break;
	case 7:		//copy
		cout << "copy\n˵������ָ���ļ����Ƶ���һ��λ�á�\n��ʽ��copy [<host>][��Դ·��]�ļ��� [<host>]Ŀ��·��";
		cout << "\n���У�<host> ����ѡ���ʾ·������Ϊ������·����\n��Դ·����Ŀ��·�������У�����ֻ����һ������ <host>��";
		cout << "\n�������ļ�·���в��ܴ��пո�";
		break;
	case 8:		//del
		cout << "del\n˵����ɾ��ָ���ļ���\n��ʽ��del [·��]�ļ���";
		break;
	case 9:	//check
		cout << "check\n˵������Ⲣ�ָ��ļ�ϵͳ��\n��ʽ��check";
		break;
	case 10:		//init
		cout << "init\n˵������ʼ�������ļ�ϵͳ��\n��ʽ��init";
		cout << "\n��ʼ�������� root/usr ϵͳĿ¼�����±����û��˻���Ϣ������ɾ����";
		cout << "\n* ��Ҫ����ԱȨ�� *";
		break;
	case 11:	//cls
		cout << "cls\n˵���������Ļ���ݡ�\n��ʽ��cls";
		break;
	case 12:	//help
		cout << "help\n˵�����ṩ����İ�����Ϣ��\n��ʽ��help [������] ���� ������ /?";
		cout << "\n���У�����������ʡ������ʾ���������б�";
		break;
	case 13:	//logon
		cout << "logon\n˵�������µ�¼��\n��ʽ��logon";
		break;
	case 14:	//useradd
		cout << "useradd\n˵����������û���\n��ʽ��useradd �˺� [�û�����]";
		cout << "\n���У��˺ű���Ϊ���֡�\n�û����Ϳ���Ϊ�����κ�ֵ֮һ��\n";
		cout << "/u = ��ͨ�û�\t/a = ����Ա";
		cout << "\n�û����Ͳ���ʡ�Խ�Ĭ��Ϊ��ͨ�û���";
		cout << "\n* ��Ҫ����ԱȨ�� *";
		break;
	case 15:	//userdel
		cout << "userdel\n˵����ɾ���û���\n��ʽ��userdel �˺�";
		cout << "\n���У��˺ű���Ϊ���֡�";
		cout << "\n* ��Ҫ����ԱȨ�� *";
		break;
	case 16:	//exit
		cout << "exit\n˵����������Ĳ��˳�����\n��ʽ��exit";
		break;
	default:
		error(INVILID_CMD, strArg);
		return;
	}
	cout << "\n\nע�⣺\t��ʽ�д� [] ��Ϊ��ѡ�·��ʡ��Ĭ��Ϊ��ǰ·����\n";
	cout << "\t֧�����·������.������ǰĿ¼����..������Ŀ¼��\n";
}

/*
check�����Ӧ��������Ⲣ�ָ��ļ�ϵͳ�����ļ�ϵͳ�е�����һ���Խ��м�⣬���Զ������ļ�ϵͳ�Ľṹ����Ϣ��������������
*/
void cmd_check()
{
	int i, j;
	int nStart;				//��ʼ��ַ
	unsigned int nFreeBlockNum, nFreeInodeNum;			//���п��i-���
	unsigned int nFreeBlockAll = 0, nFreeInodeAll = 0;	//���п��п��i-���
	cout << "\n******************************\n";
	cout << "���ڼ���ļ�ϵͳ����\n";
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
	{
		nFreeBlockNum = 0;
		nFreeInodeNum = 0;
		nStart = i * BLOCKS_EACH;
		//������п��i-����ܺ�
		for (j = 0; j < BLOCKS_EACH; j++)
		{
			if (bsBlockBmp[nStart + j] == NOT_USED) nFreeBlockNum++;
			if (bsInodeBmp[nStart + j] == NOT_USED) nFreeInodeNum++;
		}
		//�������ʹ��̼�¼��ͬ���������쳣
		if (sbGroups[i].FreeBlocksNum != nFreeBlockNum)
		{
			sbGroups[i].FreeBlocksNum = nFreeBlockNum;
		}
		if (sbGroups[i].FreeInodesNum != nFreeInodeNum)
		{
			sbGroups[i].FreeInodesNum = nFreeInodeNum;
		}
		//�����ܺ�
		nFreeBlockAll += sbGroups[i].FreeBlocksNum;
		nFreeInodeAll += sbGroups[i].FreeInodesNum;
	}
	//�������ʹ��̼�¼��ͬ���������쳣
	if (sbGroups[0].allFreeBlocks != nFreeBlockAll)
	{
		for (i = 0; i < BLOCK_GROUPS_NUM; i++)
			sbGroups[i].allFreeBlocks = nFreeBlockAll;
	}
	if (sbGroups[0].allFreeInodes != nFreeInodeAll)
	{
		for (i = 0; i < BLOCK_GROUPS_NUM; i++)
			sbGroups[i].allFreeInodes = nFreeInodeAll;
	}

	//����Ķ�

	fs.open("Virtual_Disk.bin", ios::out | ios::binary | ios::_Nocreate);
	if (!fs.is_open()) error(WRITE_FAILED);
	for (int i = 0; i < BLOCK_GROUPS_NUM; i++)
		fs.write(reinterpret_cast<char*>(&sbGroups[i]), sizeof(SuperBlock));
	for (int i = 0; i < INODES_NUM; i++)
		fs.write(reinterpret_cast<char*>(&indTable[i]), sizeof(Inode));
	for (int i = 0; i < BLOCKS_NUM; i++)
		fs.write(reinterpret_cast<char*>(&bsBlockBmp[i]), sizeof(bmp_status));
	for (int i = 0; i < INODES_NUM; i++)
		fs.write(reinterpret_cast<char*>(&bsInodeBmp[i]), sizeof(bmp_status));


	fs.seekp(indTable[cur_dir.num_Inode].BlockAddr, ios::beg);
	fs.write(reinterpret_cast<char*>(&cur_dir), sizeof(Dir));
	fs.close();
	cout << "\n******************************\n";
	cout << "�ļ�ϵͳ�޸���ɣ�\n";

}

void cmd_cls() {
	system("cls");
}

//useradd�����Ӧ������������û�
void cmd_useradd(const char* strUserId, user_type utType)
{
	if (cur_utType != SYSTEM && cur_utType != ADMIN) {
		error(NO_PRIVILEGE, "user");
		return;
	}
	if (cur_Idsum < MAX_USER_NUM)	//û�г�������
	{
		//�����Ƿ����ͬ���˺�
		for (unsigned int i = 0; i < cur_Idsum ; i++)
		{
			if (strcmp(systemUsers[i].get_user_id(),strUserId)==0)
			{
				error(USER_EXIST, strUserId);
				return;
			}
		}
		//��ʼ��
		char password[MAX_PWD_LENGTH];
		cout << "����(�20λ)��";
		cin_pwd(password, MAX_PWD_LENGTH);

		stringstream ssStream;
		ssStream <<password <<" "<< (int)utType ;
		char strBuffer[MAX_PWD_LENGTH + 5];
		strcpy(strBuffer, ssStream.str().c_str());
		ssStream.clear();
		//��ʽ����ɣ�д���ļ�
		Dir dirUsr;
		analyse_path("/user", dirUsr);
		char *tem = new char[MAX_NAME_LENGTH];
		strcpy(tem, strUserId);
		save_file(tem, strBuffer, MAX_PWD_LENGTH + 5,dirUsr, PROTECTED);
		//���һ���û����ڴ�
		cur_Idsum++;
		systemUsers[cur_Idsum - 1].set_user_info(strUserId, password,utType);
	}
	else
	{
		error(USERS_MAX);
	}
}

//userdel��ɾ���û�
void cmd_userdel(const char* strUserId) {
	if (cur_utType != ADMIN && cur_utType != SYSTEM) {
		error(NO_PRIVILEGE_USERDEL);
		return;
	}
	if (strcmp(strUserId, cur_userId) == 0) {
		error(PROTECTED_USER);
		return;
	}
	//�����Ƿ����ͬ���˺�
	for (unsigned int i = 0; i < cur_Idsum; i++)
	{
		if (strcmp(systemUsers[i].get_user_id(), strUserId) == 0)
		{
			char* path = new char[MAX_NAME_LENGTH];
			strcpy(path, "root/user/");
			strcat(path, systemUsers[i].get_user_id());
			
			Dir dirTemp;
			char strDirName[MAX_NAME_LENGTH];
			if (analyse_path(path, dirTemp, strDirName)) {//·���Ϸ�
				//�ж��û�Ȩ��
				if (cur_utType != ADMIN && cur_utType != SYSTEM && indTable[dirTemp.num_Inode].fileAttr != SHARE &&
					strcmp(indTable[dirTemp.num_Inode].userId, cur_userId) != 0 )
				{
					error(NO_PRIVILEGE, path);
					cout << endl;
					error(DEL_FAILED);
					return;
				}
				unsigned int nIndex;//�ļ�i�ڵ��
				if (!dirTemp.have_child(strDirName, nIndex, TYPE_FILE))
				{
					error(FILE_NOT_EXIST, indTable[dirTemp.num_Inode].fileName, strDirName);
					cout << endl;
					error(CAT_FAILED);
					return;
				}
				else {
					if (cur_utType != ADMIN && cur_utType != SYSTEM && indTable[nIndex].fileAttr != SHARE &&
						strcmp(indTable[nIndex].userId, cur_userId) != 0)
					{
						error(NO_PRIVILEGE, path);
						cout << endl;
						error(DEL_FAILED);
						return;
					}
					delete_file(dirTemp, nIndex);
				}
			}
			else {
				error(PATH_NOT_FOUND);
				cout << endl;
				error(DEL_FAILED);
				return;
			}

			cout << "�˺�ɾ���ɹ���" << endl;
			get_user_info();
			return;
		}
	}
	error(USER_NOT_EXIST, strUserId);
	return;

}

//�˳�����
void cmd_exit()
{
	system("cls");
	exit(0);
}