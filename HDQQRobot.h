///////////////////////////////////////////////
//
//	HDQQRobot.h
//	huidong QQ�����˿⣨�ǹٷ��ӿڰ棩
//
//	by huidong <mailkey@yeah.net>
//
//	Ver 0.3
//	����ʱ��		2020.8.9
//	���һ���޸�	2020.8.19
//

#pragma once

#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

#include <string>
using namespace std;


// type define

struct QQMsg
{
	string name;
	string msg;
	
	int y;
	int m;
	int d;
	int h;
	int min;
	int s;

	QQMsg* prev = NULL;
	QQMsg* next = NULL;
};


// functions

void QQSetKbDelay(int ms);
int QQGetKbDelay();

void QQGotoMsgWnd();
void QQGotoSendMsgWnd();

bool QQGetMsg(string & msg);
void QQLexMessage(string strMsg, QQMsg* list);

void QQSendMsg(string msg);

QQMsg* QQMsgGetLast(QQMsg* list);
QQMsg* QQMsgAddNode(QQMsg* list);


// default function

void QQStartMenu();
bool QQIsEnd();
void QQIsPause();

