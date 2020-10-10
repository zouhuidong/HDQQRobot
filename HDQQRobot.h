///////////////////////////////////////////////
//
//	HDQQRobot.h
//	huidong QQ�����˿⣨�ǹٷ��ӿڰ棩
//
//	by huidong <mailkey@yeah.net>
//
//	Ver 0.2
//	����ʱ��		2020.8.9
//	���һ���޸�	2020.8.9
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

