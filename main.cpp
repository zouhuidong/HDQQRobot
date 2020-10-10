#include "HDQQRobot.h"

char map[6][11] = {

"##########",
"# # #    #",
"#     ####",
"#  # #   #",
"## #   #  ",
"##########"

};

int born_x = 1;
int born_y = 1;

int x = born_x;
int y = born_y;

int win_x = 9;
int win_y = 4;

bool isPlay = false;


void PrintMap()
{
	string str;

	// y
	for (int i = 0; i < 6; i++)
	{
		// x
		for (int j = 0; j < 10; j++)
		{
			if (i == y && j == x)
			{
				str += '@';
			}
			else if(map[i][j] == ' ')
			{
				str += "  ";
			}
			else
			{
				str += map[i][j];
			}
		}
		str += '\n';
	}

	HDQQSendMsg(str.c_str());

	// ʤ���ж�
	if (x == win_x && y == win_y)
	{
		HDQQSendMsg("��ͨ����~~��");
		HDQQSendMsg("�Թ���Ϸ����������huidong");
	}
}

void Born()
{
	x = born_x;
	y = born_y;
}

void ResponseMsg(const char* msg)
{
	string strMsg = msg;
	string strReply;

	if (strMsg == "awa")
	{
		HDQQSendMsg("ι����110��������������");
	}

	// �Թ�
	else if (strMsg == "�Թ�")
	{
		if (isPlay)
		{
			HDQQSendMsg("�Թ���Ϸ�رգ�");
			Born();
		}
		else
		{
			HDQQSendMsg("�Թ���Ϸ������\n�ٴ�����\"�Թ�\"���ر���Ϸ��\n�����������������ƽ�ɫ�ƶ���\n��Ŀ��ģ�\n����huidong");

			PrintMap();
		}

		isPlay = !isPlay;
	}

	if (isPlay)
	{
		char Wall[] = "ѽ����������ô�ǽ�ڰ���������~ �Ǿ����¿�ʼ�ÿ���";

		if (strMsg == "��")
		{
			if (map[y - 1][x] == ' ')
			{
				y--;
			}
			else
			{
				HDQQSendMsg(Wall);
				Born();
			}
			PrintMap();
		}
		else if (strMsg == "��")
		{
			if (map[y + 1][x] == ' ')
			{
				y++;
			}
			else
			{
				HDQQSendMsg(Wall);
				Born();
			}
			PrintMap();
		}
		else if (strMsg == "��")
		{
			if (map[y][x - 1] == ' ')
			{
				x--;
			}
			else
			{
				HDQQSendMsg(Wall);
				Born();
			}
			PrintMap();
		}
		else if (strMsg == "��")
		{
			if (map[y][x + 1] == ' ')
			{
				x++;
			}
			else
			{
				HDQQSendMsg(Wall);
				Born();
			}
			PrintMap();
		}
	}

}


int main()
{
	HDQQSetResponseFunc(ResponseMsg);
	HDQQStartMenu();

	const int size = 1024000;
	char msg[size] = { 0 };

	while (true)
	{
		if (HDQQIsEnd())
		{
			break;
		}

		HDQQGetMsg(msg, size);
		HDQQLexMessage(msg);
	}

	return 0;
}
