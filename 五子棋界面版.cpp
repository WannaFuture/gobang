#include<iostream>
#include<easyx.h>			// easyxͷ�ļ�
#include<conio.h>			// ����̨�������
// ��������ΪPlaySound()������ͷ�ļ� ˳���ܴ�
#include <Windows.h>	
#include <mmsystem.h>
#pragma comment(lib,"WINMM.LIB")

using namespace std;

#define width  560				// ���̿��
#define height 560				// ���̸߶�
#define MAX  15					// ÿһ�����Ӹ���
#define unit (width/(MAX-1))	// ��Ԫ���Ӵ�С
#define chess_r unit/2			// ���Ӱ뾶

MOUSEMSG m;						// ����� ��easyx.h�������
char chess_pos[MAX][MAX];		// ��������
int x_sim;						// ���ȡ��������󿿽��������Ӧ������λ��
int y_sim;						// ���ȡ��������󿿽��������Ӧ������λ��
int next_x;						// ������б��б ��Ҫ�ҵ�ǰ�����һ����
int next_y;						// ������б��б ��Ҫ�ҵ�ǰ�����һ����
int cnt;						// ��¼����ʱ�������������ӵĸ���
char s[5];						// �������outtextxy() �����ǰ�ǵڼ���
int num_step;					// �������outtextxy() �����ǰ�ǵڼ���
int step = 1;
void music()					// ��ÿһ�������Ч
{
	PlaySound(TEXT("chess.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void draw_chessboard()			// ���������������̵ĺ���	
{								// ������Ϊֱ����ͼƬ�� �Ͳ�������
	setlinecolor(BLACK);

	for (int i = 0; i < MAX; i++)
	{
		line(1 + i * unit, 1, 1 + i * unit, unit * (MAX - 1));
		line(1, 1 + i * unit, unit * (MAX - 1), 1 + i * unit);
	}

}

void InitChess_pos()			// ��ʼ������ �������渳�ո� �������'B'��'W'�滻
{
	for (int i = 0; i < MAX; i++)
	{
		for (int j = 0; j < MAX; j++)
		{
			chess_pos[i][j] = ' ';
		}
	}
}

int Judge(int x, int y)			// �������ж�
{	
	//����
	cnt = 0;
	for (int i = 0; i < MAX; i++)
	{	
		if (chess_pos[x][i] == chess_pos[x][y])
		{
			for (int j = 1; j <= 4; j++)
			{
				if (chess_pos[x][i + j] == chess_pos[x][y])
					cnt++;
				else
					cnt = 0;
			}
		}
		if (cnt >= 4)
			return 1;
	}
	//����
	cnt = 0;
	for (int i = 0; i < MAX; i++)
	{	
		if (chess_pos[i][y] == chess_pos[x][y])
		{
			for (int j = 1; j <= 4; j++)
			{
				if (chess_pos[i+j][y] == chess_pos[x][y])
					cnt++;
				else
					cnt = 0;
			}
		}
		if (cnt >= 4)
			return 1;
	}

	//��б
	//cnt = 0;
	//next_x = x + y;
	//for (int i = 0; i < next_x; i++)
	//{
	//	if (chess_pos[0 + i][next_x - i] == chess_pos[x][y])
	//	{
	//		for (int j = 1; j <= 4; j++)
	//		{
	//			if (chess_pos[0 + i + j][next_x - i - j] == chess_pos[x][y])
	//				cnt++;
	//			else
	//				cnt = 0;
	//		}
	//	}
	//	if (cnt >= 4)
	//		return 1;
	//}

	//��б

	cnt = 0;
	next_x = x + 1;
	next_y = y - 1;
	while (next_x<MAX && next_y>-1 && chess_pos[next_x][next_y] == chess_pos[x][y])
	{
		next_x++;
		next_y--;
		cnt++; 
	}

	next_x = x - 1;
	next_y = y + 1;
	while (next_x > -1 && next_y < MAX && chess_pos[next_x][next_y] == chess_pos[x][y])
	{
		next_x--;
		next_y++;
		cnt++; 
	}

	if (cnt >= 4)
		return 1; 

	//��б
	cnt = 0;
	next_x = x - 1;
	next_y = y - 1;
	while (next_x > -1 && next_y > -1 && chess_pos[next_x][next_y] == chess_pos[x][y])
	{
		next_x--;
		next_y--;
		cnt++; 
	}
	next_x = x + 1;
	next_y = y + 1;
	while (next_x < MAX && next_y < MAX && chess_pos[next_x][next_y] == chess_pos[x][y])
	{
		next_x++;
		next_y++;
		cnt++; 
	}

	if (cnt >= 4)
		return 1; 
	
	/*for (int i = 0; i < 8; i++)
	{
		int cnt = 0;
		for (int j = 1; j <= 4; j++)
		{
			int dx = x + j * xx[i];
			int dy = y + j * yy[i];
			if ((chess_pos[dx][dy] == chess_pos[x][y]) && dx >= 0 && dx < MAX && dy >= 0 && dy < MAX)
			{
				cnt++;
			}
		}
		if (cnt == 4)
		{
			return 1;
		}
	}
	return 0;*/
	return 0;
}

void InitAllThings()			// �˺������󲿷ֳ�ʼ�������������һ��
{
	initgraph(width + 160, height + 40);	// easyx�Ĵ��ں���
	setbkcolor(GREEN);						// easyx�Ĵ��ڱ�����ɫ����
	cleardevice();							// �����Ļ��ͼ�λ����� ע��ǰ��˳��
	loadimage(NULL, _T("3.png"));			// ���ر���ͼƬ
	InitChess_pos();						// ���ó�ʼ�����Ӻ���
}

void regret(int x_sim,int y_sim)			// ���庯��
{											
	chess_pos[y_sim][x_sim] = ' ';						// ������� �Ƚ�������������
	clearcircle(x_sim * unit, y_sim * unit, chess_r);	//�����ԭ�����Ƶ�Բ
}

int main()
{	
	InitAllThings();						// ��ʼ������

	while (step <= 255)						// ������С��255������������
	{	
		settextstyle(30, 0, "����");		// ��������	�������������easyx�ĵ���
		//setbkmode(TRANSPARENT);
		num_step = step;					// ������ֵ
		sprintf_s(s, "%d", num_step);		// ת������
		outtextxy(620, 250, s);				// ����Ļ������ַ���
		FlushBatchDraw();					// ��Ҳ��֪�������ɶ��2333

		if (step % 2 == 1)					// ���� ����
		{	
			setfillcolor(BLACK);
			fillcircle(637, 434, chess_r);
			m = GetMouseMsg();
			switch (m.uMsg)
			{
			case WM_LBUTTONDOWN:
				x_sim = (int)((double)m.x / unit + 0.5);
				y_sim = (int)((double)m.y / unit + 0.5);
				//cout << x_sim << " " << y_sim << endl;			// ���ڿ�������
				if (chess_pos[y_sim][x_sim] == ' ' && y_sim >= 0 && y_sim < MAX && x_sim >= 0 && x_sim < MAX)
				{
					chess_pos[y_sim][x_sim] = 'b';
					fillcircle(x_sim * unit, y_sim * unit, chess_r);
					music();
					if (Judge(y_sim, x_sim))
					{
						sprintf_s(s, "%d", num_step + 1);
						outtextxy(620, 250, s);
						MessageBoxA(0, "�������ʤ����", "��Ϸ����", MB_OK);
						return 0;
					}
					step++;
				}
				break;
				//case WM_RBUTTONDOWN:
				//	return 0;
			}
		}
		else
		{	
			setfillcolor(WHITE);			// ������������Ļ����ʾ��ǰ���ĸ���ɫ��������
			fillcircle(637, 434, chess_r);
			m = GetMouseMsg();				// ��ȡ�����
			switch (m.uMsg)					// ������Ӧ�� ���easyx�ĵ�
			{
			case WM_LBUTTONDOWN:
				x_sim = (int)((double)m.x / unit + 0.5); // ������涨��
				y_sim = (int)((double)m.y / unit + 0.5);
				//cout << x_sim << " "<< y_sim << endl;  // ���ڿ�������
				if (chess_pos[y_sim][x_sim] == ' ' && y_sim >= 0 && y_sim < MAX && x_sim >= 0 && x_sim < MAX)
				{					// �����ǰ�����ǿո���˼����û�б�ʹ�ù�������û��Խ��
					chess_pos[y_sim][x_sim] = 'w';	// ��ɫ ��ֵW
					//setfillcolor(WHITE);
					fillcircle(x_sim * unit, y_sim * unit, chess_r);	// ���
					music();		// ������Ч
					if (Judge(y_sim, x_sim))		// ÿ��һ�� �жϵ�ǰ���ӵ��������Һ�����б�����Ƿ���������
					{
						sprintf_s(s, "%d", num_step + 1);			// ����Ļ����ʾ�ĵ�ǰ����+1
						outtextxy(620, 250, s);
						MessageBoxA(0, "�������ʤ����", "��Ϸ������", MB_OK);			// ������� ��Ϸ����
						return 0;
					}
					step++;
				}
				break;
				//case WM_RBUTTONDOWN:			// ���ڿ������� 
					//return 0;
			}
		}
		if (_kbhit()) {				// �����̰���R�� ִ�л���
			char input = _getch();
			if (input == 'r')
			{	
				step--;
				regret(x_sim,y_sim);
			}
		}
	}
    _getch();
}