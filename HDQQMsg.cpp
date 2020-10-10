///////////////////////////////////////////////
//
//	HDQQMsg.cpp
//	huidong QQ机器人库（非官方接口版）
//
//	by huidong <mailkey@yeah.net>
//
//	Ver 0.3
//	创建时间		2020.8.9
//	最后一次修改	2020.8.19
//

#include "HDQQRobot.h"

// 得到QQ消息链表中的最后一条消息
QQMsg* QQMsgGetLast(QQMsg* list)
{
	QQMsg* p = list;
	while(p->next != NULL)
		p = p->next;
	return p;
}

// 在QQ消息链表的末尾添加一个节点，返回该节点地址
QQMsg* QQMsgAddNode(QQMsg* list)
{
	QQMsg* p = new QQMsg;
	QQMsg* last = QQMsgGetLast(list);
	p->prev = last;
	last->next = p;

	return p;
}
