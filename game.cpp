#include "game.h"

// 最多同时在线人数
const int m_nMaxOnline = 10;

// 在线的用户名
string m_strOnlineUser[m_nMaxOnline];

// 在线人数
int m_nOnlineNum = 0;

// 玩家出生点
POINT m_pBorn = { 3,3 };

// 玩家位置
POINT m_pPoint[m_nMaxOnline];

// 地图高度
const int m_map_h = 40;

// 地图
string m_map[m_map_h] = {
"########################################",
"#        #                             #",
"##               ##          #         #",
"#     #        #       ###    #        #",
"#         #                            #",
"#                                 #    #",
"#     #      ### ##    #        #      #",
"#     #     #        #    #  #   #     #",
"#     #                           #    #",
"#             #           #       #    #",
"#     #                  # #           #",
"#      #      #         #    #         #",
"#       #           #       ######     #",
"#        #          #         #        #",
"#         #         #          #       #",
"#                                      #",
"#             #               ##       #",
"#                                      #",
"#      #              #                #",
"#     #             ##        ###### # #",
"#    #        #####      #          #  #",
"#   #                             #    #",
"#                               #      #",
"#                                      #",
"#  #     #  #                          #",
"#                           #          #",
"#  #  #     #               #          #",
"#  ####  ####  #            #          #",
"#                           #          #",
"#        #   #  #   #  ###        #    #",
"#                                ###   #",
"#        #   #  #   #   #         # #  #",
"#        #   #  #####  ###           # #",
"#                                     ##",
"#               #            #         #",
"#      #  #  #  #  ## #  #    #        #",
"#      #  #        #     #           # #",
"#      ###    ##   #  #        #       #",
"#                              #       #",
"########################################"
};


// 绘制游戏界面
void DrawGame()
{
	// 方块单位大小
	int nUnitSize = 5;

	for (int y = 0; y < m_map_h; y++)
	{
		for (UINT x = 0; x < m_map[0].size(); x++)
		{
			if (m_map[y][x] == '#')
			{
				setfillcolor(RGB(181, 230, 29));
				setlinecolor(RGB(181, 230, 29));
				fillrectangle(x * nUnitSize, y * nUnitSize, (x + 1) * nUnitSize, (y + 1) * nUnitSize);
			}

			else
			{
				for (int i = 0; i < m_nOnlineNum; i++)
				{
					// 输出人物方块及人物名称
					if (m_pPoint[i].x == x && m_pPoint[i].y == y)
					{
						setfillcolor(BLUE);
						setlinecolor(BLUE);
						fillrectangle(x * nUnitSize, y * nUnitSize, (x + 1) * nUnitSize, (y + 1) * nUnitSize);

						const char* name = m_strOnlineUser[i].c_str();

						wchar_t wch[128] = { 0 };
						MultiByteToWideChar(CP_ACP, 0, name, strlen(name), wch, strlen(name));    //ch转换到wch

						int text_w = textwidth(wch);
						int text_h = textheight(wch);

						settextstyle(2, 0, L"system");
						setbkmode(TRANSPARENT);
						outtextxy(x * nUnitSize - text_w / 2, y * nUnitSize - nUnitSize / 2 - text_h, wch);
					}
				}
			}
		}
	}


}

// 判断一玩家是否在线
// 在线则返回在线id，否则返回-1
int GetOnlineId(string name)
{
	for (int i = 0; i < m_nOnlineNum; i++)
	{
		if (m_strOnlineUser[i] == name)
		{
			return i;
		}
	}

	return -1;
}

// 判断某id是否在线
bool isOnline(int id)
{
	return id <= m_nOnlineNum;
}

// 对某人说他不在线
void SayOfflineTo(string name)
{
	string reply = "@" + name + " 你不在线。";
	QQSendMsg(reply);
}

// 使一玩家的位置到达出生点
void Born(int id)
{
	if (isOnline(id))
		m_pPoint[id] = m_pBorn;
}

// 使某玩家登录游戏
void login(string name)
{
	if (GetOnlineId(name) == -1)
	{
		if (m_nOnlineNum == m_nMaxOnline - 1)
		{
			string reply = "@" + name + " 在线人数已满，请稍后再试。";
			QQSendMsg(reply);
		}
		else
		{
			m_strOnlineUser[m_nOnlineNum] = name;
			Born(m_nOnlineNum);
			m_nOnlineNum++;

			string reply = "@" + name + " 上线成功。";
			QQSendMsg(reply);
		}
	}
	else
	{
		string reply = "@" + name + " 你已经在线！";
		QQSendMsg(reply);
	}
}

// 使某玩家退出游戏
void exit(string name)
{
	string new_list[m_nMaxOnline];
	POINT new_point[m_nMaxOnline];
	int num = 0;
	int id = GetOnlineId(name);

	if (id == -1)
	{
		SayOfflineTo(name);
	}

	else
	{
		for (int i = 0; i < m_nOnlineNum; i++)
		{
			if (i != id)
			{
				new_list[num] = m_strOnlineUser[i];
				new_point[num] = m_pPoint[i];
				num++;
			}
		}

		m_nOnlineNum--;

		for (int i = 0; i < m_nOnlineNum; i++)
		{
			m_strOnlineUser[i] = new_list[i];
			m_pPoint[i] = new_point[i];
		}

		string reply = "@" + name + " 下线成功。";
		QQSendMsg(reply);
	}
}

// 使某人向上走
void Up(int id)
{
	if (!isOnline(id))
	{
		SayOfflineTo(m_strOnlineUser[id]);
		return;
	}

	if (m_map[m_pPoint[id].y - 1][m_pPoint[id].x] != '#')
		m_pPoint[id].y--;
}

// 使某人向下走
void Down(int id)
{
	if (!isOnline(id))
	{
		SayOfflineTo(m_strOnlineUser[id]);
		return;
	}

	if (m_map[m_pPoint[id].y + 1][m_pPoint[id].x] != '#')
		m_pPoint[id].y++;
}

// 使某人向左走
void Left(int id)
{
	if (!isOnline(id))
	{
		SayOfflineTo(m_strOnlineUser[id]);
		return;
	}

	if (m_map[m_pPoint[id].y][m_pPoint[id].x - 1] != '#')
		m_pPoint[id].x--;
}

// 使某人向右走
void Right(int id)
{
	if (!isOnline(id))
	{
		SayOfflineTo(m_strOnlineUser[id]);
		return;
	}

	if (m_map[m_pPoint[id].y][m_pPoint[id].x + 1] != '#')
		m_pPoint[id].x++;
}

// 在QQ中显示出游戏场景
void Show()
{
	cleardevice();
	DrawGame();

	IMAGE* img = GetWorkingImage();
	QQSendMsg(GetImageHBitmap(*img));
}

// 显示在线用户表
void List()
{
	string list;

	list = "在线用户：\n";

	for (int i = 0; i < m_nOnlineNum; i++)
	{
		list += m_strOnlineUser[i] + "\n";
	}

	if (!m_nOnlineNum)
	{
		list += "空";
	}

	QQSendMsg(list);
}

// 使某人杀死最近的人
void KillAround(int id)
{
	if (!isOnline(id))
	{
		SayOfflineTo(m_strOnlineUser[id]);
		return;
	}

	// 是否尝试杀人
	bool isKill = false;

	// 遍历附近的点（范围偏差在1以内）
	for (int h = -1 - rand() % 2; h < 1 + rand() % 2; h++)
	{
		for (int w = -1 - rand() % 2; w < 1 + rand() % 2; w++)
		{
			// 判断坐标合法性
			if (
				m_pPoint[id].x + h > 0 && m_pPoint[id].x + h < (int)m_map[0].size() &&
				m_pPoint[id].y + w > 0 && m_pPoint[id].y + w < m_map_h
				)
			{
				// 判断此坐标是否有人
				for (int i = 0; i < m_nOnlineNum; i++)
				{
					if (i != id && m_pPoint[i].x == m_pPoint[id].x + i && m_pPoint[i].y == m_pPoint[id].y + w)
					{
						// 有人则有 60% 概率击杀
						if (rand() % 10 >= 4)
						{
							string reply = "@" + m_strOnlineUser[i] + " 被 @" + m_strOnlineUser[id] + "击杀死亡。";
							QQSendMsg(reply);
							exit(m_strOnlineUser[i]);
						}
						else
						{
							string reply = "@" + m_strOnlineUser[i] + " 被 @" + m_strOnlineUser[id] + "攻击，但未死亡。";
							QQSendMsg(reply);
						}

						isKill = true;
						goto finish;
					}
				}
			}
		}
	}

finish:

	if (!isKill)
	{
		string reply = "@" + m_strOnlineUser[id] + " 你尝试击杀附近的人，但没有人在攻击范围内。";
		QQSendMsg(reply);
	}
}

// 游戏响应函数
void GameResponse(QQMsg list)
{
	// 得到最后一条消息
	QQMsg msg = *QQMsgGetLast(&list);

	IMAGE img(200, 200);
	SetWorkingImage(&img);
	setbkcolor(WHITE);
	settextcolor(BLACK);

	if (msg.msg == ".help")
	{
		cleardevice();

		QQSendMsg(
			"---help menu---\n"
			"   <指令菜单>\n"
			".login 进入大厅\n"
			".exit  退出大厅\n"
			".w  向前走\n"
			".s  向后走\n"
			".a  向左走\n"
			".d  向右走\n"
			".kill  杀死最近的一个人\n"
			".show  显示游戏场景\n"
			".list  在线用户表\n"
		);
	}

	else if (msg.msg == ".login")
	{
		login(msg.name);
		Show();
	}

	else if (msg.msg == ".exit")
	{
		exit(msg.name);
	}

	else if (msg.msg == ".w")
	{
		Up(GetOnlineId(msg.name));
		Show();
	}

	else if (msg.msg == ".s")
	{
		Down(GetOnlineId(msg.name));
		Show();
	}

	else if (msg.msg == ".a")
	{
		Left(GetOnlineId(msg.name));
		Show();
	}

	else if (msg.msg == ".d")
	{
		Right(GetOnlineId(msg.name));
		Show();
	}

	else if (msg.msg == ".show")
	{
		Show();
	}

	else if (msg.msg == ".list")
	{
		List();
	}

	else if (msg.msg == ".kill")
	{
		KillAround(GetOnlineId(msg.name));
	}


	else
	{
		QQSendMsg("未知指令。输入 .help 获得帮助菜单。");
	}
}
