///////////////////////////////////////////////
//
//	HDQQRobot.h
//	huidong QQ机器人库（非官方接口版）
//
//	by huidong <mailkey@yeah.net>
//
//	Ver 0.5
//	创建时间		2020.8.9
//	最后一次修改	2020.8.26
//

#pragma once

#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

#include <string>
using namespace std;


// type define

// QQ消息链表结构体
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

string QQGetVersionString();
float QQGetVersionFloat();

void QQSetMyName(string strName);
string QQGetMyName();

void QQSetIsGetMyMsg(bool isGetMyMsg);
bool QQGetIsGetMyMsg();

void QQSetKbDelay(int ms);
int QQGetKbDelay();

bool QQGetMsg();

void QQAddMsg(string msg);
void QQAddMsg(HBITMAP bitmap_msg);
void QQFlushMsg();

void QQSendMsg(string msg);
void QQSendMsg(HBITMAP bitmap_msg);

QQMsg* QQGetMsgHead();
QQMsg* QQGetLastMsg();


// default function

void QQStartMenu();
bool QQIsEnd();
void QQIsPause();


// Others

bool QQMessageBox(string strMessage);
