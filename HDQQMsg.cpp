///////////////////////////////////////////////
//
//	HDQQMsg.cpp
//	huidong QQ�����˿⣨�ǹٷ��ӿڰ棩
//
//	by huidong <mailkey@yeah.net>
//
//	Ver 0.3
//	����ʱ��		2020.8.9
//	���һ���޸�	2020.8.19
//

#include "HDQQRobot.h"

// �õ�QQ��Ϣ�����е����һ����Ϣ
QQMsg* QQMsgGetLast(QQMsg* list)
{
	QQMsg* p = list;
	while(p->next != NULL)
		p = p->next;
	return p;
}

// ��QQ��Ϣ�����ĩβ���һ���ڵ㣬���ظýڵ��ַ
QQMsg* QQMsgAddNode(QQMsg* list)
{
	QQMsg* p = new QQMsg;
	QQMsg* last = QQMsgGetLast(list);
	p->prev = last;
	last->next = p;

	return p;
}
