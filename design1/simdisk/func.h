/*
fuc.h
��������
*/
#pragma once
#include"simdisk.h"

/*ʵ�������еĹ��ܺ���*/

//info:  ��ʾ����ϵͳ��Ϣ(�ο�Linux�ļ�ϵͳ��ϵͳ��Ϣ
void cmd_info();
//cd �� : �ı�Ŀ¼���ı䵱ǰ����Ŀ¼��Ŀ¼������ʱ����������Ϣ��
void cmd_cd(const char*strPath);
//dir �� : ��ʾĿ¼����ʾָ��Ŀ¼�»�ǰĿ¼�µ���Ϣ�������ļ����������ַ�������롢�ļ����ȡ���Ŀ¼�ȣ��� / s������dir�����ʾ������Ŀ¼����
void cmd_dir(const char* strPath);
//md ��:  ����Ŀ¼����ָ��·����ǰ·���´���ָ��Ŀ¼������ʱ��������Ϣ��
void cmd_md(const char* strPath,file_attrib privilege);
//rd ��:  ɾ��Ŀ¼��ɾ��ָ��Ŀ¼�������ļ�����Ŀ¼��ҪɾĿ¼����ʱ��Ҫ������ʾ�Ƿ�Ҫɾ����
void cmd_rd(const char* strPath);
//newfile ��:  �����ļ���
void cmd_newfile(const char* strPath, file_attrib privilege);
//cat ��:  ���ļ���
void cmd_cat(const char* strPath);
//copy ��:  �����ļ�����֧��ģ��Linux�ļ�ϵͳ�ڲ����ļ������⣬��֧��host�ļ�ϵͳ��ģ��Linux�ļ�ϵͳ����ļ�������host�ļ�ϵͳ���ļ�����Ϊ<host>��
void cmd_copy(const char* srcPath, const char* desPath);
//del ��:  ɾ���ļ���ɾ��ָ���ļ���������ʱ����������Ϣ��
void cmd_del(const char* strPath);
//check: ��Ⲣ�ָ��ļ�ϵͳ�����ļ�ϵͳ�е�����һ���Խ��м�⣬���Զ������ļ�ϵͳ�Ľṹ����Ϣ��������������
void cmd_check();
//init����ʼ��/��ʽ���ļ�ϵͳ��ɾ��������Ϣ�����½������̿ռ䡣
void cmd_init();
//cls������
void cmd_cls();
//help����ӡϵͳ����������Ϣ
void cmd_help(const char* strArg);
//logon�����µ�½ϵͳ
bool cmd_logon();
//useradd�������û�
void cmd_useradd(const char* strUserId, user_type utType);
//userdel��ɾ���û�
void cmd_userdel(const char* strUserId);
//exit���˳�ϵͳ
void cmd_exit();
//ϵͳִ��ָ��
void cmd_execute(const char* commondName, const char* strArgv1="", const char* strArgv2="");

/*��Դ������յ��йغ���*/

//Ϊ�´�����Ŀ¼/�ļ��������ݿ飻argv1:�������ݿ�/argv2:�������ʼ���ݿ������������׵�ַ
long alloc_blocks(unsigned int fileBlocks, unsigned int& startblock_index);
//Ϊ�´�����Ŀ¼/�ļ�����i-�ڵ㣻����i-�ڵ��
int alloc_inodes();
//�ͷ����ݿ飻argv:��ɾ���ļ�/Ŀ¼��i-�ڵ�
void release_blocks(unsigned int nIndex);
//�ͷ�i-�ڵ㣻argv:��ɾ���ļ�/Ŀ¼��i-�ڵ�
void release_inodes(unsigned int nIndex);

/*����Ŀ¼���ļ�����ĸ�������*/

//����·���Ƿ�Ϸ���ֱ�Ӷ�λ������Ŀ¼��argv1:·��/argv2:����Ŀ¼
bool analyse_path(const char* strPath, Dir& dirTemp);
//����·���Ƿ�Ϸ���ֱ�Ӷ�λ�������ڶ���·������argv1:·��/argv2:��λĿ¼/argv3:����·����
bool analyse_path(const char* strPath, Dir& dirTemp, char* strFileName);
//��ӡ��ӦĿ¼�µ�Ŀ¼/�ļ���Ϣ��argv:���鿴Ŀ¼��i-�ڵ�
void dir_info(unsigned int InodeNum);
//��ȡ·���е�Ŀ¼/�ļ�����������vector��
void get_dirName(const char* strPath, vector<string>& dirNames);
//���ݴ����Ŀ¼��������·����
void set_cur_path(Dir dirCurDir);
//ɾ��Ŀ¼���ļ���argv1:��������Ŀ¼��/argv2:��ɾ���ļ���i-�ڵ��
void delete_file(Dir& dirRemove, unsigned int nIndex);
//ɾ��Ŀ¼�µ���Ŀ¼���Ŀ¼�����������argv1:��Ŀ¼��/argv2:��Ŀ¼��
void remove_dir(Dir& dirTemp, Dir& dirRemove);
//�жϸ�i-�ڵ�Ŷ�Ӧ��Ŀ¼�Ƿ�Ϊ��ǰĿ¼�����ڽڵ�
bool isAncesotrOfcur_dir(unsigned int nIndex);
//�ж��Ƿ�����·��
bool isHost(char* strPath);
//�洢�ļ��ĺ���
void save_file(char* strFileName, char* strBuffer, unsigned int strBufferSize, Dir dirTemp, file_attrib privilege);
//�û������ļ���Ϣ����
void user_input(char* strBuffer, unsigned long& strBufferSize);
//���ļ�����ȡ����
long open_file(unsigned int nInode, char* strBuffer);

//simdisk��ʱ���غ���
void load();
void register_user();
void get_user_info();
void mytolower(char* s);

//��Ϣ������
void error(error_num errorType, const char* strPath1="",const char* strPath2="");
// ���������*����
bool cin_pwd(char* strPwd, unsigned int nLen);
// �˺�����Ĵ���
bool cin_id(char* strid, unsigned int nLen);
