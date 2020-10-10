#include "woof.h"

int main()
{
	printf("输入你的QQ用户名，然后回车：");
	char name[128] = { 0 };
	scanf_s("%s", name, 128);
	printf("\n");

	QQStartMenu();

	while (true)
	{
		if (QQIsEnd())
		{
			break;
		}

		QQIsPause();

		if (QQGetMsg())
		{
			if (QQGetLastMsg()->msg == "狼人杀")
			{
				woof_game(name);
			}
		}
	}
}