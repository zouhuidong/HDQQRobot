///////////////////////////////////////////////
//
//	HDQQRobot.cpp
//	huidong QQ机器人库（非官方接口版）
//
//	by huidong <mailkey@yeah.net>
//
//	Ver 0.3
//	创建时间		2020.8.9
//	最后一次修改	2020.8.19
//

#include "HDQQRobot.h"

HWND m_MsgWnd = NULL;		// QQ消息窗口
HWND m_SendMsgWnd = NULL;	// QQ发送消息窗口

POINT m_MsgPoint = { 0 };		// 消息窗口需要点击的位置
POINT m_SendMsgPoint = { 0 };	// 发送消息窗口需要点击的位置

RECT m_MsgRct = { 0 };			// 消息窗口位置、大小
RECT m_SendMsgRct = { 0 };		// 发送消息窗口位置、大小

int m_kbDelay = 50; // 因为模拟键盘而必要的延迟时间（ms）

// 复制数据至剪切板
void CopyToClipboard(const char* data)
{
	if (!data)
	{
		return;
	}

	int nDataLen = strlen(data) + 1;

	// 将数据保存至粘贴板
	HGLOBAL hClip;

	// 定义一个HGLOBAL句柄变量用来指向分配的内存块
	if (OpenClipboard(NULL))
	{
		// 将剪贴板内容清空
		EmptyClipboard();

		// 在堆上分配可移动的内存块，程序返回一个内存句柄
		hClip = GlobalAlloc(GMEM_MOVEABLE, nDataLen);

		// 对分配的内存块进行加锁，将内存块句柄转化成一个指针,并将相应的引用计数器加1
		char* buff = (char*)GlobalLock(hClip);
		strcpy_s(buff, nDataLen, data);

		// 数据写入完毕，进行解锁操作，并将引用计数器数字减1
		GlobalUnlock(hClip);

		// 将存放有数据的内存块放入剪贴板的资源管理中
		SetClipboardData(CF_TEXT, hClip);

		// 关闭剪贴板，释放剪贴板资源的占用权
		CloseClipboard();
	}
}

// 从剪切板中取得数据
void GetTextFromClipboard(char* pStr, int size)
{
	if (OpenClipboard(NULL))//打开剪贴板  
	{
		if (IsClipboardFormatAvailable(CF_TEXT))//判断格式是否是我们所需要  
		{
			HANDLE hClip;

			//读取数据  
			hClip = GetClipboardData(CF_TEXT);
			char* str = (char*)GlobalLock(hClip);

			memset(pStr, 0, size);
			if (str != NULL)
				strcpy_s(pStr, size, str);

			GlobalUnlock(hClip);
			CloseClipboard();
		}
	}
}

// 点击某一位置
void ClickAt(int x, int y)
{
	SetCursorPos(x, y);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

// 使QQ的消息窗口获得焦点
void QQGotoMsgWnd()
{
	SetWindowPos(m_MsgWnd, HWND_TOP, m_MsgRct.left, m_MsgRct.top, m_MsgRct.right - m_MsgRct.left, m_MsgRct.bottom - m_MsgRct.top, NULL);
	ClickAt(m_MsgPoint.x + rand() % 2, m_MsgPoint.y + rand() % 2);	// 鼠标点击处需要不时有少许偏差
}

// 使QQ发送消息的窗口获得焦点
void QQGotoSendMsgWnd()
{
	SetWindowPos(m_SendMsgWnd, HWND_TOP, m_SendMsgRct.left, m_SendMsgRct.top, m_SendMsgRct.right - m_SendMsgRct.left, m_SendMsgRct.bottom - m_SendMsgRct.top, NULL);
	ClickAt(m_SendMsgPoint.x, m_SendMsgPoint.y);
}

// 发送QQ消息
void QQSendMsg(string msg)
{
	QQGotoSendMsgWnd();

	CopyToClipboard(msg.c_str());

	keybd_event(VK_CONTROL, 0, 0, 0);
	Sleep(m_kbDelay);
	keybd_event('V', 0, 0, 0);
	Sleep(m_kbDelay);
	keybd_event('V', 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);

	Sleep(m_kbDelay);

	keybd_event(VK_RETURN, 0, 0, 0);
	Sleep(m_kbDelay);
	keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
}

// 设置QQ消息窗口位置
void SetQQMsgWndPoint(HWND wnd, POINT xy, RECT rct)
{
	m_MsgWnd = wnd;
	m_MsgPoint = xy;
	m_MsgRct = rct;
}

// 设置QQ发送消息窗口位置
void SetQQSendMsgWndPoint(HWND wnd, POINT xy, RECT rct)
{
	m_SendMsgWnd = wnd;
	m_SendMsgPoint = xy;
	m_SendMsgRct = rct;
}

// 设置键盘延时时间
void QQSetKbDelay(int ms)
{
	m_kbDelay = ms;
}

// 得到键盘延时时间
int QQGetKbDelay()
{
	return m_kbDelay;
}

// 预定义函数：开始菜单
void QQStartMenu()
{
	HWND MsgWnd, SendMsgWnd;
	POINT MsgPoint, SendMsgPoint;
	RECT MsgRct, SendMsgRct;

	printf("QQ Robot - by huidong \n");

	printf(
		"\n"
		"1. 将鼠标移动到QQ消息窗口上，按下F11一次。\n"
		"2. 将鼠标移动到QQ发送消息的框里，按下F12一次\n"
		"\n"
	);

	while (!(GetKeyState(VK_F11) & 0x80));
	GetCursorPos(&MsgPoint);
	MsgWnd = WindowFromPoint(MsgPoint);
	GetWindowRect(MsgWnd, &MsgRct);


	printf("第1步 已完成，等待 第2步的按下\n");

	while (!(GetKeyState(VK_F12) & 0x80));
	GetCursorPos(&SendMsgPoint);
	SendMsgWnd = WindowFromPoint(SendMsgPoint);
	GetWindowRect(SendMsgWnd, &SendMsgRct);

	printf("第2步 已完成。\n\n");

	// 设置窗口位置
	SetQQMsgWndPoint(MsgWnd, MsgPoint, MsgRct);
	SetQQSendMsgWndPoint(SendMsgWnd, SendMsgPoint, SendMsgRct);

	printf("\n [ 按F10键启动机器人 ] \n\n");

	while (!(GetKeyState(VK_F10) & 0x80));

	QQGotoMsgWnd();

	printf("\n机器人启动。\n");
	printf("按下F8键时，暂停机器人。\n");
	printf("按下F9键时，关闭机器人。\n");
}


// 预定义函数：是否退出机器人，返回ture表示要退出机器人
bool QQIsEnd()
{
	if (GetKeyState(VK_F9) & 0x80)
	{
		printf("\nF9键已按下，机器人关闭。\n\n");
		_getch();
		return true;
	}
	return false;
}

// 预定义函数：是否暂停机器人（不返回值），如果按下暂停按键则自动处理暂停
void QQIsPause()
{
	if (GetKeyState(VK_F8) & 0x80)
	{
		printf("\nF8键已按下，机器人暂停工作，按F7继续。\n\n");

		while (!(GetKeyState(VK_F7) & 0x80));
		printf("\nF7键已按下，机器人继续工作。\n\n");
	}
}

// 判断一字符是否为数字
bool isNum(char c)
{
	return c >= '0' && c <= '9';
}

// 判断一字符串是否满足时间格式（xx:xx:xx），并得出整型时间
bool isTime(string str, int* h, int* m, int* s)
{
	char hour[3] = { 0 };
	char min[3] = { 0 };
	char second[3] = { 0 };

	int hl = 1;

	// 格式判断
	if (isNum(str[0]))
	{
		if (isNum(str[1]))
		{
			hl = 2;
		}

		if (
			str[hl] == ':' &&
			isNum(str[hl + 1]) &&
			isNum(str[hl + 2]) &&
			str[hl + 3] == ':' &&
			isNum(str[hl + 4]) &&
			isNum(str[hl + 5])
			)
		{
			if (str.size() == hl + 6)
			{
				// 格式判断正确，进行类型转换

				hour[0] = str[0];
				if (hl == 2)
					hour[1] = str[1];

				min[0] = str[hl + 1];
				min[1] = str[hl + 2];

				second[0] = str[hl + 4];
				second[1] = str[hl + 5];

				int th,tm,ts;

				th = atoi(hour);
				tm = atoi(min);
				ts = atoi(second);

				if (
					th >= 0 && th <= 23 &&
					tm >= 0 && tm <= 59 &&
					ts >= 0 && ts <= 59
					)
				{
					*h = th;
					*m = tm;
					*s = ts;

					return true;
				}
			}
		}
	}

	return false;
}

// 判断一字符串是否满足日期格式（xxxx/xx/xx），并得出整型日期
bool isDate(string str, int* y, int* m, int* d)
{
	char year[5] = { 0 };
	char month[3] = { 0 };
	char day[3] = { 0 };

	int ml = 1;
	int dl = 1;

	if (
		isNum(str[0]) &&
		isNum(str[1]) &&
		isNum(str[2]) &&
		isNum(str[3]) &&
		str[4] == '/'
		)
	{
		year[0] = str[0];
		year[1] = str[1];
		year[2] = str[2];
		year[3] = str[3];

		if (isNum(str[5]))
		{
			month[0] = str[5];

			if (isNum(str[6]))
			{
				ml = 2;
				month[1] = str[6];
			}

			if (str[5 + ml] == '/' && isNum(str[6 + ml]))
			{
				day[0] = str[6 + ml];
				if (isNum(str[7 + ml]))
				{
					dl = 2;
					day[1] = str[7 + ml];
				}

				if (str.size() == 6 + dl + ml)
				{
					// 格式判断正确，进行类型转换

					int ty, tm, td;

					ty = atoi(year);
					tm = atoi(month);
					td = atoi(day);

					SYSTEMTIME time;
					GetSystemTime(&time);

					if (
						ty >= 2000 && ty <= time.wYear &&
						tm >= 1 && tm <= 12 &&
						td >= 1 && td <= 31
						)
					{

						*y = ty;
						*m = tm;
						*d = td;

						return true;
					}
				}
			}
		}
	}

	return false;
}

// 替换字符串中的所有指定字符串
string& replace_all(string& str, const  string& old_value, const  string& new_value)
{
	while (true)
	{
		string::size_type   pos(0);
		if ((pos = str.find(old_value)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else
			break;
	}
	return   str;
}


// 分析QQ消息（取最后一条消息）
void QQLexMessage(string strMsg, QQMsg* list)
{
	// 删除所有\r
	replace_all(strMsg, "\r", "");

	int len = strMsg.length();
	string strOneMsg;	// 最后一条消息（单个消息）

	int sub = 1;

	// 去除最末尾的换行
	for (int i = len - sub; i >= 0; i--)
	{
		if (strMsg[i] == '\n' || strMsg[i] == '\r')
			sub++;
		else
			break;
	}

	/*

	QQ的消息举例

		huidong 8:20:33
		ckj，出来玩吗？

		ckj 8:21:12
		不了

	每次得到的消息末尾会多出两个换行，所以需要去除末尾的换行。
	换行是\r\n式，所以删除所有\r，仅保留\n。

	然后开始从后往前读取消息内容，一直到读完一条消息，把它存入链表，这次的分析就算结束。

	当我们读取到换行时，得到接下来的一串以空格或换行结束的字符串（注意是从后往前读取），
	然后对此字符串进行格式判断，判断它是不是时间格式(xx:xx:xx)，如果是的话，就继续往后读，
	以此类推得到字符串，判断它是不是日期格式(xxxx/xx/xx)（日期格式只有在消息不是今日发送的情况下才会出现），
	如果也是，那就保存为日期，如果不是，那就取今天的日期为消息发送日期，继续读，剩下的最后一串
	以换行结尾的字符串就是用户名了，如果用户名不为空，那么时间、日期、用户名、消息内容，我们就都读取到了。

	这样的做法可能会无法识别他人造假别的用户发送信息，但是也没有再更好的办法了。

	根据我的研究，xx:xx:xx中，小时可能只有一个数字，而分钟、秒是肯定有两个数字；
	xxxx/xx/xx中，月数和号都有可能只有一个数字，据此，可以进行格式判断。

	*/


	for (int i = len - sub; i >= 0; i--)
	{
		if (strMsg[i] == '\n' || i == 0)
		{
			// 判断接下来的格式是否为正确的时间格式（xx:xx:xx）、用户名格式

			// 存储消息发送的详细时间
			int y;
			int m;
			int d;
			int h;
			int min;
			int s;

			string strTime;
			string strDate;
			string strName;

			int time_len;
			int date_len;
			int name_len;

			// 得到一段以空格结尾的字符串，即时间
			for (time_len = 1; i - time_len >= 0 && strMsg[i - time_len] != ' ' && strMsg[i - time_len] != '\n'; time_len++)
			{
				strTime += strMsg[i - time_len];
			}

			// 反转字符串，判断时间格式是否正确
			reverse(strTime.begin(), strTime.end());
			if (isTime(strTime, &h, &min, &s))
			{
				// 若时间格式正确，继续判断是否存在日期信息

				// 得到一段以空格结尾的字符串，即日期
				for (date_len = time_len + 1; i - date_len >= 0 && strMsg[i - date_len] != ' ' && strMsg[i - date_len] != '\n'; date_len++)
				{
					strDate += strMsg[i - date_len];
				}

				// 反转字符串，判断日期格式是否正确
				reverse(strDate.begin(), strDate.end());
				if (!isDate(strDate, &y, &m, &d))
				{
					SYSTEMTIME time;
					GetSystemTime(&time);

					y = time.wYear;
					m = time.wMonth;
					d = time.wDay;

					date_len = time_len;
				}

				// 读取用户名

				for (name_len = date_len + 1; i - name_len >= 0 && strMsg[i - name_len] != '\n'; name_len++)
				{
					strName += strMsg[i - name_len];
				}

				// 若用户名不为空，那么读取成功。
				if (strName.size())
				{
					// 存入链表

					// 字符串反转
					reverse(strName.begin(), strName.end());
					reverse(strOneMsg.begin(), strOneMsg.end());

					// 写入链表
					QQMsg* last = QQMsgAddNode(list);

					last->name = strName;
					last->msg = strOneMsg;
					last->y = y;
					last->m = m;
					last->d = d;
					last->h = h;
					last->min = min;
					last->s = s;

					break;
				}
			}
		}
		
		strOneMsg += strMsg[i];
	}
}


// 得到QQ消息（所有）
// 如果有新消息，则返回true
// 否则返回false
bool QQGetMsg(string & msg)
{
	QQGotoMsgWnd();

	// ctrl+a, ctrl + c
	keybd_event(VK_CONTROL, 0, 0, 0);
	Sleep(m_kbDelay);
	keybd_event('A', 0, 0, 0);
	Sleep(m_kbDelay);
	keybd_event('A', 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);

	Sleep(m_kbDelay);

	keybd_event(VK_CONTROL, 0, 0, 0);
	Sleep(m_kbDelay);
	keybd_event('C', 0, 0, 0);
	Sleep(m_kbDelay);
	keybd_event('C', 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
	// end

	const int size = 2048000;
	char* new_msg = new char[size];
	memset(new_msg, 0, size);

	GetTextFromClipboard(new_msg, size);

	string strNewMsg = new_msg;

	delete[] new_msg;

	if (new_msg)
	{
		if (strNewMsg != msg)
		{
			msg = strNewMsg;
			return true;
		}
		else
		{
			return false;
		}
	}
}
