#include "HDQQRobot.h"
#include "game.h"

#include "EasyXHBitmap.h"

// 回复消息
void Reply(QQMsg list)
{
	// 得到最后一条消息
	QQMsg msg = *QQMsgGetLast(&list);

	IMAGE img;
	SetWorkingImage(&img);
	setbkcolor(WHITE);
	settextcolor(BLACK);

	// 自己说的话则不予判断
	if (msg.name == "huidong")
	{
		//return;
	}

	string strReply;

	if (msg.msg == "签到")
	{
		char h[3];
		char m[3];
		char s[3];
		_itoa_s(msg.h, h, 10);
		_itoa_s(msg.min, m, 10);
		_itoa_s(msg.s, s, 10);

		strReply = "@" + msg.name + "\n你在" + h + ":" + m + ":" + s + "成功签到";

		Resize(&img, 200, 50);
		cleardevice();

		wchar_t wch[128] = { 0 };
		MultiByteToWideChar(CP_ACP, 0, strReply.c_str(), strlen(strReply.c_str()), wch, strlen(strReply.c_str()));    //ch转换到wch

		RECT rct = { 0,0,200,50 };
		drawtext(wch, &rct, DT_LEFT);

		QQAddMsg(GetImageHBitmap(img));
		QQAddMsg("签到成功");
		QQFlushMsg();
	}

	// 游戏指令区
	else if (msg.msg[0] == '.')
	{
		GameResponse(list);
	}

	else
	{
		strReply = msg.name + "说：" + msg.msg;
		//QQSendMsg(strReply.c_str());
	}
}


int main()
{
	// 使用预定义的启动菜单
	QQStartMenu();

	// 保存得到的QQ消息
	string msg;

	// QQ消息链表头
	QQMsg head;

	while (true)
	{
		// 使用预定义的函数判断机器人是否需要结束
		if (QQIsEnd())
		{
			break;
		}

		// 使用预定义的函数判断机器人是否需要暂停
		QQIsPause();

		// 得到所有QQ消息
		if (QQGetMsg(msg))
		{
			// 分析此QQ消息并加入链表
			QQLexMessage(msg, &head);

			// 调用响应函数
			Reply(head);
		}
	}

	return 0;
}
