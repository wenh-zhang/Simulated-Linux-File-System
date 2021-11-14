/*
logon.cpp
��¼
*/
#include"func.h"

bool cmd_logon() {
	unsigned int nCount = 3;
	while (nCount > 0)
	{
		nCount--;
		cout << "ģ��Linux�ļ�ϵͳ\n���ȵ�¼�˺�\n\n";
		char nUid[MAX_ID_LENGTH];
		char strPwd[MAX_PWD_LENGTH];
		//�����ʺ�
		cout << "�˺ţ�";
		if (!cin_id(nUid, MAX_ID_LENGTH))
		{
			error(LOGON_FAILED);
			return false;
		}
		cout << endl;
		//�������룬*����
		cout << "���룺";
		if (!cin_pwd(strPwd, MAX_PWD_LENGTH))
		{
			error(LOGON_FAILED);
			return false;
		}
		for (unsigned int i = 0; i < cur_Idsum ; i++)
		{
			// �Ƚ������Ƿ���ȷ
			//cout << systemUsers[i].get_user_id() << endl;
			//cout << systemUsers[i].userPassword << endl;
			if (strcmp(nUid , systemUsers[i].get_user_id())==0
				&& systemUsers[i].cmp_pwd(strPwd))
			{
				memset(cur_userId, 0, MAX_ID_LENGTH);
				strcpy(cur_userId, nUid);
				cur_utType = systemUsers[i].get_user_type();
				cout << "\n\n******************************\n";
				cout << "��¼�ɹ������Ժ򡭡�\n";
				Sleep(800);
				stringstream ssStream;
				ssStream << "title simdisk [�ѵ�¼��" << cur_userId << "(";
				if (cur_utType == USER)
					ssStream << "��ͨ�û�";
				else
					ssStream << "����Ա";
				ssStream << ")]";
				ssStream.clear();
				system(ssStream.str().c_str());
				return true;
			}
		}
		//��� 3 ���������
		if (nCount > 0)
		{
			cout << "\n\n******************************" << endl;
			cout << "�ʺŲ����ڻ��������\n�㻹��" << nCount << "�����Ի��ᡣ" << endl;
			Sleep(2000);
			system("cls");
		}
	}
	error(WRONG_PWD);
	return false;
}
// ���������*����
bool cin_pwd(char* strPwd, unsigned int nLen)
{
	char chGet = '\0';
	unsigned int i = 0;

	while (1)
	{
		i = 0;
		while (chGet != '\r' && i < nLen)	//����
		{
			chGet = _getch();
			if (chGet == '\r')
			{
				if (i != 0)
				{
					strPwd[i] = '\0';
					return true; //���سɹ�
				}
				else	//��ֹ�������¼
				{
					chGet = '\0';
					continue;
				}
			}
			if (chGet == ' ' || chGet == '\t') continue;
			if (chGet == 27)	//�û���ESCȡ��������ʧ��
				return false;
			if (chGet == '\b')	//�����˸��
			{
				if (i > 0)
				{
					--i;
					cout << "\b \b"; //�˸�
				}
				else
				{
					break;
				}
			}
			else
			{
				cout.put('*'); //�������Ǻ���ʾ
				strPwd[i++] = chGet;
			}

		}
		strPwd[i] = '\0';
		return true; //���سɹ�
	}
}

// �˺�����Ĵ���
bool cin_id(char* strid, unsigned int nLen)
{
	char chGet = '\0';
	unsigned int i = 0;

	while (1)
	{
		i = 0;
		while (chGet != '\r' && i < nLen)	//�˺�
		{
			chGet = _getch();
			if (chGet == '\r')
			{
				if (i != 0)
				{
					strid[i] = '\0';
					return true; //���سɹ�
				}
				else	//��ֹ���˻�
				{
					chGet = '\0';
					continue;
				}
			}
			if (chGet == ' ' || chGet == '\t') continue;
			if (chGet == 27)	//�û���ESCȡ��������ʧ��
				return false;
			if (chGet == '\b')	//�����˸��
			{
				if (i > 0)
				{
					--i;
					cout << "\b \b"; //�˸�
				}
				else
				{
					break;
				}
			}
			else
			{
				cout.put(chGet); //�˻���ʾ
				strid[i++] = chGet;
			}

		}
		strid[i] = '\0';
		return true; //���سɹ�
	}
}