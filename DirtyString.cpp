#include "DirtyString.h"

// �ж�һ�ַ����Ƿ�Ϊ�໰
bool isDirtyString(const char* str)
{
	const int nums = 14;

	// �໰��
	char strDirty[nums][24] = {
	"����", "�ҿ�", "���", "ɵ��", "sb", "SB", "ܳ",
	"����", "�в�", "��ȥ", "���", "woc", "wr", "ţ��"
	};

	for (int i = 0; i < nums; i++)
	{
		if (strstr(str, strDirty[i]))
		{
			return true;
		}
	}

	return false;
}
