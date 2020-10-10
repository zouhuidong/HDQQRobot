///////////////////////////////////////////
//
//	狼人杀游戏
//	by huidong.
//
//	创建时间：2020.7.14
//	最后修改：2020.8.23
//

#include "woof.h"

////////// player

#define NO_JOB -1	// 未指定
#define MAN 0		// 普通村民
#define WOOF 1		// 狼
#define WITCH 2		// 女巫
#define PROPHET 3	// 预言家
#define GUARD 4		// 守卫

#define MAX_PLAYER_NUM 10	// 最大玩家数

struct PLAYER
{
	int job;	// 职业
	bool isDie;	// 是否死亡
};

int m_nPlayerNum;		// 玩家数量

PLAYER m_players[MAX_PLAYER_NUM];	// 玩家列表
POINT m_cards_xy[MAX_PLAYER_NUM];	// 各个玩家的卡牌所在的位置
bool m_bWillDie[MAX_PLAYER_NUM];	// 玩家列表：是否即将死亡

int m_nPoisonNum = 1;		// 女巫的毒药数量
int m_nAntidoteNum = 1;		// 女巫的解药数量

int m_nMyId;	// 我的玩家序号

int m_look_id = -1;		// 预言家昨晚看到的人的id
int m_look_job = NO_JOB;	// 预言家昨晚看到的人的职业

////////// image

PIMAGE m_img_theme;
PIMAGE m_img_text;
PIMAGE m_img_pause;

PIMAGE m_img_card_back;
PIMAGE m_img_my_card;
PIMAGE m_img_look;
PIMAGE m_img_die;


// 发送当前屏幕图像到QQ
void QQSendScreen()
{
	PIMAGE img = newimage();
	getimage(img, 0, 0, getwidth(), getheight());
	QQSendMsg(GetImageHBitmap(img));
	delimage(img);
}

// 得到发来的消息
QQMsg* QQGetInput()
{
	// 没有新消息就一直等待
	while (!QQGetMsg())
	{
		if (QQIsEnd())
		{
			exit(0);
		}
		Sleep(10);
	}

	return QQGetLastMsg();
}


// 居中放置图片
// img 图像
// offset_x,y 和居中位置的偏移，默认为0
void putimage_center(PIMAGE img, int offset_x = 0, int off_set_y = 0)
{
	putimage(
		(getwidth() - getwidth(img)) / 2 - offset_x,
		(getheight() - getheight(img)) / 2 - off_set_y,
		img
	);
}

// 初始化游戏（资源、变量等）
void InitGame()
{
	m_img_theme = newimage();
	m_img_text = newimage();
	m_img_pause = newimage();
	m_img_card_back = newimage();
	m_img_my_card = newimage();
	m_img_look = newimage();
	m_img_die = newimage();

	getimage(m_img_theme, L"./res/img/theme.jpg");
	getimage(m_img_text, L"./res/img/text.jpg");
	getimage(m_img_pause, L"./res/img/pause.jpg");
	getimage(m_img_card_back, L"./res/img/card_back.jpg");
	getimage(m_img_my_card, L"./res/img/my_card.jpg");
	getimage(m_img_look, L"./res/img/card_look.jpg");
	getimage(m_img_die, L"./res/img/card_die.jpg");

	for (int i = 0; i < MAX_PLAYER_NUM; i++)
	{
		m_players[i].job = NO_JOB;
		m_players[i].isDie = false;
	}

	//m_music.OpenFile(L"./res/sound/bk.mp3");
	//m_music.Play();
}

// 游戏开始界面
void StartScene()
{
	putimage_center(m_img_theme, 0, 100);
	putimage_center(m_img_text, 0, -80);
	putimage_center(m_img_pause, 0, -200);

	QQSendScreen();

	Sleep(100);
}

// 初始化一个全屏窗口
void InitFullScreenWindow()
{
	initgraph(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	//SetWindowLong(getHWnd(), GWL_STYLE, GetWindowLong(getHWnd(), GWL_STYLE) & ~WS_CAPTION);
	//SetWindowPos(getHWnd(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE);

	ShowWindow(getHWnd(), SW_HIDE);    // 隐藏
}

// 在接近屏幕中央处输出文本
void centertext(LPCTSTR str)
{
	// 擦除之前的文字
	setfillcolor(getbkcolor());
	bar(0, getheight() / 2 - 20, getwidth(), getheight() / 2 + 20);

	setfont(36, 0, L"黑体");
	setcolor(EGERGB(190, 190, 190));
	outtextxy(100, getheight() / 2 - 20, str);
}

// 设置人数
void SetPlayerNum()
{
	cleardevice();
	//FlushBatchDraw();

	int num = 0;

	while (true)
	{
		QQSendMsg("输入游戏人数（只能为6-10人）：");
		num = atoi(QQGetInput()->msg.c_str());
		if (num < 6 || num > 18)
		{
			QQSendMsg("人数必须在指定范围内。");
		}
		else
		{
			break;
		}
	}

	m_nPlayerNum = num;

	// 各职业应有的人物数量
	int job_max[5] = { 0 };

	// 设置每个角色的最大数量
	switch (m_nPlayerNum)
	{
	case 6:
		job_max[MAN] = 2;
		job_max[WITCH] = 1;
		job_max[PROPHET] = 1;
		job_max[GUARD] = 0;
		job_max[WOOF] = 2;
		break;

	case 7:
		job_max[MAN] = 3;
		job_max[WITCH] = 1;
		job_max[PROPHET] = 1;
		job_max[GUARD] = 0;
		job_max[WOOF] = 2;
		break;

	case 8:
		job_max[MAN] = 2;
		job_max[WITCH] = 1;
		job_max[PROPHET] = 1;
		job_max[GUARD] = 1;
		job_max[WOOF] = 3;
		break;

	case 9:
		job_max[MAN] = 3;
		job_max[WITCH] = 1;
		job_max[PROPHET] = 1;
		job_max[GUARD] = 1;
		job_max[WOOF] = 3;
		break;

	case 10:
		job_max[MAN] = 4;
		job_max[WITCH] = 1;
		job_max[PROPHET] = 1;
		job_max[GUARD] = 1;
		job_max[WOOF] = 3;
		break;
	}


	// 各职业数量
	int job_nums[5] = { 0 };

	// 给每个人物分配角色
	for (int i = 0; i < m_nPlayerNum; i++)
	{
		// 分配角色
		while (true)
		{
			int job = rand() % 5;

			// 该人物已满，重新分配
			if (job_nums[job] >= job_max[job])
			{
				continue;
			}
			else
			{
				// 分配成功
				job_nums[job]++;	// 累计该职业人物数量
				m_players[i].job = job;
				break;
			}
		}
	}

	while (true)
	{
		// 随机给我分配一个id
		m_nMyId = rand() % m_nPlayerNum;

		// 如果被选为平民，有一半几率可以重选
		if (m_players[m_nMyId].job == MAN)
		{
			if (rand() % 10 > 4)
			{
				continue;
			}
		}

		break;
	}
}

// 判断某人是否已死
bool isDie(int id)
{
	return m_players[id].isDie;
}

// 在某人的卡牌上输出他的职业
void PrintJobOnHisCard(int id)
{
	TCHAR job[64] = { 0 };

	switch (m_players[id].job)
	{
	case MAN:
		wsprintf(job, L"平民");
		break;

	case WOOF:
		wsprintf(job, L"狼人");
		break;

	case WITCH:
		wsprintf(job, L"女巫");
		break;

	case PROPHET:
		wsprintf(job, L"预言家");
		break;

	case GUARD:
		wsprintf(job, L"守卫");
		break;
	}

	setfont(24, 0, L"黑体");
	setcolor(BLACK);
	outtextxy(m_cards_xy[id].x + 50, m_cards_xy[id].y + 150, job);
}

// 绘制棋桌和卡牌
void DrawTableAndCard()
{
	// 棋桌
	cleardevice();

	// 卡牌输出的xy坐标
	int x, y;
	int up_card_num;// 在上面的卡牌的数量

	up_card_num = m_nPlayerNum / 2;

	// 放置每个玩家的卡牌
	for (int i = 0; i < m_nPlayerNum; i++)
	{
		if (i <= up_card_num)
		{
			x = i * (getwidth(m_img_card_back) + 10) + 10;
			y = 10;
		}
		else
		{
			x = (i - up_card_num - 1) * (getwidth(m_img_card_back) + 10) + 10;
			y = getheight() - getheight(m_img_card_back) - 10;

		}

		// 记录卡牌位置
		m_cards_xy[i] = { x,y };

		if (isDie(i))
		{
			putimage(x, y, m_img_die);
			PrintJobOnHisCard(i);
		}
		else
		{
			if (i == m_nMyId)
			{
				putimage(x, y, m_img_my_card);
				PrintJobOnHisCard(i);
			}
			else
			{
				putimage(x, y, m_img_card_back);
			}
		}
	}
}

// 得到用户点击的卡牌对应的Id（阻塞）
int GetClickId()
{
	QQSendMsg("请输入目标玩家id：");

	while (true)
	{
		string msg = QQGetInput()->msg;
		int num = atoi(msg.c_str());
		num --;

		if (num < 0 || num > m_nPlayerNum)
		{
			QQSendMsg("ID错误，请重新输入");
			continue;
		}

		else
		{
			return num;
		}
	}
}

// 杀死某人（添加某人到将死名单）
void Kill(int id)
{
	m_bWillDie[id] = true;
}

// 拯救一个人（不管他是不是将死的）
void Save(int id)
{
	m_bWillDie[id] = false;
}

// 清除将死名单（并进行处理）
void ClearWillDieList()
{
	for (int i = 0; i < m_nPlayerNum; i++)
	{
		if (m_bWillDie[i])
		{
			m_players[i].isDie = true;
		}

		m_bWillDie[i] = false;
	}
}

// 狼
void woof(int id)
{
	// 如果我也是狼，则显示其它的狼
	if (m_players[m_nMyId].job == WOOF)
	{
		for (int i = 0; i < m_nPlayerNum; i++)
		{
			if (m_players[i].job == WOOF && !isDie(i) && i != m_nMyId)
			{
				putimage(m_cards_xy[i].x, m_cards_xy[i].y, m_img_look);
			}
		}
	}

	if (id == m_nMyId)
	{
		centertext(L"狼人请睁眼……");
		QQSendScreen();
		Sleep(500);

		if (!(rand() % 4))
		{
			QQSendMsg("你今夜不可杀人。");
		}

		else if (QQMessageBox("你要杀人吗？"))
		{
			centertext(L"请点击一个玩家的卡牌作为攻击目标");
			QQSendScreen();

			while (true)
			{
				int index = GetClickId();
				if (index == id || isDie(index))
				{
					QQSendMsg("此玩家不可作为目标，请重选");
				}
				else
				{
					Kill(index);
					break;
				}
			}
		}

		centertext(L"狼人请闭眼……");
		QQSendScreen();
	}
	else
	{
		// 30% 概率不杀
		while (rand() % 11 > 3)
		{
			int index = rand() % m_nPlayerNum;

			if (!(index == id || isDie(index) || m_players[index].job == WOOF))
			{
				if (index == m_nMyId && !(rand() % 4))
				{
					continue;
				}

				Kill(index);
				break;
			}
		}
	}
}

// 女巫
void witch(int id)
{
	if (id == m_nMyId)
	{
		centertext(L"女巫请睁眼……");
		QQSendScreen();
		Sleep(500);

		// 女巫要用什么药水？
		// 0表示什么也没用，1表示毒药，2表示解药
		int use = 0;

		if (m_nPoisonNum && QQMessageBox("女巫，你要使用毒药吗？"))
		{
			use = 1;
		}
		else if (m_nAntidoteNum && QQMessageBox("那么女巫，你要使用解药吗？"))
		{
			use = 2;
		}

		if (use)
		{
			centertext(L"请点击一个玩家的卡牌作为目标");
			QQSendScreen();

			while (true)
			{
				int index = GetClickId();
				if (isDie(index))
				{
					QQSendMsg("此玩家不可作为目标，请重选");
				}
				else
				{
					if (use == 1)
					{
						Kill(index);
						m_nPoisonNum--;
					}
					else if (use == 2)
					{
						Save(index);
						m_nAntidoteNum--;
					}

					break;
				}
			}
		}

		centertext(L"女巫请闭眼……");
		QQSendScreen();
	}
	else
	{
		if (rand() % 2)
		{
			return;
		}

		while (true)
		{
			int index = rand() % m_nPlayerNum;
			int use = rand() % 3;

			if (!(index == id || isDie(index)))
			{
				if (use == 1)
				{
					if (m_nPoisonNum)
					{
						// 非狼人有一半概率不杀
						if (m_players[index].job != WOOF && rand() % 2)
						{
							continue;
						}

						Kill(index);
						m_nPoisonNum--;
						break;
					}
				}
				else if (use == 2)
				{
					if (m_nAntidoteNum)
					{
						// 遇到狼人有一半概率不保
						if (m_players[index].job == WOOF && rand() % 2)
						{
							continue;
						}

						Save(index);
						m_nAntidoteNum--;
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
	}
}

// 预言家
void prophet(int id)
{
	if (id == m_nMyId)
	{
		centertext(L"预言家请睁眼……");
		QQSendScreen();
		Sleep(500);

		if (QQMessageBox("预言家，你要看某人的身份吗？"))
		{
			centertext(L"请点击一个玩家的卡牌作为目标");
			QQSendScreen();

			while (true)
			{
				int index = GetClickId();
				if (isDie(index))
				{
					QQSendMsg("此玩家不可作为目标，请重选");
				}
				else
				{
					PrintJobOnHisCard(index);
					QQSendScreen();
					Sleep(1000);

					break;
				}
			}
		}

		centertext(L"预言家请闭眼……");
		QQSendScreen();
	}
	else
	{
		if (rand() % 2)
		{
			while (true)
			{
				int index = rand() % m_nPlayerNum;
				if (!(index == id || isDie(index)))
				{
					m_look_id = index;
					m_look_job = m_players[index].job;
					break;
				}
			}
		}
	}
}

// 守卫
void guard(int id)
{
	if (id == m_nMyId)
	{
		centertext(L"守卫请睁眼……");
		QQSendScreen();
		Sleep(500);

		if (QQMessageBox("守卫，你希望保护一个人吗？"))
		{
			centertext(L"请点击一个玩家的卡牌作为目标");
			QQSendScreen();

			while (true)
			{
				int index = GetClickId();
				if (isDie(index))
				{
					QQSendMsg("此玩家不可作为目标，请重选");
				}
				else
				{
					Save(index);

					break;
				}
			}
		}

		centertext(L"守卫请闭眼……");
		QQSendScreen();
	}
	else
	{
		if (rand() % 2)
		{
			return;
		}

		while (true)
		{
			int index = rand() % m_nPlayerNum;

			if (!(index == id || isDie(index)))
			{
				// 遇到狼人有一半概率不保
				if (m_players[index].job == WOOF && rand() % 2)
				{
					continue;
				}

				Save(index);
				break;
			}
		}
	}
}

// 展示所有人的身份
void ShowAll()
{
	for (int i = 0; i < m_nPlayerNum; i++)
	{
		if (!isDie(i))
		{
			PrintJobOnHisCard(i);
		}
	}
}

// 得到针对某人的话
TCHAR* GetSay(int id, int index)
{
	// 说话格式
	int mode = rand() % 3;

	TCHAR* strMsg = new TCHAR[64];
	memset(strMsg, 0, 64);

	switch (mode)
	{
	case 0:
		wsprintf(strMsg, L"%d号说：%d号是狼吧？", id, index);
		break;
	case 1:
		wsprintf(strMsg, L"%d号说：我觉得%d号应该有问题。", id, index);
		break;
	case 2:
		wsprintf(strMsg, L"%d号说：%d号绝对有问题，投他！", id, index);
		break;
	}

	return strMsg;
}

// 得到数组中票数最高的一个或多个id
void GetHighestVotes(int* pVotes, int pNum, int* p_pOut, int* p_nOutNum)
{
	int high = 0;
	int out_num = 0;
	int* out = new int[pNum];
	memset(out, 0, pNum);

	for (int i = 0; i < pNum; i++)
	{
		if (pVotes[i] > high)
		{
			high = pVotes[i];
			out_num = 1;
			memset(out, 0, pNum);
			out[out_num - 1] = i;
		}
		else if (pVotes[i] == high)
		{
			out_num++;
			out[out_num - 1] = i;
		}
	}

	memcpy(p_pOut, out, pNum);
	*p_nOutNum = out_num;

	delete[] out;
}

// 判断是否胜利，返回0表示无，1表示平手，2表示正方胜利，3表示狼人胜利
int isWin(int woof_num, int die_num)
{
	if (woof_num)
	{
		if (die_num == m_nPlayerNum)
		{
			centertext(L"所有人都死了，双方平手。");
			return 1;
		}
		else if (m_nPlayerNum - die_num == woof_num)
		{
			centertext(L"正方已死光，狼人胜利！");
			return 3;
		}
	}
	else
	{
		centertext(L"狼人已死光，正方胜利！");
		return 2;
	}

	return 0;
}

// 统计各类人物数量
void GetPeopleNum(int& die_num, int& save_num, int& woof_num)
{
	// 计算死亡、存活人数
	for (int i = 0; i < m_nPlayerNum; i++)
	{
		if (isDie(i))
		{
			die_num++;
			save_num--;
		}

		// 统计存活狼数
		else
		{
			if (m_players[i].job == WOOF)
			{
				woof_num++;
			}
		}
	}
}

// 游戏中
void Game()
{
	setfont(20, 0, L"黑体");
	setbkmode(TRANSPARENT);

	// 棋桌背景色
	setbkcolor(EGERGB(14, 65, 46));

	// 天数
	int day_num = 1;

	// 过场时间(ms)
	int wait_time = 5000;

	// 前一天的死亡人数
	int old_die = 0;

	while (true)
	{
		if (QQIsEnd())
		{
			exit(0);
		}

		DrawTableAndCard();

		// 总死亡人数
		int die_num = 0;

		// 总存活人数
		int save_num = m_nPlayerNum;

		// 存活的狼数
		int woof_num = 0;

		GetPeopleNum(die_num, save_num, woof_num);

		// 白天
		if (day_num % 2 == 0)
		{
			centertext(L"天亮了。");
			QQSendScreen();
			Sleep(2000);

			TCHAR last_light[128] = { 0 };

			int die = die_num - old_die;
			old_die = die_num;

			if (!die)
			{
				wsprintf(last_light, L"昨晚是个平安夜。");
			}
			else
			{
				wsprintf(last_light, L"昨晚死了%d个人", die);
			}

			Sleep(2000);
			centertext(last_light);
			QQSendScreen();

			// 判断胜利 
			if (isWin(woof_num, die_num))
			{
				break;
			}

			Sleep(4000);


			// 第一天之后进行发言、投票
			if (day_num != 2)
			{
				// 逐个发言
				for (int i = 0; i < m_nPlayerNum; i++)
				{
					// 我和死人都无需发言。
					if (i == m_nMyId || isDie(i))
					{
						continue;
					}

					// 狼人 50%概率 发言
					else if (m_players[i].job == WOOF && rand() % 2)
					{
						int index = rand() % m_nPlayerNum;

						if (index != i && !isDie(index) && m_players[index].job != WOOF)
						{
							centertext(GetSay(i + 1, index + 1));
							QQSendScreen();
							Sleep(3000);
						}
					}

					// 预言家如果看到狼一定要发言的
					else if (m_players[i].job == PROPHET)
					{
						if (!isDie(m_look_id) && m_look_job == WOOF)
						{
							centertext(GetSay(i + 1, m_look_id + 1));
							QQSendScreen();
							Sleep(3000);
						}
					}

					// 有时其他人也乱说
					else if (rand() % 5 > 2)
					{
						int index = rand() % m_nPlayerNum;

						if (index != i && !isDie(index))
						{
							centertext(GetSay(i + 1, index + 1));
							QQSendScreen();
							Sleep(3000);
						}
					}
				}

				centertext(L"现在进行投票，票数最多的一人被判处死亡。");
				QQSendScreen();
				Sleep(3000);

				// 记录每人的票数
				int votes[MAX_PLAYER_NUM] = { 0 };

				// 投票环节
				for (int i = 0; i < m_nPlayerNum; i++)
				{
					if (i == m_nMyId)
					{
						continue;
					}

					else if (!isDie(i))
					{
						while (true)
						{
							int index = rand() % m_nPlayerNum;

							if (isDie(index))
							{
								continue;
							}

							else if (m_players[i].job == WOOF)
							{
								if (m_players[index].job == WOOF)
								{
									continue;
								}
								else
								{
									votes[index]++;
									break;
								}
							}

							else
							{
								// 其他人更倾向于投狼、预言家看到的狼
								if (m_players[index].job != WOOF && rand() % 2)
								{
									continue;
								}
								else
								{
									if (m_look_job == WOOF && !isDie(m_look_id) && m_players[index].job != WOOF && rand() % 2)
									{
										continue;
									}

									votes[index]++;
									break;
								}
							}
						}
					}
				}

				setcolor(BLACK);

				// 展示投票结果
				for (int i = 0; i < m_nPlayerNum; i++)
				{
					TCHAR strNum[12] = { 0 };
					wsprintf(strNum, L"%d票", votes[i]);
					outtextxy(m_cards_xy[i].x + 25, m_cards_xy[i].y + 25, strNum);
				}
				QQSendScreen();
				Sleep(6000);

				// 轮到我投票
				if (!isDie(m_nMyId) && QQMessageBox("你要投票吗？点击取消则表示弃权。"))
				{
					centertext(L"请点击一个玩家的卡牌作为投票目标。");
					QQSendScreen();

					while (true)
					{
						int index = GetClickId();

						if (isDie(index) || index == m_nMyId)
						{
							centertext(L"此玩家不可作为投票目标，请重选。");
							QQSendScreen();
						}
						else
						{
							votes[index] ++;

							TCHAR msg[64];
							wsprintf(msg, L"你投了%d号一票，现在他是%d票", index + 1, votes[index]);
							centertext(msg);
							QQSendScreen();
							Sleep(2000);
							break;
						}
					}
				}

				// 统计得出票数最高者
				int highest[MAX_PLAYER_NUM] = { 0 };
				int num = 0;
				GetHighestVotes(votes, m_nPlayerNum, highest, &num);

				// 判处词
				TCHAR msg[64] = { 0 };

				// 杀死的票最高者（可能是多个）
				for (int i = 0; i < num; i++)
				{
					wsprintf(msg, L"%ls%d号，", msg, highest[i] + 1);
					Kill(highest[i]);

					// 这里死的也要加入旧的死亡名单，否则在明天显示死亡人数的时候会出现错误
					old_die++;
				}

				wsprintf(msg, L"%ls被判处死亡。", msg);
				ClearWillDieList();
				DrawTableAndCard();
				centertext(msg);
				QQSendScreen();
				Sleep(5000);

				GetPeopleNum(die_num, save_num, woof_num);

				// 判断胜利 
				if (isWin(woof_num, die_num))
				{
					break;
				}
			}
		}
		// 黑天
		else
		{
			centertext(L"天黑请闭眼……");
			QQSendScreen();
			Sleep(wait_time);
			
			// 第一夜是平安夜
			if (day_num != 1)
			{
				for (int i = 0; i < m_nPlayerNum; i++)
				{
					if (m_players[i].isDie)
					{
						continue;
					}

					switch (m_players[i].job)
					{
					case WOOF:
						woof(i);
						break;

					case PROPHET:
						prophet(i);
						break;

					case GUARD:
						guard(i);
						break;

					case WITCH:
						witch(i);
						break;
					}
				}

				// 清理昨晚的将死名单
				ClearWillDieList();
			}
		}

		day_num++;

		Sleep(20);
	}

	Sleep(2000);

	ShowAll();
	centertext(L"游戏结束！");
	QQSendScreen();
}


void woof_game(string your_name)
{
	InitFullScreenWindow();
	setcaption(L"Simple Version - Woof killer game");

	srand((unsigned)time(NULL));
	InitGame();

	QQSetMyName(your_name);
	QQSetIsGetMyMsg(false);

	StartScene();

	SetPlayerNum();
	Game();

	closegraph();
}
