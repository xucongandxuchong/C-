#include <stdlib.h>
#include <time.h>
#include "performance.h"

PIMAGE img[COLOR_BLOCKS_NUMBER + 1];	// ͼƬָ�����飬��ż���ͼƬ�ĵ�ַ
PIMAGE background;	// ��Ϸ����ͼƬָ��
PIMAGE scoreZone;	// ��������ͼƬָ��
PIMAGE timeZone;	// ʱ�䱳��ͼƬָ��
PIMAGE leaveChannel;	// �˳�ͨ��ͼƬָ��
char szBuffer[BUFSIZ];	// ��Ϸ�ַ���ר�û�����
static mouse_msg msg, msg1, msg2;	// mouse_msg�����Ϣ�ṹ�����
static int clicksNumber;	// ���������������ɫ�����������

// ��ʼ����Ϸ����
void initEnviron()
{
	srand((unsigned)time(NULL));	// �������������
	score = 0;	// �÷�����
	leftTime = INITIAL_TIME;	// ��ʼʣ��ʱ��
	// �����鸳ֵ����ʼ����ͼ״̬
	for (int i = 0; i < MAP_SIZE; ++i)
	for (int j = 0; j < MAP_SIZE; ++j)
		map[i][j] = rand() % COLOR_BLOCKS_NUMBER + 1;	// rand��ȡ�������ȡ�õ���������
	// �����Ϣλ�ó�ʼΪ-COLOR_BLOCK_PIXELS�������ж��Ƿ������Ч�����Ϣ
	msg1.x = msg1.y = msg2.x = msg2.y = -COLOR_BLOCK_PIXELS;
	clicksNumber = 0;	// ��ʼ���������
}

// ��ʾ��Ϸ��Ϣ
void displayInfo()
{
	// ��ӡ����
	putimage_transparent(NULL, scoreZone, 25, 30, BLACK);
	rectprintf(47, 60, 54, 40, "%3d", score);
	// ��ӡʱ��
	putimage_transparent(NULL, timeZone, 25, 190, BLACK);
	rectprintf(58, 220, 36, 40, "%2.0lf", leftTime);
}

// ��ͼ����Դ�ɵ���rc�ļ���
void draw()
{
	cleardevice();	// ����
	putimage(0, 0, background);	// ����ͼ����
	for (int i = 0; i < MAP_SIZE; ++i)
	for (int j = 0; j < MAP_SIZE; ++j)
		// ����Ļ����͸����ʽ����ָ��ͼ�����map[i][j]==0��û�ж�Ӧ��ͼ��������������
		putimage_transparent(NULL, img[map[i][j]], COLOR_BLOCK_PIXELS * i + MAP_X, COLOR_BLOCK_PIXELS * j + MAP_Y, BLACK);
	putimage_transparent(NULL, leaveChannel, 25, 360, BLACK);	// ��ʾ�˳���Ϸͨ��
	displayInfo();	// ��ʾ��Ϸ��Ϣ
	delay_jfps(10);	// ƽ���ӳ�1000/fps���룬�����ȶ��߼�֡�ʿ��ƣ���ͼ����֡������fps��10
}

// ���������ɫ��
void fill()
{
	// �ӵ�ͼ�����濪ʼ���������μ��һ��
	for (int j = MAP_SIZE - 1; j >= 0; --j)
	{
		for (int i = MAP_SIZE - 1; i >= 0; --i)
		{
			while (map[i][j] == 0)	// �жϵ�ǰλ���Ƿ�Ϊ��
			{
				// ɫ�����µ�
				for (int k = j; k > 0; --k)
					map[i][k] = map[i][k - 1];
				map[i][0] = rand() % COLOR_BLOCKS_NUMBER + 1;	// ������һ�еĿ�λ���������ɫ��
				draw();	// ÿ��ɫ��仯�󣬻��Ƴ���ǰ��ͼ
			}
		}
	}
}

// ��ҿ��Ʋ���
int play()
{
	static int x1, y1, x2, y2;
	// δ���������������������Ϣ
	if (!mousemsg() || msg1.x != -COLOR_BLOCK_PIXELS && msg2.x != -COLOR_BLOCK_PIXELS)
		return 0;
	msg = getmouse();	// ��ȡ�����Ϣ
	// �������
	if (!(msg.is_down() && msg.is_left()))	// ����������
		return 0;
	if (contain(LEAVE_X, LEAVE_Y, LEAVE_PIXELS, LEAVE_PIXELS, msg.x, msg.y))	// ����˳���Ϸ����
		return ESC;	// �����˳���Ϸ��Ϣ
	if (!contain(MAP_X, MAP_Y, MAP_PIXELS, MAP_PIXELS, msg.x, msg.y))	// ����ɫ��������
		return -1;
	++clicksNumber;	// �����������һ
	if (clicksNumber == 1)	// ��һ�ε��
	{
		// �����������ɫ���Ľǵ�����
		x1 = (msg.x - MAP_X) / COLOR_BLOCK_PIXELS * COLOR_BLOCK_PIXELS + MAP_X;
		y1 = (msg.y - MAP_Y) / COLOR_BLOCK_PIXELS * COLOR_BLOCK_PIXELS + MAP_Y;
		x2 = x1 + COLOR_BLOCK_PIXELS;
		y2 = y1 + COLOR_BLOCK_PIXELS;
		rectangle(x1, y1, x2, y2);	// ���ƿ��ľ��Σ���ʾ�û���һ�ε����ɫ��
		msg1 = msg;
		return 1;	// ���أ��ȴ��û���һ�ε��ɫ��
	}
	else	// �ڶ��ε��
	{
		msg2 = msg;
		clicksNumber = 0;	// �����������ε���������ָ���ʼ״̬
		draw();	// ���ƽ���
	}
	// ��������ӳ���ɫ������±�
	x1 = (msg1.x - MAP_X) / COLOR_BLOCK_PIXELS;
	y1 = (msg1.y - MAP_Y) / COLOR_BLOCK_PIXELS;
	x2 = (msg2.x - MAP_X) / COLOR_BLOCK_PIXELS;
	y2 = (msg2.y - MAP_Y) / COLOR_BLOCK_PIXELS;
	msg1.x = msg1.y = msg2.x = msg2.y = -COLOR_BLOCK_PIXELS;	// �ﵽһ�ν��������������λ����Ϣ�ָ���ԭʼ״̬
	// �������Ҿ������ڣ�ֱ���˳�
	if (!adjacent(x1, y1, x2, y2))
		return -1;
	swap(&map[x1][y1], &map[x2][y2]);	// ������ɫ��λ��
	draw();	// ����ɫ��λ�ý�����ĵ�ͼ
	if (clear() == 0)	// ����֮�󣬷���0˵��û������ɫ��
	{
		swap(&map[x1][y1], &map[x2][y2]);	// �ٴν�����ȥ
		draw();	// ���һ��Ƴ�������ĵ�ͼ
	}
	else	// ������0������������
		fill();	// �����ȥ�Ĳ���
	return 2;
}

// ��Ϸ��ֻ���
void displayEnd()
{
	if (score < SCORE_UPPER_LIMIT)
		sprintf(szBuffer, "The final score is %d", score);	// �����յ÷���ʾд�뻺����
	else
		strcpy(szBuffer, "Congratulations!");
	int len = strlen(szBuffer) * 18;	// ������ʾ��Ϣ����
	setfillcolor(WHITE);	// ���õ�ǰ�����ɫ
	bar(0, (WINDOW_HEIGHT - 50) / 2, WINDOW_WIDTH, (WINDOW_HEIGHT - 50) / 2 + 50);	// ���ƾ���
	rectprintf((WINDOW_WIDTH - len) / 2, (WINDOW_HEIGHT - 36) / 2, len, 36, szBuffer);	// ������յ÷���ʾ
}