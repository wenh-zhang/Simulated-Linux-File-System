/*
load.cpp
加载与初始化函数的定义
*/

#include"func.h"


void load() {
	fs.open("Virtual_Disk.bin", ios::in | ios::binary);//以读的方式打开二进制文件
	while (!fs.is_open()) {
		cout << "**********************磁盘初始化中**********************" << endl;
		fs.clear();
		cmd_init();
		cout << "**********************磁盘初始化完成**********************" << endl;
		system("cls");
		return;
	}
	//当二进制文件已存在时，从文件中读取信息到内存中
	for (unsigned int i = 0;i < BLOCK_GROUPS_NUM;i++)fs.read(reinterpret_cast<char*>(&sbGroups[i]), sizeof(SuperBlock));
	for (unsigned int i = 0;i < INODES_NUM;i++)fs.read(reinterpret_cast<char*>(&indTable[i]), sizeof(Inode));
	for (unsigned int i = 0;i < BLOCKS_NUM;i++)fs.read(reinterpret_cast<char*>(&bsBlockBmp[i]), sizeof(bmp_status));
	for (unsigned int i = 0;i < INODES_NUM;i++)fs.read(reinterpret_cast<char*>(&bsInodeBmp[i]), sizeof(bmp_status));
	fs.seekg(indTable[0].BlockAddr, ios::beg);
	fs.read(reinterpret_cast<char*>(&cur_dir), sizeof(Dir));
	fs.close();
	strcpy(cur_dirName, indTable[cur_dir.num_Inode].fileName);
	//获取用户信息
	get_user_info();
}

void cmd_init() {
	//超级块的初始化
	for (int i = 0;i < BLOCK_GROUPS_NUM;i++) {
		sbGroups[i].allFreeBlocks = BLOCKS_NUM - DIR_SIZE;
		sbGroups[i].allFreeInodes = BLOCKS_NUM - 1;
		sbGroups[i].BlockAddr = DATA_AREA_ADDR + i * BLOCK_SIZE * BLOCKS_EACH;
		sbGroups[i].FreeBlocksNum = BLOCKS_EACH;
		sbGroups[i].FreeInodesNum = INODES_EACH;
	}
	sbGroups[0].FreeBlocksNum = BLOCKS_EACH - DIR_SIZE;
	sbGroups[0].FreeInodesNum = INODES_EACH - 1;

	//i-节点表的初始化
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

	//数据块位图初始化
	for (int i = 0;i < BLOCKS_NUM;i++)bsBlockBmp[i] = NOT_USED;
	for (int i = 0;i < indTable[0].haveBlocks;i++)bsBlockBmp[i] = USED;
	//i-节点位图初始化
	for (int i = 0;i < INODES_NUM;i++)bsInodeBmp[i] = NOT_USED;
	bsInodeBmp[0] = USED;

	//本地数据初始化(当前路径名、当前目录）
	strcpy(cur_dirName, indTable[0].fileName);
	cur_dir.num_Inode = 0;
	cur_dir.num_child = 2;//分别是. ..
	cur_dir.num_SubInode[0] = 0;//.表示当前文件 ..表示父文件 二者均指向根目录
	cur_dir.num_SubInode[1] = 0;

	//将初始化后的数据写入二进制文件中
	fs.open("Virtual_Disk.bin", ios::out | ios::binary);
	if (!fs.is_open()) error(WRITE_FAILED);
	else {
		for (unsigned int i = 0;i < BLOCK_GROUPS_NUM;i++) { fs.write(reinterpret_cast<char*>(&sbGroups[i]), sizeof(SuperBlock));fs.flush(); }
		for (unsigned int i = 0;i < INODES_NUM;i++) { fs.write(reinterpret_cast<char*>(&indTable[i]), sizeof(Inode));fs.flush(); }
		for (unsigned int i = 0;i < BLOCKS_NUM;i++) { fs.write(reinterpret_cast<char*>(&bsBlockBmp[i]), sizeof(bmp_status));fs.flush(); }
		for (unsigned int i = 0;i < INODES_NUM;i++) { fs.write(reinterpret_cast<char*>(&bsInodeBmp[i]), sizeof(bmp_status));fs.flush(); }
		//往数据区中写入根目录信息
		fs.seekp(DATA_AREA_ADDR, ios::beg);
		fs.write(reinterpret_cast<char*>(&cur_dir), sizeof(Dir));
		fs.flush();
		//定义100M数据区
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
	cout << "正在初始化用户信息……\n";
	cout << "注册管理员账户中……\n";
	register_user();
	get_user_info();
	memset(cur_userId, 0, MAX_ID_LENGTH);
	strcpy(cur_userId, systemUsers[0].get_user_id());
	cur_utType = systemUsers[0].get_user_type();
	//初始化完成，返回
	cout << "\n******************************\n";
	cout << "初始化完成！正在转入登录界面……\n";
	Sleep(800);
	//system("cls");
}

void register_user() {
	cmd_md("user", READ_ONLY);
	cout << "账号(最长10位)：";
	char ID[MAX_ID_LENGTH];
	cin_id(ID, MAX_ID_LENGTH);
	cout << endl;
	cmd_useradd(ID, ADMIN);
}

//读取用户信息
void get_user_info()
{
	char strInfo[MAX_PWD_LENGTH + 5];
	char strPwd[MAX_PWD_LENGTH];
	char strUserType[4];
	stringstream ssStream;
	Dir dirUsr;
	analyse_path("/user", dirUsr);
	//打开 usr 目录，读取文件
	for (unsigned int i = 2; i < dirUsr.num_child; i++)
	{
		//遍历文件，读取帐号信息
		if (indTable[dirUsr.num_SubInode[i]].fileType == TYPE_FILE)
		{
			//读取用户的资料
			open_file(dirUsr.num_SubInode[i], strInfo);
			ssStream << strInfo;
			ssStream >> strPwd >> strUserType;
			int nUserType = atoi(strUserType);
			//写入内存用户类
			systemUsers[i - 2].set_user_info(indTable[dirUsr.num_SubInode[i]].fileName, strPwd, user_type(nUserType));
			ssStream.clear();
		}
	}
	cur_Idsum = dirUsr.num_child - 2;
}