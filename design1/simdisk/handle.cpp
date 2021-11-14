/*
handle.cpp
各种处理函数
*/

#include"func.h"
#include"simdisk.h"


//出错处理函数
void error(error_num errorType, const char* strArg1, const char* strArg2) {
	switch (errorType) {
	case LOGON_FAILED:
		cout << "\n\n******************************" << endl;
		cerr << "取消登录，程序即将退出……" << endl;
		Sleep(2000);
		break;
	case WRONG_PWD:
		cout << "\n\n******************************" << endl;
		cerr << "密码错误次数已达 3 次，程序即将退出……" << endl;
		Sleep(2000);
		break;
	case TOO_MANY_ARGS:
		cerr << "Error：参数过多。如需帮助，请键入 help /command+/? " << endl;
	case INVILID_CMD:
		cerr << "Error：无效的命令 " << strArg1 << "！如需帮助，请键入 help。\n";
		break;

	case NO_PRIVILEGE:
		cerr << "Error：没有权限访问 " << strArg1 << "。";
		break;
	case DIR_READONLY:
		cerr << "Error：目录 " << strArg1 << " 只读，无法修改。";
		break;
	case FILE_READONLY:
		cerr << "Error：文件 " << strArg1 << " 只读，无法删除。";
		break;
	case DIR_WRITEONLY:
		cerr << "Error：目录 " << strArg1 << " 只写，无法读取。";
		break;
	case FILE_WRITEONLY:
		cerr << "Error：文件 " << strArg1 << " 只写，无法读取。";
		break;
	case READ_FAILED:
		cout << "\n\n******************************\n";
		cerr << "读取模拟 Linux 文件系统失败！\n";
		break;
	case WRITE_FAILED:
		cout << "\n\n******************************\n";
		cerr << "写入模拟 Linux 文件系统失败！\n";
		break;
	case PATH_NOT_FOUND:
		cerr << "Error：找不到指定路径";
		break;
	case FILE_EXIST:
		cerr << "Error：目录 " << strArg1 << " 下已存在名为 " << strArg2 << " 的文件或目录！";
		break;
	case FILE_NOT_EXIST:
		cerr << "Error：目录 " << strArg1 << " 下不存在名为 " << strArg2 << " 的文件！";
		break;
	case DIR_NOT_EXIST:
		cerr << "Error：目录 " << strArg1 << " 不存在！";
		break;
	case SPACE_NOT_ENOUGH:
		cerr << "Error：磁盘空间不足！";
		break;
	case INODE_ALLOC_FAILED:
		cerr << "Error：i-节点分配失败！";

	case CD_FAILED:
		cerr << "切换目录失败。\n";
		break;
	case DIR_FAILED:
		cerr << "目录显示失败！\n";
		break;
	case MD_FAILED:
		cerr << "目录创建失败！\n";
		break;
	case MD_MAX_FAILED:
		cerr << "目录创建失败！子目录数已达最大值！\n";
		break;

	case RD_FAILED:
		cerr << "目录删除失败！\n";
		break;
	case NEW_FILE_FAILED:
		cerr << "文件创建失败！\n";
		break;
	case CAT_FAILED:
		cerr << "文件打开失败！\n";
		break;
	case DEL_FAILED:
		cerr << "文件删除失败！\n";
		break;
	case COPY_FAILED:
		cerr << "文件复制失败！\n";
		break;
	case ERROR_PWD:
		cerr << "\n密码错误！\n";
		break;
	
	case NOT_BOTH_HOST:
		cerr << "Error：复制来源和目标不能均为在宿主机！\n";
		break;
	case HOST_FILE_NOT_EXIST:
		cerr << "Error：宿主机不存在文件 " << strArg1 << "，文件复制失败！\n";
		break;
	case HOST_FILE_WRITE_FAILED:
		cerr << "Error：宿主机写入文件 " << strArg1 << " 出错，文件复制失败！\n";
		break;
	

	case NO_DEL_CUR:
		cerr << "Error：不能删除当前目录！\n";
		break;



	case WRONG_ARGS:
		cerr << "Error：参数错误。如需帮助，请键入 " << strArg1 << " /?。" << endl;
		break;
	case USER_EXIST:
		cerr << "Error：用户 " << strArg1 << " 已存在。\n";
		break;
	case USER_NOT_EXIST:
		cerr << "Error：用户 " << strArg1 << " 不存在。\n";
		break;
	case PROTECTED_USER:
		cerr << "Error：不能删除或更改当前登录用户账号 " << strArg1 << "。\n";
		break;
	case NO_PRIVILEGE_USERDEL:
		cerr << "Error：没有权限删除账号信息" << endl;
		break;
	case USERS_MAX:
		cerr << "Error：用户数量已达到最大数量限制 " << MAX_USER_NUM << "。\n";
		break;
	}
}