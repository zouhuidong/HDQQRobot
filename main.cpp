#include "woof.h"

int main()
{
	printf("�������QQ�û�����Ȼ��س���");
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
			if (QQGetLastMsg()->msg == "����ɱ")
			{
				woof_game(name);
			}
		}
	}
}