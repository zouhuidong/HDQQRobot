#include "DirtyString.h"

// 判断一字符串是否为脏话
bool isDirtyString(const char* str)
{
	const int nums = 14;

	// 脏话表
	char strDirty[nums][24] = {
	"我日", "我靠", "妈的", "傻逼", "sb", "SB", "艹",
	"智障", "有病", "我去", "妈逼", "woc", "wr", "牛逼"
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
