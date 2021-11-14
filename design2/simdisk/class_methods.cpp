/*
simdisk.h中定义类的成员函数实现
*/

#include"func.h"

/*User类成员函数定义*/

//设置用户信息
void User::set_user_info(const char* userId, const char* userPassword, user_type utType) {
	strcpy_s(this->userId, userId);
	strcpy_s(this->userPassword, userPassword);
	this->utType = utType;
}

//获取用户类型
user_type User::get_user_type() {
	return this->utType;
}

//获取用户账号
const char* User::get_user_id() {
	return this->userId;
}

//比较判断密码是否正确
bool User::cmp_pwd(const char* userPassword) {
	if (strcmp(this->userPassword, userPassword) == 0)
		return true;
	else return false;
}

/*Time类的成员函数定义*/

//设置日期时间
void Time::set_time(tm tmDateTime)
{
	this->modify_time = tmDateTime;
}

//获取日期时间
tm Time::get_time()
{

	return this->modify_time;
}


/*Dir目录类成员函数的实现*/


//当前目录下是否有名为 strDirName 子目录/子文件
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



//该目录是否已满最大子目录数
bool Dir::isFull() {
	if (num_child >= MAX_SUBITEM_NUM)return true;
	return false;
}

