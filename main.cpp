#include "HDQQRobot.h"
#include "maze.h"
#include "DirtyString.h"

// ������ģʽ����

#define MODE_NULL 0
#define MODE_MAZE 1
#define MODE_OTHER 2

int m_mode = MODE_NULL;

// ��Ϣ��Ӧ����
void ResponseMsg(const char* msg)
{
	string strMsg = msg;

	// �ж���Ϣ�����д���
	
	if (strMsg == "awa")
	{
		HDQQSendMsg("ι����110��������������");
	}

	// �����໰
	if (isDirtyString(msg))
	{
		HDQQSendMsg("�벻Ҫ˵�໰");
	}

	// �Թ�
	else if (strMsg == "�Թ�")
	{
		if (m_mode == MODE_MAZE)
		{
			HDQQSendMsg("�Թ���Ϸ�رգ�");
			m_mode = MODE_NULL;
		}
		else if(m_mode == MODE_NULL)
		{
			HDQQSendMsg("�Թ���Ϸ������\n�ٴ�����\"�Թ�\"���ر���Ϸ��\n�����������������ƽ�ɫ�ƶ���\n��Ŀ��ģ�\n����huidong");
			m_mode = MODE_MAZE;
			Maze_Init();
		}
	}

	// ����ģʽ���˴������ӣ��������δд��
	else if (strMsg == "����")
	{
		if (m_mode == MODE_OTHER)
		{
			// ������Ӧ��Ϣ
			//HDQQSendMsg("��������");
			m_mode = MODE_NULL;
		}
		else if (m_mode == MODE_NULL)
		{
			m_mode = MODE_OTHER;
			
			// ������룺
		}
	}

	// ��ǰģʽ�жϣ���������Ϣģ��
	else
	{
		switch (m_mode)
		{
		case MODE_MAZE:
			Maze_ResponseMsg(msg);	// �Թ�ģʽ�£������Թ�������
			break;

		case MODE_OTHER:
			// ��������ģʽ����Ϣ��Ӧ����
			break;
		}
	}
}


int main()
{
	// ������Ϣ��Ӧ����
	HDQQSetResponseFunc(ResponseMsg);
	// ʹ��Ԥ����������˵�
	HDQQStartMenu();

	const int size = 1024000;
	char msg[size] = { 0 };

	while (true)
	{	
		// ʹ��Ԥ����ĺ����жϻ������Ƿ���Ҫ����
		if (HDQQIsEnd())
		{
			break;
		}

		// ʹ��Ԥ����ĺ����жϻ������Ƿ���Ҫ��ͣ
		HDQQIsPause();

		// �õ�һ��QQ��Ϣ
		HDQQGetMsg(msg, size);

		// ������QQ��Ϣ�����͵���Ӧ����
		HDQQLexMessage(msg);
	}

	return 0;
}
