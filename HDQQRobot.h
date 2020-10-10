///////////////////////////////////////////////
//
//	HDQQRobot.h
//	huidong QQ机器人库（非官方接口版）
//
//	by huidong <mailkey@yeah.net>
//
//	Ver 0.2
//	创建时间		2020.8.9
//	最后一次修改	2020.8.9
//

#pragma once

#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

#include <string>
using namespace std;


// functions

void HDQQSetResponseFunc(void (*func)(const char*));
void* HDQQGetResponseFunc();

void HDQQSetKbDelay(int ms);
int HDQQGetKbDelay();

void HDQQGotoMsgWnd();
void HDQQGotoSendMsgWnd();

void HDQQGetMsg(char* msg, const int size);
void HDQQLexMessage(const char* pMessage);

void HDQQSendMsg(const char* msg);


// default function

void HDQQStartMenu();
bool HDQQIsEnd();
void HDQQIsPause();

