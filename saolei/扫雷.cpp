#include <iostream>
#include <cstdlib>
#include <ctime>
#include <easyx.h>
#include <graphics.h>

using namespace std;

#define ROW 9
#define COL 9
#define BOOM 10
#define IMG_SIZE 40

void initMap(int map[ROW][COL]);
void mouseEvent(int map[ROW][COL]);
void drawMap(int map[ROW][COL], IMAGE img[]);
void open(int map[ROW][COL], int row, int col);
void judge(int map[ROW][COL], int row, int col);
void judge_game(int map[ROW][COL],IMAGE img[]);

bool isOver = false;
int  n_count = 0;
IMAGE img[12];


void initMap(int map[ROW][COL])
{
	for(int i = 0; i < BOOM;	)
	{
		int r = rand() % ROW;    //0 - ROW
		int c = rand() % COL;
		if (map[r][c] == 0)
		{
			map[r][c] = -1;
			i++;
		}
	}
	for(int i = 0; i < ROW; i++ )
	{
		for(int j = 0; j < COL; j++)
		{
			if(map[i][j] == -1)        //雷所在的格子的九宫格都+1;
			{
				for(int m = i - 1; m <= i + 1; m++)
				{
					for (int n = j - 1; n <= j + 1; n++)
					{
						if((m >= 0 && m < ROW && n >= 0 && n < COL) && map[m][n] != -1)  //防止越界
						{
							map[m][n]++;
						}
					}
				}
			}
		}
	}

	//+10加密
	for(int i = 0; i < ROW; i++)
	{
		for(int j = 0; j < COL; j++)
		{
			map[i][j] += 10;
		}
	}
}
void mouseEvent(int map[ROW][COL])
{
	ExMessage msg;
	if(peekmessage(&msg,EM_MOUSE))
	{
		int m = msg.y / IMG_SIZE;
		int n = msg.x / IMG_SIZE;
		if(msg.message == WM_LBUTTONDOWN)
		{
			if (map[m][n] >= 9 && map[m][n] <= 18)
			{
				n_count++;   //表示已经点开了一个格子
				map[m][n] -= 10;
				open(map, m, n);
				judge(map, m, n);
				judge_game(map,img);
			}
			else if(map[m][n] >= 1 && map[m][n] <= 8)
			{
				open(map, m, n);
				judge(map, m, n);
				judge_game(map,img);
			}
		}
		else if(msg.message == WM_RBUTTONDOWN)
		{
			if(map[m][n] >= 9 && map[m][n] <= 18)
			{
				map[m][n] += 10;     //镖旗
				
			}
			else if (map[m][n] >= 19 && map[m][n] <= 28)
			{
				map[m][n] -= 10;
			}
		}

	}
}
void drawMap(int map[ROW][COL], IMAGE img[])
{
	for(int i = 0; i < ROW; i++)
	{
		for(int j = 0; j < COL ;j ++)
		{
			if(map[i][j] >=0 && map[i][j] <= 8)  //非雷
			{
				putimage(j * IMG_SIZE, i * IMG_SIZE, &img[map[i][j]]);
			}else if(map[i][j] == -1)   //雷
			{
				putimage(j * IMG_SIZE, i * IMG_SIZE, &img[9]);
			}
			else if(map[i][j] >= 9 && map[i][j] <= 18)  //盖住
			{
				putimage(j * IMG_SIZE, i * IMG_SIZE, &img[10]);
			}
			else if(map[i][j] >= 19 && map[i][j] <= 28)
			{
				putimage(j * IMG_SIZE, i * IMG_SIZE, &img[11]);
			}
		}
	}
}
void open(int map[ROW][COL], int row, int col)
{
	if(map[row][col] == 0)
	{
		for(int i = row - 1; i <= row  + 1; i++)
		{
			for(int j = col -1 ; j <= col + 1;j++)
			{
				if((i >= 0 && i < ROW && j >= 0 && j < COL) && map[i][j] >= 9 && map[i][j] <= 18)
				{
					map[i][j] -= 10;
					n_count++;
					open(map, i, j);
				}
			}
		}
	}
	else if(map[row][col] >= 1 && map[row][col] <= 8)
	{
		bool flag = true;//表明旗插在正确的雷上
		int num = 0;
		for (int i = row - 1; i <= row + 1; i++)
		{
			for (int j = col - 1; j <= col + 1; j++)
			{
				if((i >= 0 && i < ROW && j >= 0 && j < COL) && map[i][j] > 19)
				{
					flag = false;
				}
				if((i >= 0 && i < ROW && j >= 0 && j < COL) && map[i][j] == 19)
				{
					num++;
				}
			}
		}
		if(!flag)
		{
			isOver = true;													//点错旗子了,直接游戏失败
			for(int i = 0;i < ROW; i++)
			{
				for(int j = 0 ;j < COL;j++)
				{
					if(map[i][j] == 9)
					{
						map[i][j] -= 10;
					}
				}
			}
		}
		else if (flag && num == map[row][col])                           //说明旗子都插在了正确的位置上,并且没有雷遗漏了
		{
			for (int i = row - 1; i <= row + 1; i++)
			{
				for (int j = col - 1; j <= col + 1; j++)
				{
					if ((i >= 0 && i < ROW && j >= 0 && j < COL) && map[i][j] >= 9 && map[i][j] <= 18)
					{
						map[i][j] -= 10;
						n_count++;
						open(map, i, j);
					}
				}
			}
		}
	}
}
void judge(int map[ROW][COL], int row, int col)
{
	if(map[row][col] == -1)
	{
		for (int i = 0; i < ROW; i++)
		{
			for(int j = 0; j < COL; j++)
			{
				map[i][j] -= 10;
			}
		}
		isOver = true;
	}
}
void judge_game(int map[ROW][COL], IMAGE img[])
{
	if(isOver)
	{
		drawMap(map, img);
		int ret = MessageBox(GetHWnd(), "你已经输了,是否继续游戏?", "hit", MB_OKCANCEL);
		if(ret == IDOK)
		{
			memset(map,0, ROW * COL * sizeof(int));
			initMap(map);
			isOver = false;
			n_count = 0;
		}
		else if(ret == IDCANCEL)
		{
			exit(11);
		}
	}
	if(n_count == (ROW * COL - BOOM))
	{
		int ret = MessageBox(GetHWnd(), "你已经赢了,是否继续游戏?", "hit", MB_OKCANCEL);
		if (ret == IDOK)
		{
			memset(map, 0, ROW * COL * sizeof(int));
			initMap(map);
			isOver = false;
			n_count = 0;
		}
		else if (ret == IDCANCEL)
		{
			exit(11);
		}
	}
}

int main()
{

	int map[ROW][COL] = { 0 };
	srand((unsigned)time(0));
	initgraph(COL * IMG_SIZE, ROW * IMG_SIZE, EW_SHOWCONSOLE);


	//图片的加载

	for(int i = 0 ;i < 12; i++)
	{
		char str[20] = { 0 };
		sprintf_s(str, "./images/%d.png", i);
		loadimage(&img[i], str, IMG_SIZE, IMG_SIZE);
	}


	initMap(map);
	while (1)
	{
		mouseEvent(map);
		drawMap(map, img);
		judge_game(map,img);

	}
	return 0;
}