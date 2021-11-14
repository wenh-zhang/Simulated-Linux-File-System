/*
dir.cpp
与目录、文件相关的操作指令定义
*/
#include"func.h"



/*
cd …:  改变目录：改变当前工作目录，目录不存在时给出出错信息。
*/
void cmd_cd(const char* strArgv) {
	if (strArgv[0] == '\0') {
		return;
	}
	Dir dirTemp;
	if (analyse_path(strArgv, dirTemp)) {//路径合法
		//判断用户权限
		if (cur_utType != ADMIN && cur_utType != SYSTEM && strcmp(indTable[dirTemp.num_Inode].fileName, "root") != 0 &&
			indTable[dirTemp.num_Inode].fileAttr != SHARE &&
			strcmp(indTable[dirTemp.num_Inode].userId, cur_userId) != 0) {
			error(NO_PRIVILEGE);
			cout << endl;
			error(CD_FAILED);
			return;
		}
		if (indTable[dirTemp.num_Inode].fileAttr == WRITE_ONLY)
		{
			error(DIR_WRITEONLY, indTable[dirTemp.num_Inode].fileName);
			cout << endl;
			error(CD_FAILED);
			return;
		}
		cur_dir = dirTemp;
		set_cur_path(cur_dir);
	}
	else {
		error(PATH_NOT_FOUND);
		cout << endl;
		error(CD_FAILED);
		return;
	}
}


/*
dir …:  显示目录：显示指定目录下或当前目录下的信息，
包括文件名、物理地址、保护码、文件长度、子目录等
（带/s参数的dir命令，显示所有子目录）。
*/
void cmd_dir(const char* strPath) {

	Dir dirTemp;
	if (strPath[0] == '\0') {
		dirTemp = cur_dir;
	}
	else if (analyse_path(strPath, dirTemp)) {//路径合法
		//判断用户权限
		if (cur_utType != ADMIN && cur_utType != SYSTEM && strcmp(indTable[dirTemp.num_Inode].fileName, "root") != 0 &&
			indTable[dirTemp.num_Inode].fileAttr != SHARE &&
			strcmp(indTable[dirTemp.num_Inode].userId, cur_userId) != 0) {
			error(NO_PRIVILEGE);
			cout << endl;
			error(DIR_FAILED);
			return;
		}
		if (indTable[dirTemp.num_Inode].fileAttr == WRITE_ONLY)
		{
			error(DIR_WRITEONLY, indTable[dirTemp.num_Inode].fileName);
			cout << endl;
			error(DIR_FAILED);
			return;
		}

	}
	else {
		error(PATH_NOT_FOUND);
		cout << endl;
		error(DIR_FAILED);
		return;
	}
	//打印目录下信息
	dir_info(dirTemp.num_Inode);
}

//显示目录下的相关信息
void dir_info(unsigned int InodeNum) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
	cout << indTable[InodeNum].fileName << " 的目录\n\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	cout.width(20);
	cout << left << "目录名/文件名";
	cout.width(20);
	cout << left << "修改日期";
	cout.width(4);
	cout << "类型";
	cout.width(18);
	cout << right << "大小（字节）";
	cout.width(15);
	cout << right << "物理地址";
	cout.width(15);
	cout << right << "所有者";
	cout.width(15);
	cout << right << "属性";
	cout << endl;

	Dir tem_dir;
	fs.open("Virtual_Disk.bin", ios::in | ios::binary);
	if (!fs.is_open()) error(READ_FAILED);
	else {
		fs.seekg(indTable[InodeNum].BlockAddr, ios::beg);
		fs.read(reinterpret_cast<char*>(&tem_dir), sizeof(Dir));
		for (unsigned int i = 0;i < tem_dir.num_child;i++) {
			//cout << tem_dir.num_SubInode[i] << endl;
			Inode tem_inode = indTable[tem_dir.num_SubInode[i]];

			cout.width(20);
			cout << left << (i == 0 ? "." : (i == 1 ? ".." : indTable[tem_dir.num_SubInode[i]].fileName));
			tm tmCreatedTime = tem_inode.fileMfyTime.get_time();
			cout.fill('0');
			cout.width(4);
			cout << right << tmCreatedTime.tm_year + 1900;
			cout << "/";
			cout.width(2);
			cout << tmCreatedTime.tm_mon + 1;
			cout << "/";
			cout.width(2);
			cout << tmCreatedTime.tm_mday;
			cout << " ";
			cout.width(2);
			cout << tmCreatedTime.tm_hour;
			cout << ":";
			cout.width(2);
			cout << tmCreatedTime.tm_min;
			cout << ":";
			cout.width(2);
			cout << tmCreatedTime.tm_sec;
			cout << " ";
			cout.fill('\0');
			cout.width(4);
			cout << (tem_inode.fileType == TYPE_DIR ? "目录" : "文件");
			cout.width(18);
			cout << right << (tem_inode.fileType == TYPE_DIR ? " " : to_string(tem_inode.fileSize));
			cout.width(15);
			cout << right << tem_inode.BlockAddr;
			cout.width(15);
			cout << right << tem_inode.userId;
			file_attrib tem_at = tem_inode.fileAttr;
			string attr;
			switch (tem_at) {
			case PROTECTED: attr = "系统";break;
			case READ_WRITE:attr = "读写";break;
			case READ_ONLY:attr = "只读";break;
			case WRITE_ONLY:attr = "只写";break;
			case SHARE:attr = "共享";break;
			}
			cout.width(15);
			cout << right << attr;
			cout << endl;
		}
		fs.close();
	}
}

/*
md …:  创建目录：在指定路径或当前路径下创建指定目录。重名时给出错信息。
*/
void cmd_md(const char* strPath, file_attrib privilege) {
	Dir dirTemp;
	char strDirName[MAX_NAME_LENGTH];
	if (analyse_path(strPath, dirTemp, strDirName)) {//路径合法
		//判断用户权限			if (strcmp(tem_dirName, "user") == 0 && strcmp(indTable[dirTemp.num_Inode].fileName, "root"))return false;
		if (cur_utType != ADMIN && cur_utType != SYSTEM && strcmp(indTable[dirTemp.num_Inode].fileName, "root") != 0 &&
			indTable[dirTemp.num_Inode].fileAttr != SHARE &&
			strcmp(indTable[dirTemp.num_Inode].userId, cur_userId) != 0|| (strcmp(indTable[dirTemp.num_SubInode[1]].fileName, "root") == 0 && strcmp(indTable[dirTemp.num_Inode].fileName, "user")==0))
		{
			error(NO_PRIVILEGE, strPath);
			cout << endl;
			error(MD_FAILED);
			return;
		}
		if (indTable[dirTemp.num_Inode].fileAttr == READ_ONLY)
		{
			error(DIR_READONLY, indTable[dirTemp.num_Inode].fileName);
			cout << endl;
			error(MD_FAILED);
			return;
		}
		long Addr = -1;
		int nInode = -1;
		unsigned int nIndex;
		//是否已存在该名字的子项
		if (dirTemp.have_child(strDirName, nIndex, TYPE_DIR))
		{
			error(FILE_EXIST, indTable[dirTemp.num_Inode].fileName, strDirName);
			cout << endl;
			error(MD_FAILED);
			return;
		}
		//分配目录的存储空间
		Addr = alloc_blocks(DIR_SIZE, nIndex);
		if (Addr < 0)	//空间不足
		{
			error(SPACE_NOT_ENOUGH);
			cout << endl;
			error(MD_FAILED);
		}
		else
		{
			//分配i-结点
			nInode = alloc_inodes();
			if (nInode < 0)
			{
				error(INODE_ALLOC_FAILED);
				cout << endl;
				error(MD_FAILED);
				return;
			}
			//开始创建目录，添加设置相应信息
			Dir dirNew;
			dirNew.num_Inode = (unsigned int)nInode;
			dirNew.num_child = 2;
			dirNew.num_SubInode[0] = (unsigned int)nInode;
			dirNew.num_SubInode[1] = dirTemp.num_Inode;
			strcpy(indTable[dirNew.num_Inode].fileName, strDirName);
			strcpy(indTable[dirNew.num_Inode].userId, cur_userId);
			indTable[nInode].fileType = TYPE_DIR;
			indTable[nInode].fileAttr = privilege;
			indTable[nInode].fileSize = sizeof(Dir);
			indTable[nInode].BlockAddr = Addr;
			indTable[nInode].haveBlocks = DIR_SIZE;
			time_t lTime;
			time(&lTime);
			tm tmCreatedTime = *localtime(&lTime);
			indTable[nInode].fileMfyTime.set_time(tmCreatedTime);
			//在父目录中添加信息
			dirTemp.num_SubInode[dirTemp.num_child] = (unsigned int)nInode;
			dirTemp.num_child++;
			if (dirTemp.num_Inode == cur_dir.num_Inode)	cur_dir = dirTemp;
			//保存
			fs.open("Virtual_Disk.bin", ios::out | ios::binary | ios::_Nocreate);
			if (!fs.is_open()) error(WRITE_FAILED);
			int i;
			for (i = 0; i < BLOCK_GROUPS_NUM; i++) {
				fs.write(reinterpret_cast<char*>(&sbGroups[i]), sizeof(SuperBlock));
				fs.flush();
			}
			fs.seekp((sizeof(SuperBlock) * BLOCK_GROUPS_NUM + sizeof(Inode) * nInode), ios::beg);
			fs.write(reinterpret_cast<char*>(&indTable[nInode]), sizeof(Inode));
			fs.flush();
			fs.seekp((sizeof(SuperBlock) * BLOCK_GROUPS_NUM + sizeof(Inode) * INODES_NUM + sizeof(bmp_status) * nIndex), ios::beg);
			for (i = 0; i < DIR_SIZE; i++) {
				fs.write(reinterpret_cast<char*>(&bsBlockBmp[nIndex + i]), sizeof(bmp_status));
				fs.flush();
			}
			fs.seekp((sizeof(SuperBlock) * BLOCK_GROUPS_NUM + sizeof(Inode) * INODES_NUM + sizeof(bmp_status) * BLOCKS_NUM + sizeof(bmp_status) * nInode), ios::beg);
			fs.write(reinterpret_cast<char*>(&bsInodeBmp[nInode]), sizeof(bmp_status));
			fs.flush();
			fs.seekp(Addr, ios::beg);
			fs.write(reinterpret_cast<char*>(&dirNew), sizeof(Dir));
			fs.flush();
			fs.seekp(indTable[dirTemp.num_Inode].BlockAddr, ios::beg);
			fs.write(reinterpret_cast<char*>(&dirTemp), sizeof(Dir));
			fs.flush();
			fs.close();
		}
	}

	else {
		error(PATH_NOT_FOUND);
		cout << endl;
		error(MD_FAILED);
		return;
	}
}

/*
rd …:  删除目录：删除指定目录下所有文件和子目录。要删目录不空时，要给出提示是否要删除。
*/
void cmd_rd(const char* strPath) {
	Dir dirTemp;
	Dir dirRemove;
	char strDirName[MAX_NAME_LENGTH];
	if (analyse_path(strPath, dirTemp, strDirName)) {//路径合法
		if (strcmp(strDirName, ".") == 0 || strcmp(strDirName, "..") == 0) {
			error(PATH_NOT_FOUND, strPath);
			cout << endl;
			error(RD_FAILED);
			return;
		}
		//判断用户权限
		if (cur_utType != ADMIN && cur_utType != SYSTEM && strcmp(indTable[dirTemp.num_Inode].fileName, "root") != 0 &&
			strcmp(indTable[dirTemp.num_Inode].userId, cur_userId) != 0)
		{
			error(NO_PRIVILEGE, strPath);
			cout << endl;
			error(RD_FAILED);
			return;
		}
		
		if (indTable[dirTemp.num_Inode].fileAttr == READ_ONLY)
		{
			error(DIR_READONLY, indTable[dirTemp.num_Inode].fileName);
			cout << endl;
			error(RD_FAILED);
			return;
		}
		unsigned int nIndex;
		//判断要删除的目录是否存在
		if (!dirTemp.have_child(strDirName, nIndex, TYPE_DIR)) {
			error(DIR_NOT_EXIST, strPath);
			cout << endl;
			error(RD_FAILED);
			return;
		}
		else {
			if (indTable[nIndex].fileType != TYPE_DIR) {
				error(DIR_NOT_EXIST, strPath);
				cout << endl;
				error(RD_FAILED);
			}
			if (nIndex == cur_dir.num_Inode) {
				error(NO_DEL_CUR);
				return;
			}
			if (isAncesotrOfcur_dir(nIndex)) {
				error(DIR_NOT_EXIST, strPath);
				cout << endl;
				error(RD_FAILED);
			}
			if (indTable[nIndex].fileAttr == READ_ONLY)
			{
				error(DIR_READONLY, indTable[nIndex].fileName);
				cout << endl;
				error(RD_FAILED);
				return;
			}
			fs.open("Virtual_Disk.bin", ios::in | ios::binary);
			if (!fs.is_open())error(READ_FAILED);
			else {
				fs.seekg(indTable[nIndex].BlockAddr, ios::beg);
				fs.read(reinterpret_cast<char*>(&dirRemove), sizeof(Dir));
				fs.close();
				remove_dir(dirTemp, dirRemove);
				cout << "删除目录完成!" << endl;
			}
		}
	}
	else {
		error(PATH_NOT_FOUND);
		cout << endl;
		error(RD_FAILED);
	}

}

//删除子文件 从目录dirRemove中删除节点号为nIndex的文件，主要将当前目录项的num_child以及超级块和位图修改并写回虚拟磁盘
void delete_file(Dir& dirRemove, unsigned int nIndex) {
	release_blocks(nIndex);
	release_inodes(nIndex);

	int len = dirRemove.num_child;//目录子项个数
	for (int i = 0;i < len;i++) {
		if (dirRemove.num_SubInode[i] == nIndex) {
			for (int j = i;j < len - 1;j++)
				dirRemove.num_SubInode[j] = dirRemove.num_SubInode[j + 1];
			break;
		}
	}
	dirRemove.num_child--;
	if (cur_dir.num_Inode == dirRemove.num_Inode)cur_dir = dirRemove;
	fs.open("Virtual_Disk.bin", ios::out | ios::binary | ios::_Nocreate);
	if (!fs.is_open()) error(WRITE_FAILED);
	int i;
	for (i = 0; i < BLOCK_GROUPS_NUM; i++) {
		fs.write(reinterpret_cast<char*>(&sbGroups[i]), sizeof(SuperBlock));
		fs.flush();
	}
	unsigned int startblock = (indTable[nIndex].BlockAddr - indTable[0].BlockAddr) / BLOCK_SIZE;
	fs.seekp((sizeof(SuperBlock) * BLOCK_GROUPS_NUM + sizeof(Inode) * INODES_NUM + sizeof(bmp_status) * startblock), ios::beg);
	for (i = 0; i < indTable[nIndex].haveBlocks; i++) {
		fs.write(reinterpret_cast<char*>(&bsBlockBmp[startblock + i]), sizeof(bmp_status));
		fs.flush();
	}
	fs.seekp((sizeof(SuperBlock) * BLOCK_GROUPS_NUM + sizeof(Inode) * INODES_NUM + sizeof(bmp_status) * BLOCKS_NUM + sizeof(bmp_status) * nIndex), ios::beg);
	fs.write(reinterpret_cast<char*>(&bsInodeBmp[nIndex]), sizeof(bmp_status));
	fs.flush();
	fs.seekp(indTable[dirRemove.num_Inode].BlockAddr, ios::beg);
	fs.write(reinterpret_cast<char*>(&dirRemove), sizeof(Dir));
	fs.flush();
	fs.close();

}

//删除子目录
void remove_dir(Dir& dirTemp, Dir& dirRemove) {
	unsigned int dirTempInode = dirTemp.num_Inode, dirRemoveInode = dirRemove.num_Inode;
	if (dirRemove.num_child <= 2) {//该目录下无子目录项

		release_blocks(dirRemoveInode);
		release_inodes(dirRemoveInode);

		int len = dirTemp.num_child;//目录子项个数
		for (int i = 0;i < len;i++) {
			if (dirTemp.num_SubInode[i] == dirRemoveInode) {
				for (int j = i;j < len - 1;j++)
					dirTemp.num_SubInode[j] = dirTemp.num_SubInode[j + 1];
				break;
			}
		}
		dirTemp.num_child--;
		if (cur_dir.num_Inode == dirTemp.num_Inode)cur_dir = dirTemp;
		fs.open("Virtual_Disk.bin", ios::out | ios::binary | ios::_Nocreate);
		if (!fs.is_open()) error(WRITE_FAILED);
		int i;
		for (i = 0; i < BLOCK_GROUPS_NUM; i++) {
			fs.write(reinterpret_cast<char*>(&sbGroups[i]), sizeof(SuperBlock));
			fs.flush();
		}
		unsigned int startblock = (indTable[dirRemoveInode].BlockAddr - indTable[0].BlockAddr) / BLOCK_SIZE;
		fs.seekp((sizeof(SuperBlock) * BLOCK_GROUPS_NUM + sizeof(Inode) * INODES_NUM + sizeof(bmp_status) * startblock), ios::beg);
		for (i = 0; i < indTable[dirRemoveInode].haveBlocks; i++) {
			fs.write(reinterpret_cast<char*>(&bsBlockBmp[startblock + i]), sizeof(bmp_status));
			fs.flush();
		}
		fs.seekp((sizeof(SuperBlock) * BLOCK_GROUPS_NUM + sizeof(Inode) * INODES_NUM + sizeof(bmp_status) * BLOCKS_NUM + sizeof(bmp_status) * dirRemoveInode), ios::beg);
		fs.write(reinterpret_cast<char*>(&bsInodeBmp[dirRemoveInode]), sizeof(bmp_status));
		fs.flush();
		fs.seekp(indTable[dirTempInode].BlockAddr, ios::beg);
		fs.write(reinterpret_cast<char*>(&dirTemp), sizeof(Dir));
		fs.flush();
		fs.close();
	}
	else {//该目录下有子目录/子文件项
			cout <<"Tips:"<< indTable[dirRemove.num_Inode].fileName << "目录下不为空，是否要继续删除<Y/N> ?";
			char init_choice = '\0';
			while (init_choice != 'y' && init_choice != 'Y' && init_choice != 'n' && init_choice != 'N') {
				init_choice = _getch();
			}
			cout << init_choice << endl;
			if (init_choice == 'y' || init_choice == 'Y'){}
			else { 
				error(DEL_FAILED);
				return;
			}

		int dirRemove_son = dirRemove.num_child;
		for (unsigned int i = dirRemove_son - 1;i >= 2;i--) {
			if (indTable[dirRemove.num_SubInode[i]].fileType == TYPE_DIR) {//有子目录项，则递归实现删除
				Dir dirRemoveSon;
				fs.open("Virtual_Disk.bin", ios::in | ios::binary);
				if (!fs.is_open())error(READ_FAILED);
				else {
					fs.seekg(indTable[dirRemove.num_SubInode[i]].BlockAddr, ios::beg);
					fs.read(reinterpret_cast<char*>(&dirRemoveSon), sizeof(Dir));
					fs.close();
					remove_dir(dirRemove, dirRemoveSon);
				}
			}
			else {//删除该目录下的文件
				delete_file(dirRemove, dirRemove.num_SubInode[i]);
			}

		}
		//删除该目录下的子目录/子文件后，再重新调用删除该目录
		remove_dir(dirTemp, dirRemove);
	}
}

/*
newfile …:  建立文件。
*/
void cmd_newfile(const char* strPath, file_attrib privilege) {
	Dir dirTemp;
	char strDirName[MAX_NAME_LENGTH];
	if (analyse_path(strPath, dirTemp, strDirName)) {//路径合法
		//判断用户权限
		if (cur_utType != ADMIN && cur_utType != SYSTEM && strcmp(indTable[dirTemp.num_Inode].fileName, "root") != 0 &&
			indTable[dirTemp.num_Inode].fileAttr != SHARE &&
			strcmp(indTable[dirTemp.num_Inode].userId, cur_userId) != 0)
		{
			error(NO_PRIVILEGE, strPath);
			cout << endl;
			error(NEW_FILE_FAILED);
			return;
		}
		if (indTable[dirTemp.num_Inode].fileAttr == READ_ONLY)
		{
			error(DIR_READONLY, indTable[dirTemp.num_Inode].fileName);
			cout << endl;
			error(NEW_FILE_FAILED);
			return;
		}

		unsigned int nIndex;//返回的文件索引
		if (dirTemp.have_child(strDirName, nIndex, TYPE_FILE))
		{
			error(FILE_EXIST, indTable[dirTemp.num_Inode].fileName, strDirName);
			cout << endl;
			error(NEW_FILE_FAILED);
			return;
		}
		unsigned long strBufferSize = 10;
		char* strBuffer = new char[strBufferSize];
		user_input(strBuffer, strBufferSize);//用户输入
		save_file(strDirName, strBuffer, strBufferSize, dirTemp, privilege);

	}

	else {
		error(PATH_NOT_FOUND);
		cout << endl;
		error(NEW_FILE_FAILED);
		return;
	}
}

//存储文件的函数
void save_file(char* strFileName, char* strBuffer, unsigned int strBufferSize, Dir dirTemp, file_attrib privilege) {
	long Addr = -1;//分配的数据块首地址
	int nInode = -1;//分配的i节点号
	unsigned int nIndex;
	//是否已存在该名字的子项
	//分配目录的存储空间

	Addr = alloc_blocks((strBufferSize * sizeof(char)) / BLOCK_SIZE + 1, nIndex);
	if (Addr < 0)	//空间不足
	{
		error(SPACE_NOT_ENOUGH);
		cout << endl;
		error(NEW_FILE_FAILED);
	}
	else
	{
		//分配i-结点
		nInode = alloc_inodes();
		if (nInode < 0)
		{
			error(INODE_ALLOC_FAILED);
			cout << endl;
			error(NEW_FILE_FAILED);
			return;
		}
		//开始创建i节点信息，添加设置相应信息
		strcpy(indTable[nInode].fileName, strFileName);
		strcpy(indTable[nInode].userId, cur_userId);
		indTable[nInode].fileType = TYPE_FILE;
		indTable[nInode].fileAttr = privilege;
		indTable[nInode].fileSize = strBufferSize * sizeof(char);
		indTable[nInode].BlockAddr = Addr;
		indTable[nInode].haveBlocks = (strBufferSize * sizeof(char)) / BLOCK_SIZE + 1;
		time_t lTime;
		time(&lTime);
		tm tmCreatedTime = *localtime(&lTime);
		indTable[nInode].fileMfyTime.set_time(tmCreatedTime);
		//在父目录中添加信息
		dirTemp.num_SubInode[dirTemp.num_child] = (unsigned int)nInode;
		dirTemp.num_child++;
		if (dirTemp.num_Inode == cur_dir.num_Inode)	cur_dir = dirTemp;
		//保存到磁盘中
		fs.open("Virtual_Disk.bin", ios::out | ios::binary | ios::_Nocreate);
		if (!fs.is_open()) error(WRITE_FAILED);
		int i;
		for (i = 0; i < BLOCK_GROUPS_NUM; i++) {
			fs.write(reinterpret_cast<char*>(&sbGroups[i]), sizeof(SuperBlock));
			fs.flush();
		}
		fs.seekp((sizeof(SuperBlock) * BLOCK_GROUPS_NUM + sizeof(Inode) * nInode), ios::beg);
		fs.write(reinterpret_cast<char*>(&indTable[nInode]), sizeof(Inode));
		fs.flush();
		fs.seekp((sizeof(SuperBlock) * BLOCK_GROUPS_NUM + sizeof(Inode) * INODES_NUM + sizeof(bmp_status) * nIndex), ios::beg);
		for (i = 0; i < indTable[nInode].haveBlocks; i++) {
			fs.write(reinterpret_cast<char*>(&bsBlockBmp[nIndex + i]), sizeof(bmp_status));
			fs.flush();
		}
		fs.seekp((sizeof(SuperBlock) * BLOCK_GROUPS_NUM + sizeof(Inode) * INODES_NUM + sizeof(bmp_status) * BLOCKS_NUM + sizeof(bmp_status) * nInode), ios::beg);
		fs.write(reinterpret_cast<char*>(&bsInodeBmp[nInode]), sizeof(bmp_status));
		fs.flush();
		fs.seekp(Addr, ios::beg);
		fs.write(reinterpret_cast<char*>(strBuffer), indTable[nInode].fileSize);
		fs.flush();
		fs.seekp(indTable[dirTemp.num_Inode].BlockAddr, ios::beg);
		fs.write(reinterpret_cast<char*>(&dirTemp), sizeof(Dir));
		fs.flush();
		fs.close();
	}
}

//用户输入文件信息函数
void user_input(char* strBuffer, unsigned long& strBufferSize) {
	char user_cin = '\0';//用户输入字符
	unsigned long nowBufferSize = 0;
	for (int i = 0;i < strBufferSize;i++)strBuffer[i] = ' ';
	cout << "\n请输入文件内容，按$结束" << endl;
	while ((user_cin = cin.get()) != '$') {
		strBuffer[nowBufferSize] = user_cin;
		nowBufferSize++;
		if (nowBufferSize + 1 >= strBufferSize) {
			char* tem = new char[strBufferSize];
			strcpy(tem, strBuffer);
			delete[]strBuffer;
			strBufferSize = strBufferSize * 2;
			strBuffer = new char[strBufferSize];
			for (int i = 0;i < strBufferSize;i++)strBuffer[i] = ' ';
			strcpy(strBuffer, tem);
			delete[]tem;
		}
	}
	cin.ignore();
	return;
}

/*
cat …:  打开文件。
*/
void cmd_cat(const char* strPath) {
	Dir dirTemp;
	char strDirName[MAX_NAME_LENGTH];
	if (analyse_path(strPath, dirTemp, strDirName)) {//路径合法
		//判断用户权限
		if (cur_utType != ADMIN && cur_utType != SYSTEM && indTable[dirTemp.num_Inode].fileAttr != SHARE && strcmp(indTable[dirTemp.num_Inode].fileName, "root") != 0 &&
			strcmp(indTable[dirTemp.num_Inode].userId, cur_userId) != 0)
		{
			error(NO_PRIVILEGE, strPath);
			cout << endl;
			error(CAT_FAILED);
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
				error(NO_PRIVILEGE, strPath);
				cout << endl;
				error(CAT_FAILED);
				return;
			}

			if (indTable[dirTemp.num_Inode].fileAttr == WRITE_ONLY)
			{
				error(FILE_WRITEONLY, strPath);
				cout << endl;
				error(CAT_FAILED);
				return;
			}
			char* strBuffer = new char[indTable[nIndex].fileSize];

			open_file(nIndex, strBuffer);
			cout << "文件内容：" << endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);
			cout << strBuffer << endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

		}
	}
	else {
		error(PATH_NOT_FOUND);
		cout << endl;
		error(CAT_FAILED);
		return;
	}
}

long open_file(unsigned int nInode, char* strBuffer)
{
	//读取指定地址
	fs.open("Virtual_Disk.bin", ios::in | ios::binary);
	if (!fs.is_open())error(READ_FAILED);
	else {
		fs.seekg((indTable[nInode].BlockAddr), ios::beg);
		fs.read(reinterpret_cast<char*>(strBuffer), indTable[nInode].fileSize);
		fs.close();
	}
	return indTable[nInode].fileSize;
}
/*
del …:  删除文件：删除指定文件，不存在时给出出错信息。
*/
void cmd_del(const char* strPath) {
	Dir dirTemp;
	char strDirName[MAX_NAME_LENGTH];
	if (analyse_path(strPath, dirTemp, strDirName)) {//路径合法
		//判断用户权限
		if (cur_utType != ADMIN && cur_utType != SYSTEM && indTable[dirTemp.num_Inode].fileAttr != SHARE && strcmp(indTable[dirTemp.num_Inode].fileName, "root") != 0 &&
			strcmp(indTable[dirTemp.num_Inode].userId, cur_userId) != 0 || (strcmp(indTable[dirTemp.num_Inode].fileName, "user") == 0 && strcmp(indTable[dirTemp.num_SubInode[1]].fileName, "root") == 0))
		{
			error(NO_PRIVILEGE, strPath);
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
				error(NO_PRIVILEGE, strPath);
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
}

/*
copy …:  拷贝文件，除支持模拟Linux文件系统内部的文件拷贝外，还支持host文件系统与模拟Linux文件系统间的文件拷贝，host文件系统的文件命名为<host>…,
如：将windows下D：盘的文件\data\sample\test.txt文件拷贝到模拟Linux文件系统中的/test/data目录，windows下D：盘的当前目录为D：\data，则使用命令：
simdisk copy <host>D：\data\sample\test.txt /test/data
或者：simdisk copy <host>D：sample\test.txt /test/data
*/
void cmd_copy(const char* strSrcPath, const char* strDesPath){
	char strFileName[MAX_NAME_LENGTH] = { "" };
	char* strBuffer;
	Dir dirTemp;
	char strDiv;
	long nLen = 0;
	size_t nSrcLen = strlen(strSrcPath);
	size_t nDesLen = strlen(strDesPath);
	//复制路径，用于修改
	char* strSrcFinalPath = new char[nSrcLen];
	char* strDesFinalPath = new char[nDesLen];
	strcpy(strSrcFinalPath, strSrcPath);
	strcpy(strDesFinalPath, strDesPath);
	//用 if_host_path 判断来源路径参数中是否带有 <host>
	if (isHost(strSrcFinalPath))	//host 文件复制到模拟磁盘中
	{
		if (isHost(strDesFinalPath))
		{
			error(NOT_BOTH_HOST);
			return;
		}
		//读取宿主机文件
		fstream fsHostIn;
		fsHostIn.open(strSrcFinalPath, ios::in | ios::binary);
		if (!fsHostIn.is_open())
		{
			error(HOST_FILE_NOT_EXIST, strSrcFinalPath);
			return;
		}
		//计算文件长度
		fsHostIn.seekg(0, ios::end);
		nLen = fsHostIn.tellg();
		//分配存储空间
		strBuffer = new char[nLen+1];
		strBuffer[nLen] = 0;
		fsHostIn.seekg(0, ios::beg);
		fsHostIn.read(reinterpret_cast<char*>(strBuffer), nLen);
		fsHostIn.close();
		//提取文件名
		strDiv = '\\';
		cout << strrchr(strSrcFinalPath, strDiv) << endl;
		strcpy(strFileName, strrchr(strSrcFinalPath, strDiv) + 1);
		//分析目标路径

		if (analyse_path(strDesPath, dirTemp))
		{
			//判断用户权限
			if (cur_utType != ADMIN && cur_utType != SYSTEM && strcmp(indTable[dirTemp.num_Inode].fileName, "root") != 0 &&
				indTable[dirTemp.num_Inode].fileAttr != SHARE &&
				strcmp(indTable[dirTemp.num_Inode].userId, cur_userId) != 0)
			{
				error(NO_PRIVILEGE, strDesFinalPath);
				delete(strBuffer);
				return;
			}
			if (indTable[dirTemp.num_Inode].fileAttr == READ_ONLY)
			{
				error(DIR_READONLY, strDesFinalPath);
				delete(strBuffer);
				return;
			}
			//判断目录是否已存在同名子项

			unsigned int nIndex;
			if (dirTemp.have_child(strFileName,nIndex,TYPE_FILE))
			{
				delete(strBuffer);
				error(FILE_EXIST, indTable[dirTemp.num_Inode].fileName, strFileName);
				return;
			}
			//保存到磁盘
			save_file(strFileName, strBuffer, nLen, dirTemp,READ_WRITE);
			delete(strBuffer);
			cout << "文件复制完成！\n";
		}
		else
		{
			error(COPY_FAILED);
		}
	}
	else	//第一个参数不带有 <host>
	{
		if (isHost(strDesFinalPath))	//模拟磁盘文件复制到 host 中
		{
			//分割路径，得到文件名
			if (analyse_path(strSrcFinalPath, dirTemp, strFileName))
			{
				unsigned int nIndex;
				if (!dirTemp.have_child(strFileName, nIndex, TYPE_FILE))
				{//不存在
					error(FILE_NOT_EXIST, indTable[dirTemp.num_Inode].fileName, strFileName);
					cout << endl;
					error(COPY_FAILED);
					return;
				}
				else
				{
					//判断用户权限
					if (cur_utType != ADMIN && cur_utType != SYSTEM && strcmp(indTable[dirTemp.num_Inode].fileName, "root") != 0 &&
						indTable[dirTemp.num_Inode].fileAttr != SHARE &&
						strcmp(indTable[dirTemp.num_Inode].userId, cur_userId)!=0||(strcmp(indTable[dirTemp.num_Inode].fileName,"user")==0&& strcmp(indTable[dirTemp.num_SubInode[1]].fileName, "root") == 0))
					{
						error(NO_PRIVILEGE, strSrcFinalPath);
						return;
					}
					if (indTable[nIndex].fileAttr== WRITE_ONLY)
					{
						error(FILE_WRITEONLY, strSrcFinalPath);
						return;
					}
					//读取文件到内存
					strBuffer = new char[indTable[nIndex].fileSize];
					nLen = open_file(nIndex, strBuffer);
					//合并为宿主机全路径
					char* strFullPath = new char[nSrcLen + nDesLen + 2];
					stringstream ssStream;
					ssStream << strDesFinalPath;
					if (strDesFinalPath[nDesLen - 1] != '\\') ssStream << "\\";
					ssStream << strFileName;
					ssStream >> strFullPath;
					ssStream.clear();
					//写入文件到宿主机系统
					fstream fsHostOut;
					fsHostOut.open(strFullPath, ios::out | ios::binary);
					if (!fsHostOut.is_open())
					{
						error(HOST_FILE_WRITE_FAILED, strFullPath);
						delete(strBuffer);
						delete(strFullPath);
						return;
					}
					fsHostOut.write(reinterpret_cast<char*>(strBuffer), nLen);
					fsHostOut.close();
					delete(strFullPath);
					delete(strBuffer);
					cout << "文件复制完成！\n";
				}
			}
			else
			{
				error(COPY_FAILED);
			}
		}
		else	//模拟磁盘中文件复制
		{
			//分割路径，得到文件名
			if (analyse_path(strSrcFinalPath,dirTemp, strFileName))
			{
				unsigned int nIndex;
				if (!dirTemp.have_child(strFileName, nIndex, TYPE_FILE))
				{//不存在
					error(FILE_NOT_EXIST, indTable[dirTemp.num_Inode].fileName, strFileName);
					cout << endl;
					error(COPY_FAILED);
					return;
				}
				else	//找到
				{
					//判断用户权限
					if (cur_utType != ADMIN && cur_utType != SYSTEM &&
						indTable[dirTemp.num_Inode].fileAttr != SHARE &&
						strcmp(indTable[dirTemp.num_Inode].userId, cur_userId) != 0)
					{
						error(NO_PRIVILEGE, strSrcFinalPath);
						return;
					}
					if (indTable[nIndex].fileAttr== FILE_WRITEONLY)
					{
						error(FILE_WRITEONLY, strSrcFinalPath);
						return;
					}
					file_attrib faReadWrite = indTable[nIndex].fileAttr;
					strBuffer = new char[indTable[nIndex].fileSize];
					nLen = open_file(nIndex, strBuffer);
					//合并为模拟磁盘全路径
					char* strFullPath = new char[nSrcLen + nDesLen + 2];
					stringstream ssStream;
					ssStream << strDesFinalPath;
					if (strDesFinalPath[nDesLen - 1] != '/') ssStream << "/";
					ssStream << strFileName;
					ssStream >> strFullPath;
					ssStream.clear();
					//分割目标路径，得到文件名
					if (analyse_path(strFullPath, dirTemp, strFileName))
					{
						//判断用户权限
						if (cur_utType != ADMIN && cur_utType != SYSTEM &&
						indTable[dirTemp.num_Inode].fileAttr != SHARE &&
						strcmp(indTable[dirTemp.num_Inode].userId, cur_userId) != 0)
						{
							error(NO_PRIVILEGE, strDesFinalPath);
							delete(strBuffer);
							delete(strFullPath);
							return;
						}
						if (indTable[dirTemp.num_Inode].fileAttr== READ_ONLY)
						{
							error(DIR_READONLY, strDesFinalPath);
							delete(strBuffer);
							delete(strFullPath);
							return;
						}
						//判断目录是否已存在同名子项
						unsigned int nindex;
						if (dirTemp.have_child(strFileName,nindex,TYPE_FILE))
						{
							delete(strBuffer);
							delete(strFullPath);
							error(FILE_EXIST, indTable[dirTemp.num_Inode].fileName, strFileName);
							return;
						}
						//保存文件
						save_file(strFileName, strBuffer, nLen,dirTemp, faReadWrite);
						cout << "文件复制完成！\n";
					}
					else
					{
						error(COPY_FAILED);
					}
					delete(strBuffer);
					delete(strFullPath);
				}
			}
			else
			{
				error(COPY_FAILED);
			}
		}
	}
}

//判断是否主机路径，并传回路径
bool isHost(char* strPath) {
	char* strDiv;
	strDiv = strstr(strPath, "<host>");
	if (strDiv == strPath)
	{
		strcpy(strPath, strDiv + 6);
		return true;
	}
	return false;
}