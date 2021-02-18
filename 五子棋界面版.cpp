#include<iostream>
#include<easyx.h>			// easyx头文件
#include<conio.h>			// 控制台输入输出
// 以下三个为PlaySound()函数的头文件 顺序不能错
#include <Windows.h>	
#include <mmsystem.h>
#pragma comment(lib,"WINMM.LIB")

using namespace std;

#define width  560				// 棋盘宽度
#define height 560				// 棋盘高度
#define MAX  15					// 每一行棋子个数
#define unit (width/(MAX-1))	// 单元格子大小
#define chess_r unit/2			// 棋子半径

MOUSEMSG m;						// 鼠标类 从easyx.h里面调用
char chess_pos[MAX][MAX];		// 棋盘数组
int x_sim;						// 鼠标取到横坐标后靠近最近棋子应该落点的位置
int y_sim;						// 鼠标取到纵坐标后靠近最近棋子应该落点的位置
int next_x;						// 对于左斜右斜 需要找当前点的下一个点
int next_y;						// 对于左斜右斜 需要找当前点的下一个点
int cnt;						// 记录查找时符合条件的棋子的个数
char s[5];						// 用来配合outtextxy() 输出当前是第几步
int num_step;					// 用来配合outtextxy() 输出当前是第几步
int step = 1;
void music()					// 下每一步棋的音效
{
	PlaySound(TEXT("chess.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void draw_chessboard()			// 早期用来绘制棋盘的函数	
{								// 后来因为直接用图片了 就不用它了
	setlinecolor(BLACK);

	for (int i = 0; i < MAX; i++)
	{
		line(1 + i * unit, 1, 1 + i * unit, unit * (MAX - 1));
		line(1, 1 + i * unit, unit * (MAX - 1), 1 + i * unit);
	}

}

void InitChess_pos()			// 初始化棋盘 先在里面赋空格 后面会由'B'和'W'替换
{
	for (int i = 0; i < MAX; i++)
	{
		for (int j = 0; j < MAX; j++)
		{
			chess_pos[i][j] = ' ';
		}
	}
}

int Judge(int x, int y)			// 五连子判断
{	
	//横向
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
	//纵向
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

	//左斜
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

	//左斜

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

	//右斜
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

void InitAllThings()			// 此函数将大部分初始化步骤包含在了一起
{
	initgraph(width + 160, height + 40);	// easyx的窗口函数
	setbkcolor(GREEN);						// easyx的窗口背景颜色函数
	cleardevice();							// 清除屏幕和图形缓冲区 注意前后顺序
	loadimage(NULL, _T("3.png"));			// 加载背景图片
	InitChess_pos();						// 调用初始化棋子函数
}

void regret(int x_sim,int y_sim)			// 悔棋函数
{											
	chess_pos[y_sim][x_sim] = ' ';						// 如果悔棋 先将棋子坐标重置
	clearcircle(x_sim * unit, y_sim * unit, chess_r);	//再清除原来绘制的圆
}

int main()
{	
	InitAllThings();						// 初始化函数

	while (step <= 255)						// 当步数小于255（格子总数）
	{	
		settextstyle(30, 0, "楷体");		// 字体类型	（下面三行详见easyx文档）
		//setbkmode(TRANSPARENT);
		num_step = step;					// 变量赋值
		sprintf_s(s, "%d", num_step);		// 转换类型
		outtextxy(620, 250, s);				// 在屏幕上输出字符串
		FlushBatchDraw();					// 我也不知道这个有啥用2333

		if (step % 2 == 1)					// 奇数 白棋
		{	
			setfillcolor(BLACK);
			fillcircle(637, 434, chess_r);
			m = GetMouseMsg();
			switch (m.uMsg)
			{
			case WM_LBUTTONDOWN:
				x_sim = (int)((double)m.x / unit + 0.5);
				y_sim = (int)((double)m.y / unit + 0.5);
				//cout << x_sim << " " << y_sim << endl;			// 用于看坐标用
				if (chess_pos[y_sim][x_sim] == ' ' && y_sim >= 0 && y_sim < MAX && x_sim >= 0 && x_sim < MAX)
				{
					chess_pos[y_sim][x_sim] = 'b';
					fillcircle(x_sim * unit, y_sim * unit, chess_r);
					music();
					if (Judge(y_sim, x_sim))
					{
						sprintf_s(s, "%d", num_step + 1);
						outtextxy(620, 250, s);
						MessageBoxA(0, "黑棋玩家胜利！", "游戏结束", MB_OK);
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
			setfillcolor(WHITE);			// 这两步是在屏幕上显示当前是哪个颜色的棋子走
			fillcircle(637, 434, chess_r);
			m = GetMouseMsg();				// 获取鼠标类
			switch (m.uMsg)					// 鼠标类的应用 详见easyx文档
			{
			case WM_LBUTTONDOWN:
				x_sim = (int)((double)m.x / unit + 0.5); // 详见上面定义
				y_sim = (int)((double)m.y / unit + 0.5);
				//cout << x_sim << " "<< y_sim << endl;  // 用于看坐标用
				if (chess_pos[y_sim][x_sim] == ' ' && y_sim >= 0 && y_sim < MAX && x_sim >= 0 && x_sim < MAX)
				{					// 如果当前坐标是空格（意思就是没有被使用过）并且没有越界
					chess_pos[y_sim][x_sim] = 'w';	// 白色 赋值W
					//setfillcolor(WHITE);
					fillcircle(x_sim * unit, y_sim * unit, chess_r);	// 填充
					music();		// 下棋音效
					if (Judge(y_sim, x_sim))		// 每下一步 判断当前棋子的上下左右和两个斜方向是否有五连子
					{
						sprintf_s(s, "%d", num_step + 1);			// 在屏幕上显示的当前步数+1
						outtextxy(620, 250, s);
						MessageBoxA(0, "白棋玩家胜利！", "游戏结束！", MB_OK);			// 弹出框框 游戏结束
						return 0;
					}
					step++;
				}
				break;
				//case WM_RBUTTONDOWN:			// 用于看坐标用 
					//return 0;
			}
		}
		if (_kbhit()) {				// 当键盘按下R键 执行悔棋
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