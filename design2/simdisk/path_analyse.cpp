/*
path_analyse.cpp
��·���������з���
*/
#include"func.h"

//�ж�strPath�Ƿ�ɴ�ɴ��򴫻ظ�Ŀ¼dirTemp
bool analyse_path(const char* strPath, Dir& dirTemp)
{
	dirTemp = cur_dir;	//��ʱĿ¼
		//��ʼ���·��
	char tem_dirName[MAX_NAME_LENGTH];//Ŀǰ�ļ���
	char left_dirName[MAX_NAME_LENGTH];//ʣ��Ŀ¼
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

//�ж�strPath�еĵ����ڶ���Ŀ¼/�ļ��Ƿ�ɴ���������һ��Ŀ¼/�ļ���strFileName�����ص����ڶ���Ŀ¼dirTemp
bool analyse_path(const char* strPath, Dir& dirTemp, char* strFileName)
{
	dirTemp = cur_dir;	//��ʱĿ¼
		//��ʼ���·��
	char tem_dirName[MAX_NAME_LENGTH];//Ŀǰ�ļ���
	char left_dirName[MAX_NAME_LENGTH];//ʣ��Ŀ¼
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


//��ʾ��ǰ��·�����ݹ����
void set_cur_path(Dir dirCurDir)
{
	Dir dirTemp = dirCurDir;
	//�ݹ���ң�ֱ���ҵ���Ŀ¼root
	if (dirCurDir.num_Inode != 0)
	{
		//���Ҹ�Ŀ¼
		fs.open("Virtual_Disk.bin", ios::in | ios::binary);
		if (!fs.is_open()) error(READ_FAILED);
		fs.seekg(indTable[dirCurDir.num_SubInode[1]].BlockAddr | ios::beg);
		fs.read(reinterpret_cast<char*>(&dirCurDir), sizeof(Dir));
		fs.close();
		set_cur_path(dirCurDir);
	}
	//���õ�ǰ·���ַ���
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

//��ȡ·���ϵ�Ŀ¼��������������
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

//�жϸ�i-�ڵ�Ŷ�Ӧ��Ŀ¼�Ƿ�Ϊ��ǰĿ¼�����ڽڵ�
bool isAncesotrOfcur_dir(unsigned int nIndex) {
	Dir dirTemp = cur_dir;
	//�ݹ���ң�ֱ���ҵ���Ŀ¼root
	while (dirTemp.num_Inode != 0)
	{
		//���Ҹ�Ŀ¼
		fs.open("Virtual_Disk.bin", ios::in | ios::binary);
		if (!fs.is_open()) error(READ_FAILED);
		fs.seekg(indTable[dirTemp.num_SubInode[1]].BlockAddr | ios::beg);
		fs.read(reinterpret_cast<char*>(&dirTemp), sizeof(Dir));
		fs.close();
		if (dirTemp.num_Inode == nIndex)return true;
	}
	return false;
	
}
//������ת��ΪСд
void mytolower(char* s) {
	int len = strlen(s);
	for (int i = 0;i < len;i++) {
		if (s[i] >= 'A' && s[i] <= 'Z') {
			s[i] += 32;//+32ת��ΪСд
			//s[i]=s[i]-'A'+'a';
		}
	}
}

