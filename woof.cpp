///////////////////////////////////////////
//
//	����ɱ��Ϸ
//	by huidong.
//
//	����ʱ�䣺2020.7.14
//	����޸ģ�2020.8.23
//

#include "woof.h"

////////// player

#define NO_JOB -1	// δָ��
#define MAN 0		// ��ͨ����
#define WOOF 1		// ��
#define WITCH 2		// Ů��
#define PROPHET 3	// Ԥ�Լ�
#define GUARD 4		// ����

#define MAX_PLAYER_NUM 10	// ��������

struct PLAYER
{
	int job;	// ְҵ
	bool isDie;	// �Ƿ�����
};

int m_nPlayerNum;		// �������

PLAYER m_players[MAX_PLAYER_NUM];	// ����б�
POINT m_cards_xy[MAX_PLAYER_NUM];	// ������ҵĿ������ڵ�λ��
bool m_bWillDie[MAX_PLAYER_NUM];	// ����б��Ƿ񼴽�����

int m_nPoisonNum = 1;		// Ů�׵Ķ�ҩ����
int m_nAntidoteNum = 1;		// Ů�׵Ľ�ҩ����

int m_nMyId;	// �ҵ�������

int m_look_id = -1;		// Ԥ�Լ����������˵�id
int m_look_job = NO_JOB;	// Ԥ�Լ����������˵�ְҵ

////////// image

PIMAGE m_img_theme;
PIMAGE m_img_text;
PIMAGE m_img_pause;

PIMAGE m_img_card_back;
PIMAGE m_img_my_card;
PIMAGE m_img_look;
PIMAGE m_img_die;


// ���͵�ǰ��Ļͼ��QQ
void QQSendScreen()
{
	PIMAGE img = newimage();
	getimage(img, 0, 0, getwidth(), getheight());
	QQSendMsg(GetImageHBitmap(img));
	delimage(img);
}

// �õ���������Ϣ
QQMsg* QQGetInput()
{
	// û������Ϣ��һֱ�ȴ�
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


// ���з���ͼƬ
// img ͼ��
// offset_x,y �;���λ�õ�ƫ�ƣ�Ĭ��Ϊ0
void putimage_center(PIMAGE img, int offset_x = 0, int off_set_y = 0)
{
	putimage(
		(getwidth() - getwidth(img)) / 2 - offset_x,
		(getheight() - getheight(img)) / 2 - off_set_y,
		img
	);
}

// ��ʼ����Ϸ����Դ�������ȣ�
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

// ��Ϸ��ʼ����
void StartScene()
{
	putimage_center(m_img_theme, 0, 100);
	putimage_center(m_img_text, 0, -80);
	putimage_center(m_img_pause, 0, -200);

	QQSendScreen();

	Sleep(100);
}

// ��ʼ��һ��ȫ������
void InitFullScreenWindow()
{
	initgraph(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	//SetWindowLong(getHWnd(), GWL_STYLE, GetWindowLong(getHWnd(), GWL_STYLE) & ~WS_CAPTION);
	//SetWindowPos(getHWnd(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE);

	ShowWindow(getHWnd(), SW_HIDE);    // ����
}

// �ڽӽ���Ļ���봦����ı�
void centertext(LPCTSTR str)
{
	// ����֮ǰ������
	setfillcolor(getbkcolor());
	bar(0, getheight() / 2 - 20, getwidth(), getheight() / 2 + 20);

	setfont(36, 0, L"����");
	setcolor(EGERGB(190, 190, 190));
	outtextxy(100, getheight() / 2 - 20, str);
}

// ��������
void SetPlayerNum()
{
	cleardevice();
	//FlushBatchDraw();

	int num = 0;

	while (true)
	{
		QQSendMsg("������Ϸ������ֻ��Ϊ6-10�ˣ���");
		num = atoi(QQGetInput()->msg.c_str());
		if (num < 6 || num > 18)
		{
			QQSendMsg("����������ָ����Χ�ڡ�");
		}
		else
		{
			break;
		}
	}

	m_nPlayerNum = num;

	// ��ְҵӦ�е���������
	int job_max[5] = { 0 };

	// ����ÿ����ɫ���������
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


	// ��ְҵ����
	int job_nums[5] = { 0 };

	// ��ÿ����������ɫ
	for (int i = 0; i < m_nPlayerNum; i++)
	{
		// �����ɫ
		while (true)
		{
			int job = rand() % 5;

			// ���������������·���
			if (job_nums[job] >= job_max[job])
			{
				continue;
			}
			else
			{
				// ����ɹ�
				job_nums[job]++;	// �ۼƸ�ְҵ��������
				m_players[i].job = job;
				break;
			}
		}
	}

	while (true)
	{
		// ������ҷ���һ��id
		m_nMyId = rand() % m_nPlayerNum;

		// �����ѡΪƽ����һ�뼸�ʿ�����ѡ
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

// �ж�ĳ���Ƿ�����
bool isDie(int id)
{
	return m_players[id].isDie;
}

// ��ĳ�˵Ŀ������������ְҵ
void PrintJobOnHisCard(int id)
{
	TCHAR job[64] = { 0 };

	switch (m_players[id].job)
	{
	case MAN:
		wsprintf(job, L"ƽ��");
		break;

	case WOOF:
		wsprintf(job, L"����");
		break;

	case WITCH:
		wsprintf(job, L"Ů��");
		break;

	case PROPHET:
		wsprintf(job, L"Ԥ�Լ�");
		break;

	case GUARD:
		wsprintf(job, L"����");
		break;
	}

	setfont(24, 0, L"����");
	setcolor(BLACK);
	outtextxy(m_cards_xy[id].x + 50, m_cards_xy[id].y + 150, job);
}

// ���������Ϳ���
void DrawTableAndCard()
{
	// ����
	cleardevice();

	// ���������xy����
	int x, y;
	int up_card_num;// ������Ŀ��Ƶ�����

	up_card_num = m_nPlayerNum / 2;

	// ����ÿ����ҵĿ���
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

		// ��¼����λ��
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

// �õ��û�����Ŀ��ƶ�Ӧ��Id��������
int GetClickId()
{
	QQSendMsg("������Ŀ�����id��");

	while (true)
	{
		string msg = QQGetInput()->msg;
		int num = atoi(msg.c_str());
		num --;

		if (num < 0 || num > m_nPlayerNum)
		{
			QQSendMsg("ID��������������");
			continue;
		}

		else
		{
			return num;
		}
	}
}

// ɱ��ĳ�ˣ����ĳ�˵�����������
void Kill(int id)
{
	m_bWillDie[id] = true;
}

// ����һ���ˣ��������ǲ��ǽ����ģ�
void Save(int id)
{
	m_bWillDie[id] = false;
}

// ������������������д���
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

// ��
void woof(int id)
{
	// �����Ҳ���ǣ�����ʾ��������
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
		centertext(L"���������ۡ���");
		QQSendScreen();
		Sleep(500);

		if (!(rand() % 4))
		{
			QQSendMsg("���ҹ����ɱ�ˡ�");
		}

		else if (QQMessageBox("��Ҫɱ����"))
		{
			centertext(L"����һ����ҵĿ�����Ϊ����Ŀ��");
			QQSendScreen();

			while (true)
			{
				int index = GetClickId();
				if (index == id || isDie(index))
				{
					QQSendMsg("����Ҳ�����ΪĿ�꣬����ѡ");
				}
				else
				{
					Kill(index);
					break;
				}
			}
		}

		centertext(L"��������ۡ���");
		QQSendScreen();
	}
	else
	{
		// 30% ���ʲ�ɱ
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

// Ů��
void witch(int id)
{
	if (id == m_nMyId)
	{
		centertext(L"Ů�������ۡ���");
		QQSendScreen();
		Sleep(500);

		// Ů��Ҫ��ʲôҩˮ��
		// 0��ʾʲôҲû�ã�1��ʾ��ҩ��2��ʾ��ҩ
		int use = 0;

		if (m_nPoisonNum && QQMessageBox("Ů�ף���Ҫʹ�ö�ҩ��"))
		{
			use = 1;
		}
		else if (m_nAntidoteNum && QQMessageBox("��ôŮ�ף���Ҫʹ�ý�ҩ��"))
		{
			use = 2;
		}

		if (use)
		{
			centertext(L"����һ����ҵĿ�����ΪĿ��");
			QQSendScreen();

			while (true)
			{
				int index = GetClickId();
				if (isDie(index))
				{
					QQSendMsg("����Ҳ�����ΪĿ�꣬����ѡ");
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

		centertext(L"Ů������ۡ���");
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
						// ��������һ����ʲ�ɱ
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
						// ����������һ����ʲ���
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

// Ԥ�Լ�
void prophet(int id)
{
	if (id == m_nMyId)
	{
		centertext(L"Ԥ�Լ������ۡ���");
		QQSendScreen();
		Sleep(500);

		if (QQMessageBox("Ԥ�Լң���Ҫ��ĳ�˵������"))
		{
			centertext(L"����һ����ҵĿ�����ΪĿ��");
			QQSendScreen();

			while (true)
			{
				int index = GetClickId();
				if (isDie(index))
				{
					QQSendMsg("����Ҳ�����ΪĿ�꣬����ѡ");
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

		centertext(L"Ԥ�Լ�����ۡ���");
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

// ����
void guard(int id)
{
	if (id == m_nMyId)
	{
		centertext(L"���������ۡ���");
		QQSendScreen();
		Sleep(500);

		if (QQMessageBox("��������ϣ������һ������"))
		{
			centertext(L"����һ����ҵĿ�����ΪĿ��");
			QQSendScreen();

			while (true)
			{
				int index = GetClickId();
				if (isDie(index))
				{
					QQSendMsg("����Ҳ�����ΪĿ�꣬����ѡ");
				}
				else
				{
					Save(index);

					break;
				}
			}
		}

		centertext(L"��������ۡ���");
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
				// ����������һ����ʲ���
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

// չʾ�����˵����
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

// �õ����ĳ�˵Ļ�
TCHAR* GetSay(int id, int index)
{
	// ˵����ʽ
	int mode = rand() % 3;

	TCHAR* strMsg = new TCHAR[64];
	memset(strMsg, 0, 64);

	switch (mode)
	{
	case 0:
		wsprintf(strMsg, L"%d��˵��%d�����ǰɣ�", id, index);
		break;
	case 1:
		wsprintf(strMsg, L"%d��˵���Ҿ���%d��Ӧ�������⡣", id, index);
		break;
	case 2:
		wsprintf(strMsg, L"%d��˵��%d�ž��������⣬Ͷ����", id, index);
		break;
	}

	return strMsg;
}

// �õ�������Ʊ����ߵ�һ������id
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

// �ж��Ƿ�ʤ��������0��ʾ�ޣ�1��ʾƽ�֣�2��ʾ����ʤ����3��ʾ����ʤ��
int isWin(int woof_num, int die_num)
{
	if (woof_num)
	{
		if (die_num == m_nPlayerNum)
		{
			centertext(L"�����˶����ˣ�˫��ƽ�֡�");
			return 1;
		}
		else if (m_nPlayerNum - die_num == woof_num)
		{
			centertext(L"���������⣬����ʤ����");
			return 3;
		}
	}
	else
	{
		centertext(L"���������⣬����ʤ����");
		return 2;
	}

	return 0;
}

// ͳ�Ƹ�����������
void GetPeopleNum(int& die_num, int& save_num, int& woof_num)
{
	// �����������������
	for (int i = 0; i < m_nPlayerNum; i++)
	{
		if (isDie(i))
		{
			die_num++;
			save_num--;
		}

		// ͳ�ƴ������
		else
		{
			if (m_players[i].job == WOOF)
			{
				woof_num++;
			}
		}
	}
}

// ��Ϸ��
void Game()
{
	setfont(20, 0, L"����");
	setbkmode(TRANSPARENT);

	// ��������ɫ
	setbkcolor(EGERGB(14, 65, 46));

	// ����
	int day_num = 1;

	// ����ʱ��(ms)
	int wait_time = 5000;

	// ǰһ�����������
	int old_die = 0;

	while (true)
	{
		if (QQIsEnd())
		{
			exit(0);
		}

		DrawTableAndCard();

		// ����������
		int die_num = 0;

		// �ܴ������
		int save_num = m_nPlayerNum;

		// ��������
		int woof_num = 0;

		GetPeopleNum(die_num, save_num, woof_num);

		// ����
		if (day_num % 2 == 0)
		{
			centertext(L"�����ˡ�");
			QQSendScreen();
			Sleep(2000);

			TCHAR last_light[128] = { 0 };

			int die = die_num - old_die;
			old_die = die_num;

			if (!die)
			{
				wsprintf(last_light, L"�����Ǹ�ƽ��ҹ��");
			}
			else
			{
				wsprintf(last_light, L"��������%d����", die);
			}

			Sleep(2000);
			centertext(last_light);
			QQSendScreen();

			// �ж�ʤ�� 
			if (isWin(woof_num, die_num))
			{
				break;
			}

			Sleep(4000);


			// ��һ��֮����з��ԡ�ͶƱ
			if (day_num != 2)
			{
				// �������
				for (int i = 0; i < m_nPlayerNum; i++)
				{
					// �Һ����˶����跢�ԡ�
					if (i == m_nMyId || isDie(i))
					{
						continue;
					}

					// ���� 50%���� ����
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

					// Ԥ�Լ����������һ��Ҫ���Ե�
					else if (m_players[i].job == PROPHET)
					{
						if (!isDie(m_look_id) && m_look_job == WOOF)
						{
							centertext(GetSay(i + 1, m_look_id + 1));
							QQSendScreen();
							Sleep(3000);
						}
					}

					// ��ʱ������Ҳ��˵
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

				centertext(L"���ڽ���ͶƱ��Ʊ������һ�˱��д�������");
				QQSendScreen();
				Sleep(3000);

				// ��¼ÿ�˵�Ʊ��
				int votes[MAX_PLAYER_NUM] = { 0 };

				// ͶƱ����
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
								// �����˸�������Ͷ�ǡ�Ԥ�Լҿ�������
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

				// չʾͶƱ���
				for (int i = 0; i < m_nPlayerNum; i++)
				{
					TCHAR strNum[12] = { 0 };
					wsprintf(strNum, L"%dƱ", votes[i]);
					outtextxy(m_cards_xy[i].x + 25, m_cards_xy[i].y + 25, strNum);
				}
				QQSendScreen();
				Sleep(6000);

				// �ֵ���ͶƱ
				if (!isDie(m_nMyId) && QQMessageBox("��ҪͶƱ�𣿵��ȡ�����ʾ��Ȩ��"))
				{
					centertext(L"����һ����ҵĿ�����ΪͶƱĿ�ꡣ");
					QQSendScreen();

					while (true)
					{
						int index = GetClickId();

						if (isDie(index) || index == m_nMyId)
						{
							centertext(L"����Ҳ�����ΪͶƱĿ�꣬����ѡ��");
							QQSendScreen();
						}
						else
						{
							votes[index] ++;

							TCHAR msg[64];
							wsprintf(msg, L"��Ͷ��%d��һƱ����������%dƱ", index + 1, votes[index]);
							centertext(msg);
							QQSendScreen();
							Sleep(2000);
							break;
						}
					}
				}

				// ͳ�Ƶó�Ʊ�������
				int highest[MAX_PLAYER_NUM] = { 0 };
				int num = 0;
				GetHighestVotes(votes, m_nPlayerNum, highest, &num);

				// �д���
				TCHAR msg[64] = { 0 };

				// ɱ����Ʊ����ߣ������Ƕ����
				for (int i = 0; i < num; i++)
				{
					wsprintf(msg, L"%ls%d�ţ�", msg, highest[i] + 1);
					Kill(highest[i]);

					// ��������ҲҪ����ɵ�����������������������ʾ����������ʱ�����ִ���
					old_die++;
				}

				wsprintf(msg, L"%ls���д�������", msg);
				ClearWillDieList();
				DrawTableAndCard();
				centertext(msg);
				QQSendScreen();
				Sleep(5000);

				GetPeopleNum(die_num, save_num, woof_num);

				// �ж�ʤ�� 
				if (isWin(woof_num, die_num))
				{
					break;
				}
			}
		}
		// ����
		else
		{
			centertext(L"�������ۡ���");
			QQSendScreen();
			Sleep(wait_time);
			
			// ��һҹ��ƽ��ҹ
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

				// ��������Ľ�������
				ClearWillDieList();
			}
		}

		day_num++;

		Sleep(20);
	}

	Sleep(2000);

	ShowAll();
	centertext(L"��Ϸ������");
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
