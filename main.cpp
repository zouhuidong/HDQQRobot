#include "HDQQRobot.h"

#define SIZE 204800

char msg[SIZE] = { 0 };

int number_1, cnt_1;
bool isin_1;

void ReturnMsg(const char* msg)
{
	string strReturn;

	if (!strcmp(msg, "�˵�"))
	{
		strReturn =  "\
�������ƽ�����Ӧ�Ĳ���\r\n\r\n\
1.������\r\n\
2.����\r\n\
3.����";
	}
	if (!strcmp(msg, "������"))
	{
		if (isin_1)
		{
			isin_1 = false;
			cnt_1 = 0;
			strReturn = "������������Ϸ��������";
		}
		srand(time(NULL));
		number_1 = /*rand() % 1000 + 1*/666;
		isin_1 = true;
		strReturn = "\
����������������Ϸ��ʼ��������\r\n\r\n\
������һ��1~1000֮������֣���\r\n\
���¡��һ�˵���ˡ�С�˻��߶���\
";
	}
	if (isin_1)
	{
		int in = 0, cnt = 1, length = strlen(msg);
		bool isnumber = true;
		for (int i = length - 1; i >= 0; i--)
		{
			if (!(msg[i] >= '0' && msg[i] <= '9'))
			{
				isnumber = false;
				break;
			}
			in += (msg[i] - '0') * cnt;
			cnt *= 10;
		}
		if (isnumber)
		{
			if (in > number_1)
			{
				strReturn = "����";
				cnt_1++;
			}
			if (in < number_1)
			{
				strReturn = "С��";
				cnt_1++;
			}
			if (in == number_1)
			{
				cnt_1++;
				char* re;
				re = new char[1024];
				memset(re, 0, sizeof(re));
				sprintf(re, "\
����!�ܹ�����%d��      \r\n\r\n\
������������Ϸ��������",cnt_1);
				isin_1 = false;
				cnt_1 = 0;
				strReturn = re;
			}
		}
	}
	if (strstr(msg, "��") || strstr(msg, "ţ��") || strstr(msg, "ܳ") || strstr(msg, "������")
		|| strstr(msg, "WTF") || strstr(msg, "Wtf") || strstr(msg, "wtf")
		|| strstr(msg, "��") || strstr(msg, "WOC") || strstr(msg, "woc")
		|| strstr(msg, "��ȥ�����˸���") || strstr(msg, "��ȥ") || strstr(msg, "��ȥ�����")
		|| strstr(msg, "wdnmd") || strstr(msg, "WC") || strstr(msg, "Wc") || strstr(msg, "wC") || strstr(msg, "wc")
		|| strstr(msg, "�Բ�") || strstr(msg, "�Ҳ�") || strstr(msg, "�Ҳ�") || strstr(msg, "��")
		|| strstr(msg, "��")
		|| strstr(msg, "Fuck") || strstr(msg, "fucK") || strstr(msg, "fuck")
		|| strstr(msg,"F**k") || strstr(msg, "f**K") || strstr(msg, "f**k") || strstr(msg, "F**K")
		|| strstr(msg,"��"))
	{
		strReturn = "�벻Ҫ˵�໰";
	}
	
	HDQQSendMsg(strReturn.c_str());
}

int jiec(int n)
{
	if (n == 1)
	{
		return 1;
	}
	return n * jiec(n - 1);
}

int main()
{
	HDQQStartMenu();
	
	HDQQSetResponseFunc(ReturnMsg);

	while (1)
	{
		HDQQGetMsg(msg, SIZE);

		HDQQLexMessage(msg);

		HDQQIsPause();

		if (HDQQIsEnd())
			break;
	}

	return 0;
}