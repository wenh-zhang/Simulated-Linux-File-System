/*
handle.cpp
���ִ�����
*/

#include"func.h"
#include"simdisk.h"


//��������
void error(error_num errorType, const char* strArg1, const char* strArg2) {
	switch (errorType) {
	case LOGON_FAILED:
		cout << "\n\n******************************" << endl;
		cerr << "ȡ����¼�����򼴽��˳�����" << endl;
		Sleep(2000);
		break;
	case WRONG_PWD:
		cout << "\n\n******************************" << endl;
		cerr << "�����������Ѵ� 3 �Σ����򼴽��˳�����" << endl;
		Sleep(2000);
		break;
	case TOO_MANY_ARGS:
		cerr << "Error���������ࡣ�������������� help /command+/? " << endl;
	case INVILID_CMD:
		cerr << "Error����Ч������ " << strArg1 << "���������������� help��\n";
		break;

	case NO_PRIVILEGE:
		cerr << "Error��û��Ȩ�޷��� " << strArg1 << "��";
		break;
	case DIR_READONLY:
		cerr << "Error��Ŀ¼ " << strArg1 << " ֻ�����޷��޸ġ�";
		break;
	case FILE_READONLY:
		cerr << "Error���ļ� " << strArg1 << " ֻ�����޷�ɾ����";
		break;
	case DIR_WRITEONLY:
		cerr << "Error��Ŀ¼ " << strArg1 << " ֻд���޷���ȡ��";
		break;
	case FILE_WRITEONLY:
		cerr << "Error���ļ� " << strArg1 << " ֻд���޷���ȡ��";
		break;
	case READ_FAILED:
		cout << "\n\n******************************\n";
		cerr << "��ȡģ�� Linux �ļ�ϵͳʧ�ܣ�\n";
		break;
	case WRITE_FAILED:
		cout << "\n\n******************************\n";
		cerr << "д��ģ�� Linux �ļ�ϵͳʧ�ܣ�\n";
		break;
	case PATH_NOT_FOUND:
		cerr << "Error���Ҳ���ָ��·��";
		break;
	case FILE_EXIST:
		cerr << "Error��Ŀ¼ " << strArg1 << " ���Ѵ�����Ϊ " << strArg2 << " ���ļ���Ŀ¼��";
		break;
	case FILE_NOT_EXIST:
		cerr << "Error��Ŀ¼ " << strArg1 << " �²�������Ϊ " << strArg2 << " ���ļ���";
		break;
	case DIR_NOT_EXIST:
		cerr << "Error��Ŀ¼ " << strArg1 << " �����ڣ�";
		break;
	case SPACE_NOT_ENOUGH:
		cerr << "Error�����̿ռ䲻�㣡";
		break;
	case INODE_ALLOC_FAILED:
		cerr << "Error��i-�ڵ����ʧ�ܣ�";

	case CD_FAILED:
		cerr << "�л�Ŀ¼ʧ�ܡ�\n";
		break;
	case DIR_FAILED:
		cerr << "Ŀ¼��ʾʧ�ܣ�\n";
		break;
	case MD_FAILED:
		cerr << "Ŀ¼����ʧ�ܣ�\n";
		break;
	case MD_MAX_FAILED:
		cerr << "Ŀ¼����ʧ�ܣ���Ŀ¼���Ѵ����ֵ��\n";
		break;

	case RD_FAILED:
		cerr << "Ŀ¼ɾ��ʧ�ܣ�\n";
		break;
	case NEW_FILE_FAILED:
		cerr << "�ļ�����ʧ�ܣ�\n";
		break;
	case CAT_FAILED:
		cerr << "�ļ���ʧ�ܣ�\n";
		break;
	case DEL_FAILED:
		cerr << "�ļ�ɾ��ʧ�ܣ�\n";
		break;
	case COPY_FAILED:
		cerr << "�ļ�����ʧ�ܣ�\n";
		break;
	case ERROR_PWD:
		cerr << "\n�������\n";
		break;
	
	case NOT_BOTH_HOST:
		cerr << "Error��������Դ��Ŀ�겻�ܾ�Ϊ����������\n";
		break;
	case HOST_FILE_NOT_EXIST:
		cerr << "Error���������������ļ� " << strArg1 << "���ļ�����ʧ�ܣ�\n";
		break;
	case HOST_FILE_WRITE_FAILED:
		cerr << "Error��������д���ļ� " << strArg1 << " �����ļ�����ʧ�ܣ�\n";
		break;
	

	case NO_DEL_CUR:
		cerr << "Error������ɾ����ǰĿ¼��\n";
		break;



	case WRONG_ARGS:
		cerr << "Error������������������������ " << strArg1 << " /?��" << endl;
		break;
	case USER_EXIST:
		cerr << "Error���û� " << strArg1 << " �Ѵ��ڡ�\n";
		break;
	case USER_NOT_EXIST:
		cerr << "Error���û� " << strArg1 << " �����ڡ�\n";
		break;
	case PROTECTED_USER:
		cerr << "Error������ɾ������ĵ�ǰ��¼�û��˺� " << strArg1 << "��\n";
		break;
	case NO_PRIVILEGE_USERDEL:
		cerr << "Error��û��Ȩ��ɾ���˺���Ϣ" << endl;
		break;
	case USERS_MAX:
		cerr << "Error���û������Ѵﵽ����������� " << MAX_USER_NUM << "��\n";
		break;
	}
}