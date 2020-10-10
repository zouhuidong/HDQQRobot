#include "HDQQRobot.h"

#define SIZE 204800

char msg[SIZE] = { 0 };

int number_1, cnt_1;
bool isin_1;

void ReturnMsg(const char* msg)
{
	string strReturn;

	if (!strcmp(msg, "菜单"))
	{
		strReturn =  "\
输入名称进行相应的操作\r\n\r\n\
1.猜数字\r\n\
2.暂无\r\n\
3.暂无";
	}
	if (!strcmp(msg, "猜数字"))
	{
		if (isin_1)
		{
			isin_1 = false;
			cnt_1 = 0;
			strReturn = "——猜数字游戏结束——";
		}
		srand(time(NULL));
		number_1 = /*rand() % 1000 + 1*/666;
		isin_1 = true;
		strReturn = "\
————猜数字游戏开始————\r\n\r\n\
我先想一个1~1000之间的数字，你\r\n\
来猜。我会说大了、小了或者对了\
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
				strReturn = "大了";
				cnt_1++;
			}
			if (in < number_1)
			{
				strReturn = "小了";
				cnt_1++;
			}
			if (in == number_1)
			{
				cnt_1++;
				char* re;
				re = new char[1024];
				memset(re, 0, sizeof(re));
				sprintf(re, "\
对了!总共猜了%d次      \r\n\r\n\
——猜数字游戏结束——",cnt_1);
				isin_1 = false;
				cnt_1 = 0;
				strReturn = re;
			}
		}
	}
	if (strstr(msg, "草") || strstr(msg, "牛逼") || strstr(msg, "艹") || strstr(msg, "草泥马")
		|| strstr(msg, "WTF") || strstr(msg, "Wtf") || strstr(msg, "wtf")
		|| strstr(msg, "靠") || strstr(msg, "WOC") || strstr(msg, "woc")
		|| strstr(msg, "我去年买了个表") || strstr(msg, "我去") || strstr(msg, "我去你妈的")
		|| strstr(msg, "wdnmd") || strstr(msg, "WC") || strstr(msg, "Wc") || strstr(msg, "wC") || strstr(msg, "wc")
		|| strstr(msg, "卧槽") || strstr(msg, "我操") || strstr(msg, "我草") || strstr(msg, "靠")
		|| strstr(msg, "淦")
		|| strstr(msg, "Fuck") || strstr(msg, "fucK") || strstr(msg, "fuck")
		|| strstr(msg,"F**k") || strstr(msg, "f**K") || strstr(msg, "f**k") || strstr(msg, "F**K")
		|| strstr(msg,"贱"))
	{
		strReturn = "请不要说脏话";
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