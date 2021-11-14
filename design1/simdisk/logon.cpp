/*
logon.cpp
登录
*/
#include"func.h"

bool cmd_logon() {
	unsigned int nCount = 3;
	while (nCount > 0)
	{
		nCount--;
		cout << "模拟Linux文件系统\n请先登录账号\n\n";
		char nUid[MAX_ID_LENGTH];
		char strPwd[MAX_PWD_LENGTH];
		//输入帐号
		cout << "账号：";
		if (!cin_id(nUid, MAX_ID_LENGTH))
		{
			error(LOGON_FAILED);
			return false;
		}
		cout << endl;
		//输入密码，*处理
		cout << "密码：";
		if (!cin_pwd(strPwd, MAX_PWD_LENGTH))
		{
			error(LOGON_FAILED);
			return false;
		}
		for (unsigned int i = 0; i < cur_Idsum ; i++)
		{
			// 比较密码是否正确
			//cout << systemUsers[i].get_user_id() << endl;
			//cout << systemUsers[i].userPassword << endl;
			if (strcmp(nUid , systemUsers[i].get_user_id())==0
				&& systemUsers[i].cmp_pwd(strPwd))
			{
				memset(cur_userId, 0, MAX_ID_LENGTH);
				strcpy(cur_userId, nUid);
				cur_utType = systemUsers[i].get_user_type();
				cout << "\n\n******************************\n";
				cout << "登录成功！请稍候……\n";
				Sleep(800);
				stringstream ssStream;
				ssStream << "title simdisk [已登录：" << cur_userId << "(";
				if (cur_utType == USER)
					ssStream << "普通用户";
				else
					ssStream << "管理员";
				ssStream << ")]";
				ssStream.clear();
				system(ssStream.str().c_str());
				return true;
			}
		}
		//最多 3 次密码错误
		if (nCount > 0)
		{
			cout << "\n\n******************************" << endl;
			cout << "帐号不存在或密码错误！\n你还有" << nCount << "次重试机会。" << endl;
			Sleep(2000);
			system("cls");
		}
	}
	error(WRONG_PWD);
	return false;
}
// 密码输入的*处理
bool cin_pwd(char* strPwd, unsigned int nLen)
{
	char chGet = '\0';
	unsigned int i = 0;

	while (1)
	{
		i = 0;
		while (chGet != '\r' && i < nLen)	//密码
		{
			chGet = _getch();
			if (chGet == '\r')
			{
				if (i != 0)
				{
					strPwd[i] = '\0';
					return true; //返回成功
				}
				else	//禁止空密码登录
				{
					chGet = '\0';
					continue;
				}
			}
			if (chGet == ' ' || chGet == '\t') continue;
			if (chGet == 27)	//用户按ESC取消，返回失败
				return false;
			if (chGet == '\b')	//按下退格键
			{
				if (i > 0)
				{
					--i;
					cout << "\b \b"; //退格
				}
				else
				{
					break;
				}
			}
			else
			{
				cout.put('*'); //密码以星号显示
				strPwd[i++] = chGet;
			}

		}
		strPwd[i] = '\0';
		return true; //返回成功
	}
}

// 账号输入的处理
bool cin_id(char* strid, unsigned int nLen)
{
	char chGet = '\0';
	unsigned int i = 0;

	while (1)
	{
		i = 0;
		while (chGet != '\r' && i < nLen)	//账号
		{
			chGet = _getch();
			if (chGet == '\r')
			{
				if (i != 0)
				{
					strid[i] = '\0';
					return true; //返回成功
				}
				else	//禁止空账户
				{
					chGet = '\0';
					continue;
				}
			}
			if (chGet == ' ' || chGet == '\t') continue;
			if (chGet == 27)	//用户按ESC取消，返回失败
				return false;
			if (chGet == '\b')	//按下退格键
			{
				if (i > 0)
				{
					--i;
					cout << "\b \b"; //退格
				}
				else
				{
					break;
				}
			}
			else
			{
				cout.put(chGet); //账户显示
				strid[i++] = chGet;
			}

		}
		strid[i] = '\0';
		return true; //返回成功
	}
}