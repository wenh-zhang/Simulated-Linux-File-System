/*
simdisk.h�ж�����ĳ�Ա����ʵ��
*/

#include"func.h"

/*User���Ա��������*/

//�����û���Ϣ
void User::set_user_info(const char* userId, const char* userPassword, user_type utType) {
	strcpy_s(this->userId, userId);
	strcpy_s(this->userPassword, userPassword);
	this->utType = utType;
}

//��ȡ�û�����
user_type User::get_user_type() {
	return this->utType;
}

//��ȡ�û��˺�
const char* User::get_user_id() {
	return this->userId;
}

//�Ƚ��ж������Ƿ���ȷ
bool User::cmp_pwd(const char* userPassword) {
	if (strcmp(this->userPassword, userPassword) == 0)
		return true;
	else return false;
}

/*Time��ĳ�Ա��������*/

//��������ʱ��
void Time::set_time(tm tmDateTime)
{
	this->modify_time = tmDateTime;
}

//��ȡ����ʱ��
tm Time::get_time()
{

	return this->modify_time;
}


/*DirĿ¼���Ա������ʵ��*/


//��ǰĿ¼���Ƿ�����Ϊ strDirName ��Ŀ¼/���ļ�
bool Dir::have_child(const char* strDirName, unsigned int& index, file_type type) {
	if (strcmp(strDirName, "root") == 0) { index = 0;return true; }
	if (strcmp(strDirName, ".") == 0) { index = num_SubInode[0];return true; }
	if (strcmp(strDirName, "..") == 0) { index = num_SubInode[1];return true; }
	for (int i = 2;i < num_child;i++) {
		if (strcmp(indTable[num_SubInode[i]].fileName, strDirName) == 0
			&& indTable[num_SubInode[i]].fileType == type) {
			index = num_SubInode[i];
			return true;
		}
	}

	return false;
}



//��Ŀ¼�Ƿ����������Ŀ¼��
bool Dir::isFull() {
	if (num_child >= MAX_SUBITEM_NUM)return true;
	return false;
}

