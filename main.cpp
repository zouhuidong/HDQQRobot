#include "HDQQRobot.h"
#include "maze.h"
#include "DirtyString.h"

// 机器人模式定义

#define MODE_NULL 0
#define MODE_MAZE 1
#define MODE_OTHER 2

int m_mode = MODE_NULL;

// 消息响应函数
void ResponseMsg(const char* msg)
{
	string strMsg = msg;

	// 判断消息并进行处理
	
	if (strMsg == "awa")
	{
		HDQQSendMsg("喂，是110吗？这里有人卖萌");
	}

	// 屏蔽脏话
	if (isDirtyString(msg))
	{
		HDQQSendMsg("请不要说脏话");
	}

	// 迷宫
	else if (strMsg == "迷宫")
	{
		if (m_mode == MODE_MAZE)
		{
			HDQQSendMsg("迷宫游戏关闭！");
			m_mode = MODE_NULL;
		}
		else if(m_mode == MODE_NULL)
		{
			HDQQSendMsg("迷宫游戏开启！\n再次输入\"迷宫\"来关闭游戏。\n输入上下左右来控制角色移动。\n玩的开心！\n——huidong");
			m_mode = MODE_MAZE;
			Maze_Init();
		}
	}

	// 其它模式（此处是例子，处理代码未写）
	else if (strMsg == "其它")
	{
		if (m_mode == MODE_OTHER)
		{
			// 发送响应消息
			//HDQQSendMsg("其它……");
			m_mode = MODE_NULL;
		}
		else if (m_mode == MODE_NULL)
		{
			m_mode = MODE_OTHER;
			
			// 处理代码：
		}
	}

	// 当前模式判断，并处理消息模块
	else
	{
		switch (m_mode)
		{
		case MODE_MAZE:
			Maze_ResponseMsg(msg);	// 迷宫模式下，交由迷宫处理函数
			break;

		case MODE_OTHER:
			// 调用其它模式的消息响应函数
			break;
		}
	}
}


int main()
{
	// 设置消息响应函数
	HDQQSetResponseFunc(ResponseMsg);
	// 使用预定义的启动菜单
	HDQQStartMenu();

	const int size = 1024000;
	char msg[size] = { 0 };

	while (true)
	{	
		// 使用预定义的函数判断机器人是否需要结束
		if (HDQQIsEnd())
		{
			break;
		}

		// 使用预定义的函数判断机器人是否需要暂停
		HDQQIsPause();

		// 得到一条QQ消息
		HDQQGetMsg(msg, size);

		// 分析此QQ消息并发送到响应函数
		HDQQLexMessage(msg);
	}

	return 0;
}
