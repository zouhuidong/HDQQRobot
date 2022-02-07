# HDQQRobot
![Downloads](https://img.shields.io/github/downloads/zouhuidong/HDQQRobot/total)
![GitHub Version](https://img.shields.io/github/v/release/zouhuidong/HDQQRobot)
[![Blog](https://img.shields.io/badge/blog-huidong.xyz-green.svg)](http://huidong.xyz)

HuiDong QQ Robot —— 非官方接口QQ机器人 

---

QQ虽然有官方的机器人接口，但是使用它的接口需要申请，很麻烦。 
于是我想，可以用粗暴的方式实现第三方（非官方）机器人，虽然功能没有那么全面，但是还是够用，还不会被腾讯限制。

原理是用win api模拟鼠标移动到QQ窗口，不停地模拟键盘按下 Ctrl + A , Ctrl + C 把消息内容放到剪贴板，然后再从剪贴板取消息内容
取得消息内容后进行分析，以获取消息的详细信息
得到消息之后可以做回复，回复的原理是把消息内容放进剪贴板，再模拟鼠标点击消息输入框，再然后模拟按下Ctrl + V 和 Enter 来发送消息。
所以，这个机器人的运行需要占用鼠标、键盘以及剪贴板，可以开个虚拟机挂在后台。

此项目在 VS2019 编译通过，win7 和 win10 + TIM（QQ的办公版）测试通过。

请见此连接：
http://www.huidong.xyz/?mode=2&id=137

示例代码（基于Ver0.5）：
这是一个一直发送【戳一戳】消息的Rot代码
```C++
#include "HDQQRobot.h"

int main()
{
	// 使用预设方式配置QQRobot
	QQStartMenu();

	string str;
	char num[12] = { 0 };
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
		memset(num, 0, 12);
		_itoa_s(n, num, 12, 10);
		str += num;
		str += "戳]请使用新版手机QQ查看";

		// 输出消息
		QQSendMsg(str);

		Sleep(200);

		n++;
	}
	
	return 0;
}

```

### 操作说明：

![](http://www-x-huidong-x-xyz.img.abc188.com/ueditor/php/upload/image/20200809/1596963155321576.png)

首先：将鼠标移动到QQ消息窗口上，按下F11，注意：按下F11的时候不需要点击cmd窗口，程序会全局捕获按键，下面也一样，按键都是全局捕获的。

然后将鼠标移动到QQ输入框上，按下F12。

因为这个机器人的实现需要模拟鼠标键盘，所以按F11的时候推荐将鼠标放到消息框的右下角，这样鼠标不会点到消息

按F12的时候，就把鼠标放在输入框内就好，没什么要特别注意的。

如下图：

![](http://www-x-huidong-x-xyz.img.abc188.com/ueditor/php/upload/image/20200809/1596962955344082.png)

然后按下F10开启机器人

按下F8暂停机器人，然后按下F7使机器人继续工作（Ver 0.2新增）

按下F9来停止机器人（可能需要长按一下）

开启机器人之后，你无法控制你的鼠标，如果需要停止，请务必按F9

保证启动机器人时，没有别的程序有快捷键冲突。

有的时候，开启了机器人，对方说了指令，可机器人没反应怎么办？

下面的教程很重要：

两种可能：

1. 鼠标位置不太对，【稍微】 移动鼠标，然后可能就可以

2. 模拟 Ctrl+C 时复制聊天内容失败，此时请先按F9关闭机器人，然后在一个文本框按Ctrl+V检测剪贴板内是否有信息，

如果没有信息，说明Ctrl+C时获取信息失败，措施如下：

    先看下机器人启动的时候输入法关了没，如果没关，把它关掉。

    然后手动在QQ消息框里Ctrl + A, Ctrl + C，再到一个文本框里粘贴出来看看有没有信息，如果有信息，那可以了，重启机器人再继续就行。

    如果手动复制出来什么也没有，说明你可能打开了什么程序，检查一下，把它关掉再试。

还不行的话，重启电脑试试。
