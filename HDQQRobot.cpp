///////////////////////////////////////////////
//
//	HDQQRobot.cpp
//	huidong QQ�����˿⣨�ǹٷ��ӿڰ棩
//
//	by huidong <mailkey@yeah.net>
//
//	Ver 0.3
//	����ʱ��		2020.8.9
//	���һ���޸�	2020.8.19
//

#include "HDQQRobot.h"

HWND m_MsgWnd = NULL;		// QQ��Ϣ����
HWND m_SendMsgWnd = NULL;	// QQ������Ϣ����

POINT m_MsgPoint = { 0 };		// ��Ϣ������Ҫ�����λ��
POINT m_SendMsgPoint = { 0 };	// ������Ϣ������Ҫ�����λ��

RECT m_MsgRct = { 0 };			// ��Ϣ����λ�á���С
RECT m_SendMsgRct = { 0 };		// ������Ϣ����λ�á���С

int m_kbDelay = 50; // ��Ϊģ����̶���Ҫ���ӳ�ʱ�䣨ms��

// �������������а�
void CopyToClipboard(const char* data)
{
	if (!data)
	{
		return;
	}

	int nDataLen = strlen(data) + 1;

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
		strcpy_s(buff, nDataLen, data);

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
void QQGotoMsgWnd()
{
	SetWindowPos(m_MsgWnd, HWND_TOP, m_MsgRct.left, m_MsgRct.top, m_MsgRct.right - m_MsgRct.left, m_MsgRct.bottom - m_MsgRct.top, NULL);
	ClickAt(m_MsgPoint.x + rand() % 2, m_MsgPoint.y + rand() % 2);	// ���������Ҫ��ʱ������ƫ��
}

// ʹQQ������Ϣ�Ĵ��ڻ�ý���
void QQGotoSendMsgWnd()
{
	SetWindowPos(m_SendMsgWnd, HWND_TOP, m_SendMsgRct.left, m_SendMsgRct.top, m_SendMsgRct.right - m_SendMsgRct.left, m_SendMsgRct.bottom - m_SendMsgRct.top, NULL);
	ClickAt(m_SendMsgPoint.x, m_SendMsgPoint.y);
}

// ����QQ��Ϣ
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

// ����QQ��Ϣ����λ��
void SetQQMsgWndPoint(HWND wnd, POINT xy, RECT rct)
{
	m_MsgWnd = wnd;
	m_MsgPoint = xy;
	m_MsgRct = rct;
}

// ����QQ������Ϣ����λ��
void SetQQSendMsgWndPoint(HWND wnd, POINT xy, RECT rct)
{
	m_SendMsgWnd = wnd;
	m_SendMsgPoint = xy;
	m_SendMsgRct = rct;
}

// ���ü�����ʱʱ��
void QQSetKbDelay(int ms)
{
	m_kbDelay = ms;
}

// �õ�������ʱʱ��
int QQGetKbDelay()
{
	return m_kbDelay;
}

// Ԥ���庯������ʼ�˵�
void QQStartMenu()
{
	HWND MsgWnd, SendMsgWnd;
	POINT MsgPoint, SendMsgPoint;
	RECT MsgRct, SendMsgRct;

	printf("QQ Robot - by huidong \n");

	printf(
		"\n"
		"1. ������ƶ���QQ��Ϣ�����ϣ�����F11һ�Ρ�\n"
		"2. ������ƶ���QQ������Ϣ�Ŀ������F12һ��\n"
		"\n"
	);

	while (!(GetKeyState(VK_F11) & 0x80));
	GetCursorPos(&MsgPoint);
	MsgWnd = WindowFromPoint(MsgPoint);
	GetWindowRect(MsgWnd, &MsgRct);


	printf("��1�� ����ɣ��ȴ� ��2���İ���\n");

	while (!(GetKeyState(VK_F12) & 0x80));
	GetCursorPos(&SendMsgPoint);
	SendMsgWnd = WindowFromPoint(SendMsgPoint);
	GetWindowRect(SendMsgWnd, &SendMsgRct);

	printf("��2�� ����ɡ�\n\n");

	// ���ô���λ��
	SetQQMsgWndPoint(MsgWnd, MsgPoint, MsgRct);
	SetQQSendMsgWndPoint(SendMsgWnd, SendMsgPoint, SendMsgRct);

	printf("\n [ ��F10������������ ] \n\n");

	while (!(GetKeyState(VK_F10) & 0x80));

	QQGotoMsgWnd();

	printf("\n������������\n");
	printf("����F8��ʱ����ͣ�����ˡ�\n");
	printf("����F9��ʱ���رջ����ˡ�\n");
}


// Ԥ���庯�����Ƿ��˳������ˣ�����ture��ʾҪ�˳�������
bool QQIsEnd()
{
	if (GetKeyState(VK_F9) & 0x80)
	{
		printf("\nF9���Ѱ��£������˹رա�\n\n");
		_getch();
		return true;
	}
	return false;
}

// Ԥ���庯�����Ƿ���ͣ�����ˣ�������ֵ�������������ͣ�������Զ�������ͣ
void QQIsPause()
{
	if (GetKeyState(VK_F8) & 0x80)
	{
		printf("\nF8���Ѱ��£���������ͣ��������F7������\n\n");

		while (!(GetKeyState(VK_F7) & 0x80));
		printf("\nF7���Ѱ��£������˼���������\n\n");
	}
}

// �ж�һ�ַ��Ƿ�Ϊ����
bool isNum(char c)
{
	return c >= '0' && c <= '9';
}

// �ж�һ�ַ����Ƿ�����ʱ���ʽ��xx:xx:xx�������ó�����ʱ��
bool isTime(string str, int* h, int* m, int* s)
{
	char hour[3] = { 0 };
	char min[3] = { 0 };
	char second[3] = { 0 };

	int hl = 1;

	// ��ʽ�ж�
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
				// ��ʽ�ж���ȷ����������ת��

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

// �ж�һ�ַ����Ƿ��������ڸ�ʽ��xxxx/xx/xx�������ó���������
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
					// ��ʽ�ж���ȷ����������ת��

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

// �滻�ַ����е�����ָ���ַ���
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


// ����QQ��Ϣ��ȡ���һ����Ϣ��
void QQLexMessage(string strMsg, QQMsg* list)
{
	// ɾ������\r
	replace_all(strMsg, "\r", "");

	int len = strMsg.length();
	string strOneMsg;	// ���һ����Ϣ��������Ϣ��

	int sub = 1;

	// ȥ����ĩβ�Ļ���
	for (int i = len - sub; i >= 0; i--)
	{
		if (strMsg[i] == '\n' || strMsg[i] == '\r')
			sub++;
		else
			break;
	}

	/*

	QQ����Ϣ����

		huidong 8:20:33
		ckj����������

		ckj 8:21:12
		����

	ÿ�εõ�����Ϣĩβ�����������У�������Ҫȥ��ĩβ�Ļ��С�
	������\r\nʽ������ɾ������\r��������\n��

	Ȼ��ʼ�Ӻ���ǰ��ȡ��Ϣ���ݣ�һֱ������һ����Ϣ����������������εķ������������

	�����Ƕ�ȡ������ʱ���õ���������һ���Կո���н������ַ�����ע���ǴӺ���ǰ��ȡ����
	Ȼ��Դ��ַ������и�ʽ�жϣ��ж����ǲ���ʱ���ʽ(xx:xx:xx)������ǵĻ����ͼ����������
	�Դ����Ƶõ��ַ������ж����ǲ������ڸ�ʽ(xxxx/xx/xx)�����ڸ�ʽֻ������Ϣ���ǽ��շ��͵�����²Ż���֣���
	���Ҳ�ǣ��Ǿͱ���Ϊ���ڣ�������ǣ��Ǿ�ȡ���������Ϊ��Ϣ�������ڣ���������ʣ�µ����һ��
	�Ի��н�β���ַ��������û����ˣ�����û�����Ϊ�գ���ôʱ�䡢���ڡ��û�������Ϣ���ݣ����ǾͶ���ȡ���ˡ�

	�������������ܻ��޷�ʶ��������ٱ���û�������Ϣ������Ҳû���ٸ��õİ취�ˡ�

	�����ҵ��о���xx:xx:xx�У�Сʱ����ֻ��һ�����֣������ӡ����ǿ϶����������֣�
	xxxx/xx/xx�У������ͺŶ��п���ֻ��һ�����֣��ݴˣ����Խ��и�ʽ�жϡ�

	*/


	for (int i = len - sub; i >= 0; i--)
	{
		if (strMsg[i] == '\n' || i == 0)
		{
			// �жϽ������ĸ�ʽ�Ƿ�Ϊ��ȷ��ʱ���ʽ��xx:xx:xx�����û�����ʽ

			// �洢��Ϣ���͵���ϸʱ��
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

			// �õ�һ���Կո��β���ַ�������ʱ��
			for (time_len = 1; i - time_len >= 0 && strMsg[i - time_len] != ' ' && strMsg[i - time_len] != '\n'; time_len++)
			{
				strTime += strMsg[i - time_len];
			}

			// ��ת�ַ������ж�ʱ���ʽ�Ƿ���ȷ
			reverse(strTime.begin(), strTime.end());
			if (isTime(strTime, &h, &min, &s))
			{
				// ��ʱ���ʽ��ȷ�������ж��Ƿ����������Ϣ

				// �õ�һ���Կո��β���ַ�����������
				for (date_len = time_len + 1; i - date_len >= 0 && strMsg[i - date_len] != ' ' && strMsg[i - date_len] != '\n'; date_len++)
				{
					strDate += strMsg[i - date_len];
				}

				// ��ת�ַ������ж����ڸ�ʽ�Ƿ���ȷ
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

				// ��ȡ�û���

				for (name_len = date_len + 1; i - name_len >= 0 && strMsg[i - name_len] != '\n'; name_len++)
				{
					strName += strMsg[i - name_len];
				}

				// ���û�����Ϊ�գ���ô��ȡ�ɹ���
				if (strName.size())
				{
					// ��������

					// �ַ�����ת
					reverse(strName.begin(), strName.end());
					reverse(strOneMsg.begin(), strOneMsg.end());

					// д������
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


// �õ�QQ��Ϣ�����У�
// ���������Ϣ���򷵻�true
// ���򷵻�false
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
