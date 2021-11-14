/*
load.cpp
�������ʼ�������Ķ���
*/

#include"func.h"


void load() {
	fs.open("Virtual_Disk.bin", ios::in | ios::binary);//�Զ��ķ�ʽ�򿪶������ļ�
	while (!fs.is_open()) {
		cout << "**********************���̳�ʼ����**********************" << endl;
		fs.clear();
		cmd_init();
		cout << "**********************���̳�ʼ�����**********************" << endl;
		system("cls");
		return;
	}
	//���������ļ��Ѵ���ʱ�����ļ��ж�ȡ��Ϣ���ڴ���
	for (unsigned int i = 0;i < BLOCK_GROUPS_NUM;i++)fs.read(reinterpret_cast<char*>(&sbGroups[i]), sizeof(SuperBlock));
	for (unsigned int i = 0;i < INODES_NUM;i++)fs.read(reinterpret_cast<char*>(&indTable[i]), sizeof(Inode));
	for (unsigned int i = 0;i < BLOCKS_NUM;i++)fs.read(reinterpret_cast<char*>(&bsBlockBmp[i]), sizeof(bmp_status));
	for (unsigned int i = 0;i < INODES_NUM;i++)fs.read(reinterpret_cast<char*>(&bsInodeBmp[i]), sizeof(bmp_status));
	fs.seekg(indTable[0].BlockAddr, ios::beg);
	fs.read(reinterpret_cast<char*>(&cur_dir), sizeof(Dir));
	fs.close();
	strcpy(cur_dirName, indTable[cur_dir.num_Inode].fileName);
	//��ȡ�û���Ϣ
	get_user_info();
}

void cmd_init() {
	//������ĳ�ʼ��
	for (int i = 0;i < BLOCK_GROUPS_NUM;i++) {
		sbGroups[i].allFreeBlocks = BLOCKS_NUM - DIR_SIZE;
		sbGroups[i].allFreeInodes = BLOCKS_NUM - 1;
		sbGroups[i].BlockAddr = DATA_AREA_ADDR + i * BLOCK_SIZE * BLOCKS_EACH;
		sbGroups[i].FreeBlocksNum = BLOCKS_EACH;
		sbGroups[i].FreeInodesNum = INODES_EACH;
	}
	sbGroups[0].FreeBlocksNum = BLOCKS_EACH - DIR_SIZE;
	sbGroups[0].FreeInodesNum = INODES_EACH - 1;

	//i-�ڵ��ĳ�ʼ��
	for (int i = 0;i < INODES_NUM;i++) {
		indTable[i].fileAttr = READ_WRITE;
		indTable[i].fileType = TYPE_DIR;
		strcpy(indTable[i].userId, "000000");
		indTable[i].fileSize = 0;
		indTable[i].BlockAddr = 0;
		indTable[i].haveBlocks = 0;
	}
	strcpy(indTable[0].fileName, "root");
	indTable[0].fileAttr = PROTECTED;
	indTable[0].fileType = TYPE_DIR;
	strcpy(indTable[0].userId, "000000");
	time_t cur_time;
	time(&cur_time);
	indTable[0].fileMfyTime.set_time(*localtime(&cur_time));
	indTable[0].fileSize = sizeof(Dir);
	indTable[0].BlockAddr = DATA_AREA_ADDR;
	indTable[0].haveBlocks = DIR_SIZE;

	//���ݿ�λͼ��ʼ��
	for (int i = 0;i < BLOCKS_NUM;i++)bsBlockBmp[i] = NOT_USED;
	for (int i = 0;i < indTable[0].haveBlocks;i++)bsBlockBmp[i] = USED;
	//i-�ڵ�λͼ��ʼ��
	for (int i = 0;i < INODES_NUM;i++)bsInodeBmp[i] = NOT_USED;
	bsInodeBmp[0] = USED;

	//�������ݳ�ʼ��(��ǰ·��������ǰĿ¼��
	strcpy(cur_dirName, indTable[0].fileName);
	cur_dir.num_Inode = 0;
	cur_dir.num_child = 2;//�ֱ���. ..
	cur_dir.num_SubInode[0] = 0;//.��ʾ��ǰ�ļ� ..��ʾ���ļ� ���߾�ָ���Ŀ¼
	cur_dir.num_SubInode[1] = 0;

	//����ʼ���������д��������ļ���
	fs.open("Virtual_Disk.bin", ios::out | ios::binary);
	if (!fs.is_open()) error(WRITE_FAILED);
	else {
		for (unsigned int i = 0;i < BLOCK_GROUPS_NUM;i++) { fs.write(reinterpret_cast<char*>(&sbGroups[i]), sizeof(SuperBlock));fs.flush(); }
		for (unsigned int i = 0;i < INODES_NUM;i++) { fs.write(reinterpret_cast<char*>(&indTable[i]), sizeof(Inode));fs.flush(); }
		for (unsigned int i = 0;i < BLOCKS_NUM;i++) { fs.write(reinterpret_cast<char*>(&bsBlockBmp[i]), sizeof(bmp_status));fs.flush(); }
		for (unsigned int i = 0;i < INODES_NUM;i++) { fs.write(reinterpret_cast<char*>(&bsInodeBmp[i]), sizeof(bmp_status));fs.flush(); }
		//����������д���Ŀ¼��Ϣ
		fs.seekp(DATA_AREA_ADDR, ios::beg);
		fs.write(reinterpret_cast<char*>(&cur_dir), sizeof(Dir));
		fs.flush();
		//����100M������
		char* data_area = new char[BLOCKS_NUM * BLOCK_SIZE];
		fs.write(reinterpret_cast<char*>(data_area), BLOCKS_NUM * BLOCK_SIZE * sizeof(char));
		fs.flush();
		delete[]data_area;
		//cmd_md("user", READ_ONLY);
		fs.close();
	}
	for (int i = 0; i < MAX_USER_NUM; i++)
		systemUsers[i].set_user_info("000000", "", SYSTEM);
	cout << "\n\n******************************\n";
	cout << "���ڳ�ʼ���û���Ϣ����\n";
	cout << "ע�����Ա�˻��С���\n";
	register_user();
	get_user_info();
	memset(cur_userId, 0, MAX_ID_LENGTH);
	strcpy(cur_userId, systemUsers[0].get_user_id());
	cur_utType = systemUsers[0].get_user_type();
	//��ʼ����ɣ�����
	cout << "\n******************************\n";
	cout << "��ʼ����ɣ�����ת���¼���桭��\n";
	Sleep(800);
	//system("cls");
}

void register_user() {
	cmd_md("user", READ_ONLY);
	cout << "�˺�(�10λ)��";
	char ID[MAX_ID_LENGTH];
	cin_id(ID, MAX_ID_LENGTH);
	cout << endl;
	cmd_useradd(ID, ADMIN);
}

//��ȡ�û���Ϣ
void get_user_info()
{
	char strInfo[MAX_PWD_LENGTH + 5];
	char strPwd[MAX_PWD_LENGTH];
	char strUserType[4];
	stringstream ssStream;
	Dir dirUsr;
	analyse_path("/user", dirUsr);
	//�� usr Ŀ¼����ȡ�ļ�
	for (unsigned int i = 2; i < dirUsr.num_child; i++)
	{
		//�����ļ�����ȡ�ʺ���Ϣ
		if (indTable[dirUsr.num_SubInode[i]].fileType == TYPE_FILE)
		{
			//��ȡ�û�������
			open_file(dirUsr.num_SubInode[i], strInfo);
			ssStream << strInfo;
			ssStream >> strPwd >> strUserType;
			int nUserType = atoi(strUserType);
			//д���ڴ��û���
			systemUsers[i - 2].set_user_info(indTable[dirUsr.num_SubInode[i]].fileName, strPwd, user_type(nUserType));
			ssStream.clear();
		}
	}
	cur_Idsum = dirUsr.num_child - 2;
}