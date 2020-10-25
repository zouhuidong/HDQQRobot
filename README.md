# HDQQRobot

HuiDong QQ Robot —— 非官方接口QQ机器人 
---

QQ虽然有官方的机器人接口，但是使用它的接口需要申请，很麻烦。 
于是我想，可以用粗暴的方式实现第三方（非官方）机器人，虽然功能没有那么全面，但是还是够用，还不会被腾讯限制。

原理是用win api模拟鼠标移动到QQ窗口，不停地模拟键盘按下 Ctrl + A , Ctrl + C 把消息内容放到剪贴板，然后再从剪贴板取消息内容
取得消息内容后进行分析，以获取消息的详细信息
得到消息之后可以做回复，回复的原理是把消息内容放进剪贴板，再模拟鼠标点击消息输入框，再然后模拟按下Ctrl + V 和 Enter 来发送消息。
所以，这个机器人的运行需要占用鼠标、键盘以及剪贴板，可以开个虚拟机挂在后台。

此项目在 VS2019 编译通过，win7 和 win10 + TIM（QQ的办公版）测试通过。

示例代码（基于Ver0.5）：
这是一个一直发送【戳一戳】消息的Rot代码
```C++
#include "HDQQRobot.h"

int main()
{
	// 使用预设方式配置QQRobot
	QQStartMenu();

	string str;
	int n = 1;

	while(true)
	{
		// 以预设方式判断是否要求停止QQRobot
		if (QQIsEnd())
			break;

		// 以预设方式判断是否要求暂停QQRobot
		// 如果需要暂停，会直接在函数内部处理暂停事件
		QQIsPause();

		str = "[戳";
		str += itoa(n, NULL, 10);
		str += "戳]请使用新版手机QQ查看";

		// 输出消息
		QQSendMsg(str);

		Sleep(800);
	}
	
	return 0;
}

```

http://www.huidong.xyz/?mode=2&id=137
