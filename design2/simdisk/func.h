/*
fuc.h
函数声明
*/
#pragma once
#include"simdisk.h"

/*实现命令行的功能函数*/

//info:  显示整个系统信息(参考Linux文件系统的系统信息
void cmd_info();
//cd … : 改变目录：改变当前工作目录，目录不存在时给出出错信息。
void cmd_cd(const char*strPath);
//dir … : 显示目录：显示指定目录下或当前目录下的信息，包括文件名、物理地址、保护码、文件长度、子目录等（带 / s参数的dir命令，显示所有子目录）。
void cmd_dir(const char* strPath);
//md …:  创建目录：在指定路径或当前路径下创建指定目录。重名时给出错信息。
void cmd_md(const char* strPath,file_attrib privilege);
//rd …:  删除目录：删除指定目录下所有文件和子目录。要删目录不空时，要给出提示是否要删除。
void cmd_rd(const char* strPath);
//newfile …:  建立文件。
void cmd_newfile(const char* strPath, file_attrib privilege);
//cat …:  打开文件。
void cmd_cat(const char* strPath);
//copy …:  拷贝文件，除支持模拟Linux文件系统内部的文件拷贝外，还支持host文件系统与模拟Linux文件系统间的文件拷贝，host文件系统的文件命名为<host>…
void cmd_copy(const char* srcPath, const char* desPath);
//del …:  删除文件：删除指定文件，不存在时给出出错信息。
void cmd_del(const char* strPath);
//check: 检测并恢复文件系统：对文件系统中的数据一致性进行检测，并自动根据文件系统的结构和信息进行数据再整理。
void cmd_check();
//init：初始化/格式化文件系统，删除所有信息，重新建立磁盘空间。
void cmd_init();
//cls：清屏
void cmd_cls();
//help：打印系统操作辅助信息
void cmd_help(const char* strArg);
//logon：重新登陆系统
bool cmd_logon();
//useradd：增加用户
void cmd_useradd(const char* strUserId, user_type utType);
//userdel：删除用户
void cmd_userdel(const char* strUserId);
//exit：退出系统
void cmd_exit();
//系统执行指令
void cmd_execute(const char* commondName, const char* strArgv1="", const char* strArgv2="");

/*资源分配回收的有关函数*/

//为新创建的目录/文件分配数据块；argv1:所需数据块/argv2:分配的起始数据块索引；返回首地址
long alloc_blocks(unsigned int fileBlocks, unsigned int& startblock_index);
//为新创建的目录/文件分配i-节点；返回i-节点号
int alloc_inodes();
//释放数据块；argv:被删除文件/目录的i-节点
void release_blocks(unsigned int nIndex);
//释放i-节点；argv:被删除文件/目录的i-节点
void release_inodes(unsigned int nIndex);

/*关于目录、文件命令的辅助函数*/

//分析路径是否合法，直接定位至最终目录；argv1:路径/argv2:最终目录
bool analyse_path(const char* strPath, Dir& dirTemp);
//分析路径是否合法，直接定位至倒数第二个路径名；argv1:路径/argv2:定位目录/argv3:最终路径名
bool analyse_path(const char* strPath, Dir& dirTemp, char* strFileName);
//打印对应目录下的目录/文件信息；argv:所查看目录的i-节点
void dir_info(unsigned int InodeNum);
//获取路径中的目录/文件名，并存入vector中
void get_dirName(const char* strPath, vector<string>& dirNames);
//根据传入的目录参数设置路径名
void set_cur_path(Dir dirCurDir);
//删除目录下文件；argv1:所操作的目录项/argv2:被删除文件的i-节点号
void delete_file(Dir& dirRemove, unsigned int nIndex);
//删除目录下的子目录项（该目录项下无子项）；argv1:父目录项/argv2:子目录项
void remove_dir(Dir& dirTemp, Dir& dirRemove);
//判断该i-节点号对应的目录是否为当前目录的祖宗节点
bool isAncesotrOfcur_dir(unsigned int nIndex);
//判断是否主机路径
bool isHost(char* strPath);
//存储文件的函数
void save_file(char* strFileName, char* strBuffer, unsigned int strBufferSize, Dir dirTemp, file_attrib privilege);
//用户输入文件信息函数
void user_input(char* strBuffer, unsigned long& strBufferSize);
//打开文件并读取数据
long open_file(unsigned int nInode, char* strBuffer);

//simdisk打开时加载函数
void load();
void register_user();
void get_user_info();
void mytolower(char* s);

//信息处理函数
void error(error_num errorType, const char* strPath1="",const char* strPath2="");
// 密码输入的*处理
bool cin_pwd(char* strPwd, unsigned int nLen);
// 账号输入的处理
bool cin_id(char* strid, unsigned int nLen);
