#include "HDQQRobot.h"

void Reply(QQMsg list)
{
	// 得到最后一条消息
	QQMsg msg = *QQMsgGetLast(&list);
	
	// 自己说的话则不予判断
	if (msg.name == "huidong")
	{
		return;
	}

	string strReply;

	strReply = msg.name + "说：" + msg.msg;
	QQSendMsg(strReply.c_str());
}


int main()
{
	// 使用预定义的启动菜单
	QQStartMenu();

	string msg;
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