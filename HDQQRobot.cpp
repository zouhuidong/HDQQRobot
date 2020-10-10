///////////////////////////////////////////////
//
//	HDQQRobot.cpp
//	huidong QQ�����˿⣨�ǹٷ��ӿڰ棩
//
//	by huidong <mailkey@yeah.net>
//
//	Ver 0.2
//	����ʱ��		2020.8.9
//	���һ���޸�	2020.8.9
//

#include "HDQQRobot.h"

POINT m_pQQMsgWnd;		// QQ��Ϣ��������
POINT m_pQQSendMsgWnd;	// QQ������Ϣ��������

// �û��Զ������Ϣ��Ӧ����
void (*ResponseFunc)(const char*) = NULL;

int m_kbDelay = 50; // ��Ϊģ����̶���Ҫ���ӳ�ʱ�䣨ms��

// �������������а�
void CopyToClipboard(const char* pszData)
{
	int nDataLen = strlen(pszData) + 1;

	// �����ݱ�����ճ����
	HGLOBAL hClip;

	// ����һ��HGLOBAL�����������ָ�������ڴ��
	if (OpenClipboard(NULL))
	{
		// ���������������
		EmptyClipboard();

		// �ڶ��Ϸ�����ƶ����ڴ�飬���򷵻�һ���ڴ���
		hClip = GlobalAlloc(GMEM_MOVEABLE, nDataLen);

		// �Է�����ڴ����м��������ڴ����ת����һ��ָ��,������Ӧ�����ü�������1
		char* buff = (char*)GlobalLock(hClip);
		strcpy_s(buff, nDataLen, pszData);

		// ����д����ϣ����н����������������ü��������ּ�1
		GlobalUnlock(hClip);

		// ����������ݵ��ڴ�������������Դ������
		SetClipboardData(CF_TEXT, hClip);

		// �رռ����壬�ͷż�������Դ��ռ��Ȩ
		CloseClipboard();
	}
}

// �Ӽ��а���ȡ������
void GetTextFromClipboard(char* pStr, int size)
{
	if (OpenClipboard(NULL))//�򿪼�����  
	{
		if (IsClipboardFormatAvailable(CF_TEXT))//�жϸ�ʽ�Ƿ�����������Ҫ  
		{
			HANDLE hClip;

			//��ȡ����  
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

// ���ĳһλ��
void ClickAt(int x, int y)
{
	SetCursorPos(x, y);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

// ʹQQ����Ϣ���ڻ�ý���
void HDQQGotoMsgWnd()
{
	ClickAt(m_pQQMsgWnd.x + rand() % 2, m_pQQMsgWnd.y + rand() % 2);
}

// ʹQQ������Ϣ�Ĵ��ڻ�ý���
void HDQQGotoSendMsgWnd()
{
	ClickAt(m_pQQSendMsgWnd.x + rand() % 2, m_pQQSendMsgWnd.y + rand() % 2);
}

// ����QQ��Ϣ
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

// ����QQ��Ϣ����λ��
void SetQQMsgWndPoint(POINT xy)
{
	m_pQQMsgWnd = xy;
}

// ����QQ������Ϣ����λ��
void SetQQSendMsgWndPoint(POINT xy)
{
	m_pQQSendMsgWnd = xy;
}

// ���ü�����ʱʱ��
void HDQQSetKbDelay(int ms)
{
	m_kbDelay = ms;
}

// �õ�������ʱʱ��
int HDQQGetKbDelay()
{
	return m_kbDelay;
}

// Ԥ���庯������ʼ�˵�
void HDQQStartMenu()
{
	POINT MsgWnd, SendMsgWnd;

	printf("QQ Robot - by huidong \n");

	printf(
		"\n"
		"1. ������ƶ���QQ��Ϣ�����ϣ�����F11һ�Ρ�\n"
		"2. ������ƶ���QQ������Ϣ�Ŀ������F12һ��\n"
		"\n"
	);

	while (!(GetKeyState(VK_F11) & 0x80));
	GetCursorPos(&MsgWnd);

	printf("��1�� ����ɣ��ȴ� ��2���İ���\n");

	while (!(GetKeyState(VK_F12) & 0x80));
	GetCursorPos(&SendMsgWnd);

	printf("��2�� ����ɡ�\n\n");

	// ���ô���λ��
	SetQQMsgWndPoint(MsgWnd);
	SetQQSendMsgWndPoint(SendMsgWnd);

	printf("\n [ ��F10������������ ] \n\n");

	while (!(GetKeyState(VK_F10) & 0x80));

	HDQQGotoMsgWnd();

	printf("\n������������\n");
	printf("����F8��ʱ����ͣ�����ˡ�\n");
	printf("����F9��ʱ���رջ����ˡ�\n");
}


// Ԥ���庯�����Ƿ��˳������ˣ�����ture��ʾҪ�˳�������
bool HDQQIsEnd()
{
	if (GetKeyState(VK_F9) & 0x80)
	{
		printf("\nF9���Ѱ��£������˹رա�\n\n");
		_getch();
		//system("pause");
		return true;
	}
	return false;
}

// Ԥ���庯�����Ƿ���ͣ�����ˣ�������ֵ�������������ͣ�������Զ�������ͣ
void HDQQIsPause()
{
	if (GetKeyState(VK_F8) & 0x80)
	{
		printf("\nF8���Ѱ��£���������ͣ��������F7������\n\n");
		
		while(!(GetKeyState(VK_F7) & 0x80));
		printf("\nF7���Ѱ��£������˼���������\n\n");
	}
}

// ������Ϣ��Ӧ������Ҳ�൱�ڳ�ʼ��������
void HDQQSetResponseFunc(void (*func)(const char*))
{
	srand((unsigned)time(NULL));
	ResponseFunc = func;
}

// �õ���Ϣ��Ӧ����
void* HDQQGetResponseFunc()
{
	return ResponseFunc;
}

// ����Ϣ������Ӧ
void DoResponse(char* pMsg)
{
	if (ResponseFunc && pMsg)
	{
		ResponseFunc(pMsg);
	}
}


// ����QQ��Ϣ��ȡ���һ����Ϣ��
void HDQQLexMessage(const char* pMessage)
{
	int len = strlen(pMessage);
	int index = 0;
	char* strOneMsg = new char[len + 1];
	memset(strOneMsg, 0, len + 1);

	int sub = 1;

	// ȥ����ĩβ�Ļ���
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
		// ����\r
		if (pMessage[i] == '\r')
		{
			//index++;
		}

		else if (pMessage[i] != '\n' || i == 0)
		{
			strOneMsg[index] = pMessage[i];
			index++;
		}

		// ���򣬵õ�һ����Ϣ
		else
		{
			// �ַ�����ת
			_strrev(strOneMsg);
			DoResponse(strOneMsg);
			break;
		}
	}

	delete[] strOneMsg;
}


// �õ�QQ��Ϣ�����У�
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
