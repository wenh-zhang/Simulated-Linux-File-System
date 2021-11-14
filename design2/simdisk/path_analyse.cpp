/*
path_analyse.cpp
对路径参数进行分析
*/
#include"func.h"

//判断strPath是否可达，可达则传回该目录dirTemp
bool analyse_path(const char* strPath, Dir& dirTemp)
{
	dirTemp = cur_dir;	//临时目录
		//开始拆分路径
	char tem_dirName[MAX_NAME_LENGTH];//目前文件夹
	char left_dirName[MAX_NAME_LENGTH];//剩余目录
	strcpy(left_dirName, strPath);
	vector<string> dirNames{};
	get_dirName(left_dirName, dirNames);

	unsigned int inodeNum = cur_dir.num_Inode;

	for (int i = 0;i < dirNames.size();i++) {

		strcpy(tem_dirName, dirNames[i].c_str());
		if (!dirTemp.have_child(tem_dirName, inodeNum, TYPE_DIR)) {
			return false;
		}
		else {

			fs.open("Virtual_Disk.bin", ios::in | ios::binary);
			if (!fs.is_open())error(READ_FAILED);
			else {
				fs.seekg((indTable[inodeNum].BlockAddr), ios::beg);
				fs.read(reinterpret_cast<char*>(&dirTemp), sizeof(Dir));
				fs.close();
			}

		}
	}
	return true;
}

//判断strPath中的倒数第二个目录/文件是否可达，并传回最后一个目录/文件名strFileName，传回倒数第二个目录dirTemp
bool analyse_path(const char* strPath, Dir& dirTemp, char* strFileName)
{
	dirTemp = cur_dir;	//临时目录
		//开始拆分路径
	char tem_dirName[MAX_NAME_LENGTH];//目前文件夹
	char left_dirName[MAX_NAME_LENGTH];//剩余目录
	strcpy(left_dirName, strPath);
	vector<string> dirNames{};
	get_dirName(left_dirName, dirNames);

	
	unsigned int inodeNum = cur_dir.num_Inode;

	for (int i = 0;i < dirNames.size() - 1;i++) {
		
		strcpy(tem_dirName, dirNames[i].c_str());
		if (!dirTemp.have_child(tem_dirName, inodeNum, TYPE_DIR)) {
			return false;
		}
		else {
			
			fs.open("Virtual_Disk.bin", ios::in | ios::binary);
			if (!fs.is_open())error(READ_FAILED);
			else {
				fs.seekg((indTable[inodeNum].BlockAddr), ios::beg);
				fs.read(reinterpret_cast<char*>(&dirTemp), sizeof(Dir));
				fs.close();

			}
		}
	}
	if(strcmp(dirNames[dirNames.size() - 1].c_str(),"root")!=0)
	strcpy(strFileName, dirNames[dirNames.size() - 1].c_str());
	return true;
}


//显示当前的路径，递归查找
void set_cur_path(Dir dirCurDir)
{
	Dir dirTemp = dirCurDir;
	//递归查找，直到找到根目录root
	if (dirCurDir.num_Inode != 0)
	{
		//查找父目录
		fs.open("Virtual_Disk.bin", ios::in | ios::binary);
		if (!fs.is_open()) error(READ_FAILED);
		fs.seekg(indTable[dirCurDir.num_SubInode[1]].BlockAddr | ios::beg);
		fs.read(reinterpret_cast<char*>(&dirCurDir), sizeof(Dir));
		fs.close();
		set_cur_path(dirCurDir);
	}
	//设置当前路径字符串
	if (dirTemp.num_Inode == 0)
	{
		strcpy(cur_dirName, "root");
	}
	else
	{
		stringstream ssStream;
		ssStream << cur_dirName << "/" << indTable[dirTemp.num_Inode].fileName;
		ssStream >> cur_dirName;
		ssStream.clear();
	}
}

//获取路径上的目录名并存入数组中
void get_dirName(const char* strPath, vector<string>& dirNames) {
	char x[MAX_NAME_LENGTH];
	strcpy(x, strPath);
	char* name = strtok(x, " /");
	while (name != NULL) {
		dirNames.push_back(name);
		name = strtok(NULL, " /");
	}
	return;

}

//判断该i-节点号对应的目录是否为当前目录的祖宗节点
bool isAncesotrOfcur_dir(unsigned int nIndex) {
	Dir dirTemp = cur_dir;
	//递归查找，直到找到根目录root
	while (dirTemp.num_Inode != 0)
	{
		//查找父目录
		fs.open("Virtual_Disk.bin", ios::in | ios::binary);
		if (!fs.is_open()) error(READ_FAILED);
		fs.seekg(indTable[dirTemp.num_SubInode[1]].BlockAddr | ios::beg);
		fs.read(reinterpret_cast<char*>(&dirTemp), sizeof(Dir));
		fs.close();
		if (dirTemp.num_Inode == nIndex)return true;
	}
	return false;
	
}
//将命令转换为小写
void mytolower(char* s) {
	int len = strlen(s);
	for (int i = 0;i < len;i++) {
		if (s[i] >= 'A' && s[i] <= 'Z') {
			s[i] += 32;//+32转换为小写
			//s[i]=s[i]-'A'+'a';
		}
	}
}

