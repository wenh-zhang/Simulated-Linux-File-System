/*
command.cpp
系统命令的实现
包含cmd_execute/cmd_info/cmd_help
*/
#define _CRT_SECURE_NO_WARNINGS
#include"func.h"

//系统执行指令
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
		cout << "是否选择格式化磁盘 <Y/N> ? ";
		char init_choice = '\0';
		while (init_choice != 'y' && init_choice != 'Y' && init_choice != 'n' && init_choice != 'N') {
			init_choice = _getch();
		}
		cout << init_choice << endl;
		if (init_choice == 'y' || init_choice == 'Y')
		{
			cmd_init();
			cout << "磁盘格式化完成\n";
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
info:  显示整个系统信息(参考Linux文件系统的系统信息)，文件可以根据用户进行读写保护。
目录名和文件名支持全路径名和相对路径名，路径名各分量间用“/”隔开。
*/
void cmd_info() {
	cout << "\t模拟 Linux 文件系统信息：\n\n";
	cout << "整体信息：\n";
	cout.width(18);
	cout << "用户数量：";
	cout.width(10);
	cout << cur_Idsum;
	cout.width(3);
	cout << " 个\n";
	cout.width(18);
	cout << "最大用户数量：";
	cout.width(10);
	cout << MAX_USER_NUM;
	cout.width(3);
	cout << " 个\n";
	cout.width(18);
	cout << "磁盘容量：";
	cout.width(10);
	cout << BLOCKS_NUM * BLOCK_SIZE;
	cout.width(3);
	cout << " 字节\t";
	cout.width(10);
	cout << (float)BLOCKS_NUM / 1024;
	cout.width(3);
	cout << " MB\n";
	cout.width(18);
	cout << "已用空间：";
	cout.width(10);
	cout << (BLOCKS_NUM - sbGroups[0].allFreeBlocks) * BLOCK_SIZE;
	cout.width(3);
	cout << " 字节\t";
	cout.width(10);
	cout << (float)(BLOCKS_NUM - sbGroups[0].allFreeBlocks) / 1024;
	cout.width(3);
	cout << " MB\n";
	cout.width(18);
	cout << "可用空间：";
	cout.width(10);
	cout << sbGroups[0].allFreeBlocks * BLOCK_SIZE;
	cout.width(3);
	cout << " 字节\t";
	cout.width(10);
	cout << (float)sbGroups[0].allFreeBlocks / 1024;
	cout.width(3);
	cout << " MB\n";
	cout.width(18);
	cout << "可用空间比例：";
	cout.width(10);
	cout << ((float)sbGroups[0].allFreeBlocks / (float)BLOCKS_NUM) * 100
		<< "%\n";
	cout << "\n详细信息：\n";
	cout.width(18);
	cout << "盘块大小：";
	cout.width(10);
	cout << BLOCK_SIZE;
	cout.width(3);
	cout << " 字节\n";
	cout.width(18);
	cout << "每组盘块数：";
	cout.width(10);
	cout << BLOCKS_EACH;
	cout.width(3);
	cout << " 块\n";
	cout.width(18);
	cout << "每组i-结点数：";
	cout.width(10);
	cout << INODES_EACH;
	cout.width(3);
	cout << " 个\n";
	cout.width(18);
	cout << "盘块总数：";
	cout.width(10);
	cout << BLOCKS_NUM;
	cout.width(3);
	cout << " 块\n";
	cout.width(18);
	cout << "i-结点总数：";
	cout.width(10);
	cout << INODES_NUM;
	cout.width(3);
	cout << " 个\n";
	cout.width(18);
	cout << "空闲块总数：";
	cout.width(10);
	cout << sbGroups[0].allFreeBlocks;
	cout.width(3);
	cout << " 块\n";
	cout.width(18);
	cout << "空闲i-结点总数：";
	cout.width(10);
	cout << sbGroups[0].allFreeInodes;
	cout.width(3);
	cout << " 个\n";
}

void cmd_help(const char* strArg) {
	if (strArg[0] == '\0')
	{
		cout << "命令帮助信息：\n\n";
		cout << "info\t显示系统信息。\n";
		cout << "cd\t显示当前目录名或改变当前目录。\n";
		cout << "dir\t显示目录中的文件和子目录列表。\n";
		cout << "md\t创建目录。\n";
		cout << "rd\t删除目录。\n";
		cout << "newfile\t建立文件。\n";
		cout << "cat\t显示文本文件的内容。\n";
		cout << "copy\t将指定文件复制到另一个位置。\n";
		cout << "del\t删除指定文件。\n";
		cout << "check\t检测并恢复文件系统。\n";
		cout << "init\t初始化磁盘文件系统。\n";
		cout << "cls\t清除屏幕内容。\n";
		cout << "help\t提供命令的帮助信息。\n";
		cout << "logon\t重新登录。\n";
		cout << "useradd\t添加新用户。\n";
		cout << "userdel\t删除用户。\n";
		cout << "exit\t保存更改并退出程序。\n\n";
		cout << "\t有关某个命令的详细信息，请键入 help 命令名，或者 命令名 /?。\n";
		return;
	}
	int i = 0;
	for (i = 0; i < COMMAND_COUNT; i++)	// 比较命令
		if (strcmp(strCommands[i], strArg) == 0)	break;
	switch (i)
	{
	case 0:		//info
		cout << "info\n说明：显示系统信息。\n格式：info";
		break;
	case 1:		//cd
		cout << "cd\n说明：显示当前目录名或改变当前目录。\n格式：cd [路径]";
		cout << "\n其中，路径参数省略将显示当前路径。";
		break;
	case 2:		//dir
		cout << "dir\n说明：显示目录中的文件和子目录列表。\n格式：dir [路径]";
		break;
	case 3:		//md
		cout << "md\n说明：创建目录。\n格式：md 路径 [属性]";
		cout << "\n其中，属性值可以是以下任何值之一：\n";
		cout << "/a = 读写\t/r = 只读\n/w = 只写\t/s = 共享";
		cout << "\n属性参数省略将默认为“读写”属性。";
		break;
	case 4:		//rd
		cout << "rd\n说明：删除目录。\n格式：rd 路径";
		break;
	case 5:		//newfile
		cout << "newfile\n说明：建立文件。\n格式：newfile [路径]文件名 [属性]";
		cout << "\n其中，属性值可以是以下任何值之一：\n";
		cout << "/rw = 读写\t/r = 只读\n/w = 只写\t/s = 共享";
		cout << "\n属性参数省略将默认为“读写”属性。";
		break;
	case 6:		//cat
		cout << "cat\n说明：显示文本文件的内容。\n格式：cat [路径]文件名";
		break;
	case 7:		//copy
		cout << "copy\n说明：将指定文件复制到另一个位置。\n格式：copy [<host>][来源路径]文件名 [<host>]目标路径";
		cout << "\n其中，<host> 开关选项表示路径参数为宿主机路径；\n来源路径和目标路径参数中，至多只能有一个带有 <host>。";
		cout << "\n宿主机文件路径中不能带有空格。";
		break;
	case 8:		//del
		cout << "del\n说明：删除指定文件。\n格式：del [路径]文件名";
		break;
	case 9:	//check
		cout << "check\n说明：检测并恢复文件系统。\n格式：check";
		break;
	case 10:		//init
		cout << "init\n说明：初始化磁盘文件系统。\n格式：init";
		cout << "\n初始化后将生成 root/usr 系统目录，其下保存用户账户信息，不可删除。";
		cout << "\n* 需要管理员权限 *";
		break;
	case 11:	//cls
		cout << "cls\n说明：清除屏幕内容。\n格式：cls";
		break;
	case 12:	//help
		cout << "help\n说明：提供命令的帮助信息。\n格式：help [命令名] 或者 命令名 /?";
		cout << "\n其中，命令名参数省略则显示所有命令列表。";
		break;
	case 13:	//logon
		cout << "logon\n说明：重新登录。\n格式：logon";
		break;
	case 14:	//useradd
		cout << "useradd\n说明：添加新用户。\n格式：useradd 账号 [用户类型]";
		cout << "\n其中，账号必须为数字。\n用户类型可以为以下任何值之一：\n";
		cout << "/u = 普通用户\t/a = 管理员";
		cout << "\n用户类型参数省略将默认为普通用户。";
		cout << "\n* 需要管理员权限 *";
		break;
	case 15:	//userdel
		cout << "userdel\n说明：删除用户。\n格式：userdel 账号";
		cout << "\n其中，账号必须为数字。";
		cout << "\n* 需要管理员权限 *";
		break;
	case 16:	//exit
		cout << "exit\n说明：保存更改并退出程序。\n格式：exit";
		break;
	default:
		error(INVILID_CMD, strArg);
		return;
	}
	cout << "\n\n注意：\t格式中带 [] 的为可选项，路径省略默认为当前路径。\n";
	cout << "\t支持相对路径，“.”代表当前目录，“..”代表父目录。\n";
}

/*
check命令对应函数，检测并恢复文件系统，对文件系统中的数据一致性进行检测，并自动根据文件系统的结构和信息进行数据再整理
*/
void cmd_check()
{
	int i, j;
	int nStart;				//起始地址
	unsigned int nFreeBlockNum, nFreeInodeNum;			//空闲块和i-结点
	unsigned int nFreeBlockAll = 0, nFreeInodeAll = 0;	//所有空闲块和i-结点
	cout << "\n******************************\n";
	cout << "正在检查文件系统……\n";
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
	{
		nFreeBlockNum = 0;
		nFreeInodeNum = 0;
		nStart = i * BLOCKS_EACH;
		//计算空闲块和i-结点总和
		for (j = 0; j < BLOCKS_EACH; j++)
		{
			if (bsBlockBmp[nStart + j] == NOT_USED) nFreeBlockNum++;
			if (bsInodeBmp[nStart + j] == NOT_USED) nFreeInodeNum++;
		}
		//计算结果和磁盘记录不同，则发生了异常
		if (sbGroups[i].FreeBlocksNum != nFreeBlockNum)
		{
			sbGroups[i].FreeBlocksNum = nFreeBlockNum;
		}
		if (sbGroups[i].FreeInodesNum != nFreeInodeNum)
		{
			sbGroups[i].FreeInodesNum = nFreeInodeNum;
		}
		//加入总和
		nFreeBlockAll += sbGroups[i].FreeBlocksNum;
		nFreeInodeAll += sbGroups[i].FreeInodesNum;
	}
	//计算结果和磁盘记录不同，则发生了异常
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

	//保存改动

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
	cout << "文件系统修复完成！\n";

}

void cmd_cls() {
	system("cls");
}

//useradd命令对应函数，添加新用户
void cmd_useradd(const char* strUserId, user_type utType)
{
	if (cur_utType != SYSTEM && cur_utType != ADMIN) {
		error(NO_PRIVILEGE, "user");
		return;
	}
	if (cur_Idsum < MAX_USER_NUM)	//没有超过限制
	{
		//查找是否存在同名账号
		for (unsigned int i = 0; i < cur_Idsum ; i++)
		{
			if (strcmp(systemUsers[i].get_user_id(),strUserId)==0)
			{
				error(USER_EXIST, strUserId);
				return;
			}
		}
		//初始化
		char password[MAX_PWD_LENGTH];
		cout << "密码(最长20位)：";
		cin_pwd(password, MAX_PWD_LENGTH);

		stringstream ssStream;
		ssStream <<password <<" "<< (int)utType ;
		char strBuffer[MAX_PWD_LENGTH + 5];
		strcpy(strBuffer, ssStream.str().c_str());
		ssStream.clear();
		//格式化完成，写入文件
		Dir dirUsr;
		analyse_path("/user", dirUsr);
		char *tem = new char[MAX_NAME_LENGTH];
		strcpy(tem, strUserId);
		save_file(tem, strBuffer, MAX_PWD_LENGTH + 5,dirUsr, PROTECTED);
		//添加一个用户到内存
		cur_Idsum++;
		systemUsers[cur_Idsum - 1].set_user_info(strUserId, password,utType);
	}
	else
	{
		error(USERS_MAX);
	}
}

//userdel：删除用户
void cmd_userdel(const char* strUserId) {
	if (cur_utType != ADMIN && cur_utType != SYSTEM) {
		error(NO_PRIVILEGE_USERDEL);
		return;
	}
	if (strcmp(strUserId, cur_userId) == 0) {
		error(PROTECTED_USER);
		return;
	}
	//查找是否存在同名账号
	for (unsigned int i = 0; i < cur_Idsum; i++)
	{
		if (strcmp(systemUsers[i].get_user_id(), strUserId) == 0)
		{
			char* path = new char[MAX_NAME_LENGTH];
			strcpy(path, "root/user/");
			strcat(path, systemUsers[i].get_user_id());
			
			Dir dirTemp;
			char strDirName[MAX_NAME_LENGTH];
			if (analyse_path(path, dirTemp, strDirName)) {//路径合法
				//判断用户权限
				if (cur_utType != ADMIN && cur_utType != SYSTEM && indTable[dirTemp.num_Inode].fileAttr != SHARE &&
					strcmp(indTable[dirTemp.num_Inode].userId, cur_userId) != 0 )
				{
					error(NO_PRIVILEGE, path);
					cout << endl;
					error(DEL_FAILED);
					return;
				}
				unsigned int nIndex;//文件i节点号
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

			cout << "账号删除成功！" << endl;
			get_user_info();
			return;
		}
	}
	error(USER_NOT_EXIST, strUserId);
	return;

}

//退出程序
void cmd_exit()
{
	system("cls");
	exit(0);
}