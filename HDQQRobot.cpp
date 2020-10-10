///////////////////////////////////////////////
//
//	HDQQRobot.cpp
//	huidong QQ机器人库（非官方接口版）
//
//	by huidong <mailkey@yeah.net>
//
//	Ver 0.2
//	创建时间		2020.8.9
//	最后一次修改	2020.8.9
//

#include "HDQQRobot.h"

POINT m_pQQMsgWnd;		// QQ消息窗口坐标
POINT m_pQQSendMsgWnd;	// QQ发送消息窗口坐标

// 用户自定义的消息响应函数
void (*ResponseFunc)(const char*) = NULL;

int m_kbDelay = 50; // 因为模拟键盘而必要的延迟时间（ms）

// 复制数据至剪切板
void CopyToClipboard(const char* pszData)
{
	int nDataLen = strlen(pszData) + 1;

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
		strcpy_s(buff, nDataLen, pszData);

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
void HDQQGotoMsgWnd()
{
	ClickAt(m_pQQMsgWnd.x + rand() % 2, m_pQQMsgWnd.y + rand() % 2);
}

// 使QQ发送消息的窗口获得焦点
void HDQQGotoSendMsgWnd()
{
	ClickAt(m_pQQSendMsgWnd.x + rand() % 2, m_pQQSendMsgWnd.y + rand() % 2);
}

// 发送QQ消息
void HDQQSendMsg(const char* msg)
{
	HDQQGotoSendMsgWnd();

	CopyToClipboard(msg);

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
void SetQQMsgWndPoint(POINT xy)
{
	m_pQQMsgWnd = xy;
}

// 设置QQ发送消息窗口位置
void SetQQSendMsgWndPoint(POINT xy)
{
	m_pQQSendMsgWnd = xy;
}

// 设置键盘延时时间
void HDQQSetKbDelay(int ms)
{
	m_kbDelay = ms;
}

// 得到键盘延时时间
int HDQQGetKbDelay()
{
	return m_kbDelay;
}

// 预定义函数：开始菜单
void HDQQStartMenu()
{
	POINT MsgWnd, SendMsgWnd;

	printf("QQ Robot - by huidong \n");

	printf(
		"\n"
		"1. 将鼠标移动到QQ消息窗口上，按下F11一次。\n"
		"2. 将鼠标移动到QQ发送消息的框里，按下F12一次\n"
		"\n"
	);

	while (!(GetKeyState(VK_F11) & 0x80));
	GetCursorPos(&MsgWnd);

	printf("第1步 已完成，等待 第2步的按下\n");

	while (!(GetKeyState(VK_F12) & 0x80));
	GetCursorPos(&SendMsgWnd);

	printf("第2步 已完成。\n\n");

	// 设置窗口位置
	SetQQMsgWndPoint(MsgWnd);
	SetQQSendMsgWndPoint(SendMsgWnd);

	printf("\n [ 按F10键启动机器人 ] \n\n");

	while (!(GetKeyState(VK_F10) & 0x80));

	HDQQGotoMsgWnd();

	printf("\n机器人启动。\n");
	printf("按下F8键时，暂停机器人。\n");
	printf("按下F9键时，关闭机器人。\n");
}


// 预定义函数：是否退出机器人，返回ture表示要退出机器人
bool HDQQIsEnd()
{
	if (GetKeyState(VK_F9) & 0x80)
	{
		printf("\nF9键已按下，机器人关闭。\n\n");
		_getch();
		//system("pause");
		return true;
	}
	return false;
}

// 预定义函数：是否暂停机器人（不返回值），如果按下暂停按键则自动处理暂停
void HDQQIsPause()
{
	if (GetKeyState(VK_F8) & 0x80)
	{
		printf("\nF8键已按下，机器人暂停工作，按F7继续。\n\n");
		
		while(!(GetKeyState(VK_F7) & 0x80));
		printf("\nF7键已按下，机器人继续工作。\n\n");
	}
}

// 设置消息响应函数（也相当于初始化函数）
void HDQQSetResponseFunc(void (*func)(const char*))
{
	srand((unsigned)time(NULL));
	ResponseFunc = func;
}

// 得到消息响应函数
void* HDQQGetResponseFunc()
{
	return ResponseFunc;
}

// 对消息进行响应
void DoResponse(char* pMsg)
{
	if (ResponseFunc && pMsg)
	{
		ResponseFunc(pMsg);
	}
}


// 分析QQ消息（取最后一条消息）
void HDQQLexMessage(const char* pMessage)
{
	int len = strlen(pMessage);
	int index = 0;
	char* strOneMsg = new char[len + 1];
	memset(strOneMsg, 0, len + 1);

	int sub = 1;

	// 去除最末尾的换行
	for (int i = len - sub; i >= 0; i--)
	{
		if (pMessage[i] == '\n' || pMessage[i] == '\r')
		{
			sub++;
		}
		else
		{
			break;
		}
	}

	for (int i = len - sub; i >= 0; i--)
	{
		// 跳过\r
		if (pMessage[i] == '\r')
		{
			//index++;
		}

		else if (pMessage[i] != '\n' || i == 0)
		{
			strOneMsg[index] = pMessage[i];
			index++;
		}

		// 否则，得到一条消息
		else
		{
			// 字符串反转
			_strrev(strOneMsg);
			DoResponse(strOneMsg);
			break;
		}
	}

	delete[] strOneMsg;
}


// 得到QQ消息（所有）
void HDQQGetMsg(char* msg, const int size)
{
	HDQQGotoMsgWnd();

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

	char* new_msg = new char[size];
	memset(new_msg, 0, size);

	GetTextFromClipboard(new_msg, size);
	if (new_msg)
	{
		if (strcmp(msg, new_msg) != 0)
		{
			memset(msg, 0, size);
			strcpy_s(msg, size, new_msg);
		}
	}

	delete[] new_msg;
}
