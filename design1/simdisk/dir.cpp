/*
dir.cpp
��Ŀ¼���ļ���صĲ���ָ���
*/
#include"func.h"



/*
cd ��:  �ı�Ŀ¼���ı䵱ǰ����Ŀ¼��Ŀ¼������ʱ����������Ϣ��
*/
void cmd_cd(const char* strArgv) {
	if (strArgv[0] == '\0') {
		return;
	}
	Dir dirTemp;
	if (analyse_path(strArgv, dirTemp)) {//·���Ϸ�
		//�ж��û�Ȩ��
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
dir ��:  ��ʾĿ¼����ʾָ��Ŀ¼�»�ǰĿ¼�µ���Ϣ��
�����ļ����������ַ�������롢�ļ����ȡ���Ŀ¼��
����/s������dir�����ʾ������Ŀ¼����
*/
void cmd_dir(const char* strPath) {

	Dir dirTemp;
	if (strPath[0] == '\0') {
		dirTemp = cur_dir;
	}
	else if (analyse_path(strPath, dirTemp)) {//·���Ϸ�
		//�ж��û�Ȩ��
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
	//��ӡĿ¼����Ϣ
	dir_info(dirTemp.num_Inode);
}

//��ʾĿ¼�µ������Ϣ
void dir_info(unsigned int InodeNum) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
	cout << indTable[InodeNum].fileName << " ��Ŀ¼\n\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	cout.width(20);
	cout << left << "Ŀ¼��/�ļ���";
	cout.width(20);
	cout << left << "�޸�����";
	cout.width(4);
	cout << "����";
	cout.width(18);
	cout << right << "��С���ֽڣ�";
	cout.width(15);
	cout << right << "�����ַ";
	cout.width(15);
	cout << right << "������";
	cout.width(15);
	cout << right << "����";
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
			cout << (tem_inode.fileType == TYPE_DIR ? "Ŀ¼" : "�ļ�");
			cout.width(18);
			cout << right << (tem_inode.fileType == TYPE_DIR ? " " : to_string(tem_inode.fileSize));
			cout.width(15);
			cout << right << tem_inode.BlockAddr;
			cout.width(15);
			cout << right << tem_inode.userId;
			file_attrib tem_at = tem_inode.fileAttr;
			string attr;
			switch (tem_at) {
			case PROTECTED: attr = "ϵͳ";break;
			case READ_WRITE:attr = "��д";break;
			case READ_ONLY:attr = "ֻ��";break;
			case WRITE_ONLY:attr = "ֻд";break;
			case SHARE:attr = "����";break;
			}
			cout.width(15);
			cout << right << attr;
			cout << endl;
		}
		fs.close();
	}
}

/*
md ��:  ����Ŀ¼����ָ��·����ǰ·���´���ָ��Ŀ¼������ʱ��������Ϣ��
*/
void cmd_md(const char* strPath, file_attrib privilege) {
	Dir dirTemp;
	char strDirName[MAX_NAME_LENGTH];
	if (analyse_path(strPath, dirTemp, strDirName)) {//·���Ϸ�
		//�ж��û�Ȩ��			if (strcmp(tem_dirName, "user") == 0 && strcmp(indTable[dirTemp.num_Inode].fileName, "root"))return false;
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
		//�Ƿ��Ѵ��ڸ����ֵ�����
		if (dirTemp.have_child(strDirName, nIndex, TYPE_DIR))
		{
			error(FILE_EXIST, indTable[dirTemp.num_Inode].fileName, strDirName);
			cout << endl;
			error(MD_FAILED);
			return;
		}
		//����Ŀ¼�Ĵ洢�ռ�
		Addr = alloc_blocks(DIR_SIZE, nIndex);
		if (Addr < 0)	//�ռ䲻��
		{
			error(SPACE_NOT_ENOUGH);
			cout << endl;
			error(MD_FAILED);
		}
		else
		{
			//����i-���
			nInode = alloc_inodes();
			if (nInode < 0)
			{
				error(INODE_ALLOC_FAILED);
				cout << endl;
				error(MD_FAILED);
				return;
			}
			//��ʼ����Ŀ¼�����������Ӧ��Ϣ
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
			//�ڸ�Ŀ¼�������Ϣ
			dirTemp.num_SubInode[dirTemp.num_child] = (unsigned int)nInode;
			dirTemp.num_child++;
			if (dirTemp.num_Inode == cur_dir.num_Inode)	cur_dir = dirTemp;
			//����
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
rd ��:  ɾ��Ŀ¼��ɾ��ָ��Ŀ¼�������ļ�����Ŀ¼��ҪɾĿ¼����ʱ��Ҫ������ʾ�Ƿ�Ҫɾ����
*/
void cmd_rd(const char* strPath) {
	Dir dirTemp;
	Dir dirRemove;
	char strDirName[MAX_NAME_LENGTH];
	if (analyse_path(strPath, dirTemp, strDirName)) {//·���Ϸ�
		if (strcmp(strDirName, ".") == 0 || strcmp(strDirName, "..") == 0) {
			error(PATH_NOT_FOUND, strPath);
			cout << endl;
			error(RD_FAILED);
			return;
		}
		//�ж��û�Ȩ��
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
		//�ж�Ҫɾ����Ŀ¼�Ƿ����
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
				cout << "ɾ��Ŀ¼���!" << endl;
			}
		}
	}
	else {
		error(PATH_NOT_FOUND);
		cout << endl;
		error(RD_FAILED);
	}

}

//ɾ�����ļ� ��Ŀ¼dirRemove��ɾ���ڵ��ΪnIndex���ļ�����Ҫ����ǰĿ¼���num_child�Լ��������λͼ�޸Ĳ�д���������
void delete_file(Dir& dirRemove, unsigned int nIndex) {
	release_blocks(nIndex);
	release_inodes(nIndex);

	int len = dirRemove.num_child;//Ŀ¼�������
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

//ɾ����Ŀ¼
void remove_dir(Dir& dirTemp, Dir& dirRemove) {
	unsigned int dirTempInode = dirTemp.num_Inode, dirRemoveInode = dirRemove.num_Inode;
	if (dirRemove.num_child <= 2) {//��Ŀ¼������Ŀ¼��

		release_blocks(dirRemoveInode);
		release_inodes(dirRemoveInode);

		int len = dirTemp.num_child;//Ŀ¼�������
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
	else {//��Ŀ¼������Ŀ¼/���ļ���
			cout <<"Tips:"<< indTable[dirRemove.num_Inode].fileName << "Ŀ¼�²�Ϊ�գ��Ƿ�Ҫ����ɾ��<Y/N> ?";
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
			if (indTable[dirRemove.num_SubInode[i]].fileType == TYPE_DIR) {//����Ŀ¼���ݹ�ʵ��ɾ��
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
			else {//ɾ����Ŀ¼�µ��ļ�
				delete_file(dirRemove, dirRemove.num_SubInode[i]);
			}

		}
		//ɾ����Ŀ¼�µ���Ŀ¼/���ļ��������µ���ɾ����Ŀ¼
		remove_dir(dirTemp, dirRemove);
	}
}

/*
newfile ��:  �����ļ���
*/
void cmd_newfile(const char* strPath, file_attrib privilege) {
	Dir dirTemp;
	char strDirName[MAX_NAME_LENGTH];
	if (analyse_path(strPath, dirTemp, strDirName)) {//·���Ϸ�
		//�ж��û�Ȩ��
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

		unsigned int nIndex;//���ص��ļ�����
		if (dirTemp.have_child(strDirName, nIndex, TYPE_FILE))
		{
			error(FILE_EXIST, indTable[dirTemp.num_Inode].fileName, strDirName);
			cout << endl;
			error(NEW_FILE_FAILED);
			return;
		}
		unsigned long strBufferSize = 10;
		char* strBuffer = new char[strBufferSize];
		user_input(strBuffer, strBufferSize);//�û�����
		save_file(strDirName, strBuffer, strBufferSize, dirTemp, privilege);

	}

	else {
		error(PATH_NOT_FOUND);
		cout << endl;
		error(NEW_FILE_FAILED);
		return;
	}
}

//�洢�ļ��ĺ���
void save_file(char* strFileName, char* strBuffer, unsigned int strBufferSize, Dir dirTemp, file_attrib privilege) {
	long Addr = -1;//��������ݿ��׵�ַ
	int nInode = -1;//�����i�ڵ��
	unsigned int nIndex;
	//�Ƿ��Ѵ��ڸ����ֵ�����
	//����Ŀ¼�Ĵ洢�ռ�

	Addr = alloc_blocks((strBufferSize * sizeof(char)) / BLOCK_SIZE + 1, nIndex);
	if (Addr < 0)	//�ռ䲻��
	{
		error(SPACE_NOT_ENOUGH);
		cout << endl;
		error(NEW_FILE_FAILED);
	}
	else
	{
		//����i-���
		nInode = alloc_inodes();
		if (nInode < 0)
		{
			error(INODE_ALLOC_FAILED);
			cout << endl;
			error(NEW_FILE_FAILED);
			return;
		}
		//��ʼ����i�ڵ���Ϣ�����������Ӧ��Ϣ
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
		//�ڸ�Ŀ¼�������Ϣ
		dirTemp.num_SubInode[dirTemp.num_child] = (unsigned int)nInode;
		dirTemp.num_child++;
		if (dirTemp.num_Inode == cur_dir.num_Inode)	cur_dir = dirTemp;
		//���浽������
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

//�û������ļ���Ϣ����
void user_input(char* strBuffer, unsigned long& strBufferSize) {
	char user_cin = '\0';//�û������ַ�
	unsigned long nowBufferSize = 0;
	for (int i = 0;i < strBufferSize;i++)strBuffer[i] = ' ';
	cout << "\n�������ļ����ݣ���$����" << endl;
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
cat ��:  ���ļ���
*/
void cmd_cat(const char* strPath) {
	Dir dirTemp;
	char strDirName[MAX_NAME_LENGTH];
	if (analyse_path(strPath, dirTemp, strDirName)) {//·���Ϸ�
		//�ж��û�Ȩ��
		if (cur_utType != ADMIN && cur_utType != SYSTEM && indTable[dirTemp.num_Inode].fileAttr != SHARE && strcmp(indTable[dirTemp.num_Inode].fileName, "root") != 0 &&
			strcmp(indTable[dirTemp.num_Inode].userId, cur_userId) != 0)
		{
			error(NO_PRIVILEGE, strPath);
			cout << endl;
			error(CAT_FAILED);
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
			cout << "�ļ����ݣ�" << endl;
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
	//��ȡָ����ַ
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
del ��:  ɾ���ļ���ɾ��ָ���ļ���������ʱ����������Ϣ��
*/
void cmd_del(const char* strPath) {
	Dir dirTemp;
	char strDirName[MAX_NAME_LENGTH];
	if (analyse_path(strPath, dirTemp, strDirName)) {//·���Ϸ�
		//�ж��û�Ȩ��
		if (cur_utType != ADMIN && cur_utType != SYSTEM && indTable[dirTemp.num_Inode].fileAttr != SHARE && strcmp(indTable[dirTemp.num_Inode].fileName, "root") != 0 &&
			strcmp(indTable[dirTemp.num_Inode].userId, cur_userId) != 0 || (strcmp(indTable[dirTemp.num_Inode].fileName, "user") == 0 && strcmp(indTable[dirTemp.num_SubInode[1]].fileName, "root") == 0))
		{
			error(NO_PRIVILEGE, strPath);
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
copy ��:  �����ļ�����֧��ģ��Linux�ļ�ϵͳ�ڲ����ļ������⣬��֧��host�ļ�ϵͳ��ģ��Linux�ļ�ϵͳ����ļ�������host�ļ�ϵͳ���ļ�����Ϊ<host>��,
�磺��windows��D���̵��ļ�\data\sample\test.txt�ļ�������ģ��Linux�ļ�ϵͳ�е�/test/dataĿ¼��windows��D���̵ĵ�ǰĿ¼ΪD��\data����ʹ�����
simdisk copy <host>D��\data\sample\test.txt /test/data
���ߣ�simdisk copy <host>D��sample\test.txt /test/data
*/
void cmd_copy(const char* strSrcPath, const char* strDesPath){
	char strFileName[MAX_NAME_LENGTH] = { "" };
	char* strBuffer;
	Dir dirTemp;
	char strDiv;
	long nLen = 0;
	size_t nSrcLen = strlen(strSrcPath);
	size_t nDesLen = strlen(strDesPath);
	//����·���������޸�
	char* strSrcFinalPath = new char[nSrcLen];
	char* strDesFinalPath = new char[nDesLen];
	strcpy(strSrcFinalPath, strSrcPath);
	strcpy(strDesFinalPath, strDesPath);
	//�� if_host_path �ж���Դ·���������Ƿ���� <host>
	if (isHost(strSrcFinalPath))	//host �ļ����Ƶ�ģ�������
	{
		if (isHost(strDesFinalPath))
		{
			error(NOT_BOTH_HOST);
			return;
		}
		//��ȡ�������ļ�
		fstream fsHostIn;
		fsHostIn.open(strSrcFinalPath, ios::in | ios::binary);
		if (!fsHostIn.is_open())
		{
			error(HOST_FILE_NOT_EXIST, strSrcFinalPath);
			return;
		}
		//�����ļ�����
		fsHostIn.seekg(0, ios::end);
		nLen = fsHostIn.tellg();
		//����洢�ռ�
		strBuffer = new char[nLen+1];
		strBuffer[nLen] = 0;
		fsHostIn.seekg(0, ios::beg);
		fsHostIn.read(reinterpret_cast<char*>(strBuffer), nLen);
		fsHostIn.close();
		//��ȡ�ļ���
		strDiv = '\\';
		cout << strrchr(strSrcFinalPath, strDiv) << endl;
		strcpy(strFileName, strrchr(strSrcFinalPath, strDiv) + 1);
		//����Ŀ��·��

		if (analyse_path(strDesPath, dirTemp))
		{
			//�ж��û�Ȩ��
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
			//�ж�Ŀ¼�Ƿ��Ѵ���ͬ������

			unsigned int nIndex;
			if (dirTemp.have_child(strFileName,nIndex,TYPE_FILE))
			{
				delete(strBuffer);
				error(FILE_EXIST, indTable[dirTemp.num_Inode].fileName, strFileName);
				return;
			}
			//���浽����
			save_file(strFileName, strBuffer, nLen, dirTemp,READ_WRITE);
			delete(strBuffer);
			cout << "�ļ�������ɣ�\n";
		}
		else
		{
			error(COPY_FAILED);
		}
	}
	else	//��һ������������ <host>
	{
		if (isHost(strDesFinalPath))	//ģ������ļ����Ƶ� host ��
		{
			//�ָ�·�����õ��ļ���
			if (analyse_path(strSrcFinalPath, dirTemp, strFileName))
			{
				unsigned int nIndex;
				if (!dirTemp.have_child(strFileName, nIndex, TYPE_FILE))
				{//������
					error(FILE_NOT_EXIST, indTable[dirTemp.num_Inode].fileName, strFileName);
					cout << endl;
					error(COPY_FAILED);
					return;
				}
				else
				{
					//�ж��û�Ȩ��
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
					//��ȡ�ļ����ڴ�
					strBuffer = new char[indTable[nIndex].fileSize];
					nLen = open_file(nIndex, strBuffer);
					//�ϲ�Ϊ������ȫ·��
					char* strFullPath = new char[nSrcLen + nDesLen + 2];
					stringstream ssStream;
					ssStream << strDesFinalPath;
					if (strDesFinalPath[nDesLen - 1] != '\\') ssStream << "\\";
					ssStream << strFileName;
					ssStream >> strFullPath;
					ssStream.clear();
					//д���ļ���������ϵͳ
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
					cout << "�ļ�������ɣ�\n";
				}
			}
			else
			{
				error(COPY_FAILED);
			}
		}
		else	//ģ��������ļ�����
		{
			//�ָ�·�����õ��ļ���
			if (analyse_path(strSrcFinalPath,dirTemp, strFileName))
			{
				unsigned int nIndex;
				if (!dirTemp.have_child(strFileName, nIndex, TYPE_FILE))
				{//������
					error(FILE_NOT_EXIST, indTable[dirTemp.num_Inode].fileName, strFileName);
					cout << endl;
					error(COPY_FAILED);
					return;
				}
				else	//�ҵ�
				{
					//�ж��û�Ȩ��
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
					//�ϲ�Ϊģ�����ȫ·��
					char* strFullPath = new char[nSrcLen + nDesLen + 2];
					stringstream ssStream;
					ssStream << strDesFinalPath;
					if (strDesFinalPath[nDesLen - 1] != '/') ssStream << "/";
					ssStream << strFileName;
					ssStream >> strFullPath;
					ssStream.clear();
					//�ָ�Ŀ��·�����õ��ļ���
					if (analyse_path(strFullPath, dirTemp, strFileName))
					{
						//�ж��û�Ȩ��
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
						//�ж�Ŀ¼�Ƿ��Ѵ���ͬ������
						unsigned int nindex;
						if (dirTemp.have_child(strFileName,nindex,TYPE_FILE))
						{
							delete(strBuffer);
							delete(strFullPath);
							error(FILE_EXIST, indTable[dirTemp.num_Inode].fileName, strFileName);
							return;
						}
						//�����ļ�
						save_file(strFileName, strBuffer, nLen,dirTemp, faReadWrite);
						cout << "�ļ�������ɣ�\n";
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

//�ж��Ƿ�����·����������·��
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