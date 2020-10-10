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

void Maze_PrintMap()
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
			else if (map[i][j] == ' ')
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

	// 胜利判断
	if (x == win_x && y == win_y)
	{
		HDQQSendMsg("你通关啦~~！");
		HDQQSendMsg("迷宫游戏——制作：huidong");
	}
}

void Maze_Born()
{
	x = born_x;
	y = born_y;
}


void Maze_Init()
{
	HDQQSendMsg("赛马游戏开启！\n再次输入\"赛马\"来关闭游戏。\n输入上下左右来控制角色移动。\n玩的开心！\n——huidong");
	Maze_Born();
	Maze_PrintMap();
}


void Maze_ResponseMsg(const char* msg)
{
	string strMsg = msg;
	string strReply;

	char Wall[] = "呀，你这人怎么怼墙壁啊，讨厌啦~ 那就重新开始好咯。";

	if (strMsg == "上")
	{
		if (map[y - 1][x] == ' ')
		{
			y--;
		}
		else
		{
			HDQQSendMsg(Wall);
			Maze_Born();
		}
		Maze_PrintMap();
	}
	else if (strMsg == "下")
	{
		if (map[y + 1][x] == ' ')
		{
			y++;
		}
		else
		{
			HDQQSendMsg(Wall);
			Maze_Born();
		}
		Maze_PrintMap();
	}
	else if (strMsg == "左")
	{
		if (map[y][x - 1] == ' ')
		{
			x--;
		}
		else
		{
			HDQQSendMsg(Wall);
			Maze_Born();
		}
		Maze_PrintMap();
	}
	else if (strMsg == "右")
	{
		if (map[y][x + 1] == ' ')
		{
			x++;
		}
		else
		{
			HDQQSendMsg(Wall);
			Maze_Born();
		}
		Maze_PrintMap();
	}
}