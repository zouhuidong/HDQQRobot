#include "HDQQRobot.h"
#include "game.h"

#include "EasyXHBitmap.h"

// �ظ���Ϣ
void Reply(QQMsg list)
{
	// �õ����һ����Ϣ
	QQMsg msg = *QQMsgGetLast(&list);

	IMAGE img;
	SetWorkingImage(&img);
	setbkcolor(WHITE);
	settextcolor(BLACK);

	// �Լ�˵�Ļ������ж�
	if (msg.name == "huidong")
	{
		//return;
	}

	string strReply;

	if (msg.msg == "ǩ��")
	{
		char h[3];
		char m[3];
		char s[3];
		_itoa_s(msg.h, h, 10);
		_itoa_s(msg.min, m, 10);
		_itoa_s(msg.s, s, 10);

		strReply = "@" + msg.name + "\n����" + h + ":" + m + ":" + s + "�ɹ�ǩ��";

		Resize(&img, 200, 50);
		cleardevice();

		wchar_t wch[128] = { 0 };
		MultiByteToWideChar(CP_ACP, 0, strReply.c_str(), strlen(strReply.c_str()), wch, strlen(strReply.c_str()));    //chת����wch

		RECT rct = { 0,0,200,50 };
		drawtext(wch, &rct, DT_LEFT);

		QQAddMsg(GetImageHBitmap(img));
		QQAddMsg("ǩ���ɹ�");
		QQFlushMsg();
	}

	// ��Ϸָ����
	else if (msg.msg[0] == '.')
	{
		GameResponse(list);
	}

	else
	{
		strReply = msg.name + "˵��" + msg.msg;
		//QQSendMsg(strReply.c_str());
	}
}


int main()
{
	// ʹ��Ԥ����������˵�
	QQStartMenu();

	// ����õ���QQ��Ϣ
	string msg;

	// QQ��Ϣ����ͷ
	QQMsg head;

	while (true)
	{
		// ʹ��Ԥ����ĺ����жϻ������Ƿ���Ҫ����
		if (QQIsEnd())
		{
			break;
		}

		// ʹ��Ԥ����ĺ����жϻ������Ƿ���Ҫ��ͣ
		QQIsPause();

		// �õ�����QQ��Ϣ
		if (QQGetMsg(msg))
		{
			// ������QQ��Ϣ����������
			QQLexMessage(msg, &head);

			// ������Ӧ����
			Reply(head);
		}
	}

	return 0;
}
