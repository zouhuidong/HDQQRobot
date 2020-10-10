#include "game.h"

// ���ͬʱ��������
const int m_nMaxOnline = 10;

// ���ߵ��û���
string m_strOnlineUser[m_nMaxOnline];

// ��������
int m_nOnlineNum = 0;

// ��ҳ�����
POINT m_pBorn = { 3,3 };

// ���λ��
POINT m_pPoint[m_nMaxOnline];

// ��ͼ�߶�
const int m_map_h = 40;

// ��ͼ
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


// ������Ϸ����
void DrawGame()
{
	// ���鵥λ��С
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
					// ������﷽�鼰��������
					if (m_pPoint[i].x == x && m_pPoint[i].y == y)
					{
						setfillcolor(BLUE);
						setlinecolor(BLUE);
						fillrectangle(x * nUnitSize, y * nUnitSize, (x + 1) * nUnitSize, (y + 1) * nUnitSize);

						const char* name = m_strOnlineUser[i].c_str();

						wchar_t wch[128] = { 0 };
						MultiByteToWideChar(CP_ACP, 0, name, strlen(name), wch, strlen(name));    //chת����wch

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

// �ж�һ����Ƿ�����
// �����򷵻�����id�����򷵻�-1
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

// �ж�ĳid�Ƿ�����
bool isOnline(int id)
{
	return id <= m_nOnlineNum;
}

// ��ĳ��˵��������
void SayOfflineTo(string name)
{
	string reply = "@" + name + " �㲻���ߡ�";
	QQSendMsg(reply);
}

// ʹһ��ҵ�λ�õ��������
void Born(int id)
{
	if (isOnline(id))
		m_pPoint[id] = m_pBorn;
}

// ʹĳ��ҵ�¼��Ϸ
void login(string name)
{
	if (GetOnlineId(name) == -1)
	{
		if (m_nOnlineNum == m_nMaxOnline - 1)
		{
			string reply = "@" + name + " �����������������Ժ����ԡ�";
			QQSendMsg(reply);
		}
		else
		{
			m_strOnlineUser[m_nOnlineNum] = name;
			Born(m_nOnlineNum);
			m_nOnlineNum++;

			string reply = "@" + name + " ���߳ɹ���";
			QQSendMsg(reply);
		}
	}
	else
	{
		string reply = "@" + name + " ���Ѿ����ߣ�";
		QQSendMsg(reply);
	}
}

// ʹĳ����˳���Ϸ
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

		string reply = "@" + name + " ���߳ɹ���";
		QQSendMsg(reply);
	}
}

// ʹĳ��������
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

// ʹĳ��������
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

// ʹĳ��������
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

// ʹĳ��������
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

// ��QQ����ʾ����Ϸ����
void Show()
{
	cleardevice();
	DrawGame();

	IMAGE* img = GetWorkingImage();
	QQSendMsg(GetImageHBitmap(*img));
}

// ��ʾ�����û���
void List()
{
	string list;

	list = "�����û���\n";

	for (int i = 0; i < m_nOnlineNum; i++)
	{
		list += m_strOnlineUser[i] + "\n";
	}

	if (!m_nOnlineNum)
	{
		list += "��";
	}

	QQSendMsg(list);
}

// ʹĳ��ɱ���������
void KillAround(int id)
{
	if (!isOnline(id))
	{
		SayOfflineTo(m_strOnlineUser[id]);
		return;
	}

	// �Ƿ���ɱ��
	bool isKill = false;

	// ���������ĵ㣨��Χƫ����1���ڣ�
	for (int h = -1 - rand() % 2; h < 1 + rand() % 2; h++)
	{
		for (int w = -1 - rand() % 2; w < 1 + rand() % 2; w++)
		{
			// �ж�����Ϸ���
			if (
				m_pPoint[id].x + h > 0 && m_pPoint[id].x + h < (int)m_map[0].size() &&
				m_pPoint[id].y + w > 0 && m_pPoint[id].y + w < m_map_h
				)
			{
				// �жϴ������Ƿ�����
				for (int i = 0; i < m_nOnlineNum; i++)
				{
					if (i != id && m_pPoint[i].x == m_pPoint[id].x + i && m_pPoint[i].y == m_pPoint[id].y + w)
					{
						// �������� 60% ���ʻ�ɱ
						if (rand() % 10 >= 4)
						{
							string reply = "@" + m_strOnlineUser[i] + " �� @" + m_strOnlineUser[id] + "��ɱ������";
							QQSendMsg(reply);
							exit(m_strOnlineUser[i]);
						}
						else
						{
							string reply = "@" + m_strOnlineUser[i] + " �� @" + m_strOnlineUser[id] + "��������δ������";
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
		string reply = "@" + m_strOnlineUser[id] + " �㳢�Ի�ɱ�������ˣ���û�����ڹ�����Χ�ڡ�";
		QQSendMsg(reply);
	}
}

// ��Ϸ��Ӧ����
void GameResponse(QQMsg list)
{
	// �õ����һ����Ϣ
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
			"   <ָ��˵�>\n"
			".login �������\n"
			".exit  �˳�����\n"
			".w  ��ǰ��\n"
			".s  �����\n"
			".a  ������\n"
			".d  ������\n"
			".kill  ɱ�������һ����\n"
			".show  ��ʾ��Ϸ����\n"
			".list  �����û���\n"
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
		QQSendMsg("δָ֪����� .help ��ð����˵���");
	}
}
