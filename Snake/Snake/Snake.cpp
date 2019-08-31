#include<iostream>
#include<graphics.h>
#include<time.h>
#include<conio.h>

using namespace std;

#define Width 400//定义游戏界面的长和宽
#define Height 300

void AddFood();
void Move();
void Paint();


struct Body
{
	int x;
	int y;
};//定义蛇类
Body snake[1200];//包含蛇身的对象
int len;//蛇身长度
int direct = 3;//其实移动方向

struct Food
{
	int x;
	int y;
};//定义要吃的食物类
Food food;
bool IsFood=true;//是否生成了食物
bool IsOver = false;//游戏是否结束
bool IsChange = true;//接收键盘上的信号后是否响应了

DWORD WINAPI ThreadProc(LPVOID lParam);//创建另一个接收键盘信息的线程
int main()
{
	initgraph(Width, Height);//画界面
	srand((unsigned int)time(0));//随机生成一个食物
	snake[0].x = 0;//初始化蛇头
	snake[0].y = 0;
	len = 1;//初始化蛇身的长度
	AddFood();

	CreateThread(0, 0, ThreadProc, 0, 0, 0);//创建线程
	while (!IsOver)
	{
		Paint();
		Move();
		Sleep(200);
	}
	
	_getch();
	closegraph();
	return 0;

}

void AddFood()
{
	if (!IsFood)
		return;
	IsFood = false;
	bool b = true;

	do
	{
		food.x = rand() % (Width / 10);//随意生成食物
		food.y = rand() % (Height / 10);
		for (int i = 0; i < len ; i++)
		{
			if (food.x == snake[i].x&&food.y == snake[i].y)//如果随机生成的食物与蛇身某处重合，跳出循环
			{
				b = true;
				break;//跳出for循环，重新生成
			}
			else
				b = false;

		}
	} while (b);
}

void Move()
{
	int px = 0;
	int py = 0;
	switch (direct)//定义四个方向蛇头移动规则
	{
	case 0:
		py -= 1;
		break;
	case 1:
		py += 1;
		break;
	case 2:
		px -= 1;
		break;
	case 3:
		px += 1;
		break;
	}

	Body head;//定义蛇头对象
	head.x = snake[0].x + px;
	head.y = snake[0].y + py;
	for (int i = len-1; i >0; i--)//如果蛇头撞向自己，则游戏结束
	{
		snake[i] = snake[i - 1];
		if (head.x == snake[i].x&&head.y == snake[i].y)
		{
			IsOver = true;
			cout << "游戏结束，祝生活愉快！" << endl;
		}
	}
	snake[0].x += px;
	snake[0].y += py;

	if (snake[0].x < 0)//蛇头碰到界面边界时
		snake[0].x = 39;
	if (snake[0].x > 39)
		snake[0].x = 0;
	if (snake[0].y < 0)
		snake[0].y = 29;
	if (snake[0].y >29)
		snake[0].y = 0;

	if (snake[0].x == food.x&&snake[0].y == food.y)//蛇头吃到食物时
	{
		snake[len].x = -1;//防止每次吃到食物，在（0,0）处生成一个蛇头
		snake[len].y = -1;
		len++;
		IsFood = true;
		food.x = -1;
		food.y = -1;
	}
	AddFood();
	IsChange = true;
}

void Paint()
{
	IMAGE img(400, 300);

	SetWorkingImage(&img);
	setfillcolor(RGB(255, 255, 255));
	solidrectangle(0, 0, 400, 300);

	setfillcolor(RGB(0, 255, 0));
	for (int i=0; i < len; i++)
	{
		fillrectangle(snake[i].x * 10, snake[i].y * 10, snake[i].x * 10 + 10, snake[i].y * 10 + 10);
	}

	setfillcolor(RGB(255, 0, 0));
	fillrectangle(food.x * 10, food.y * 10, food.x * 10 + 10, food.y * 10 + 10);

	SetWorkingImage();
	putimage(0, 0, &img);
}

DWORD WINAPI ThreadProc(LPVOID lParam)//创建线程
{
	while (!IsOver)
	{
		if (!IsChange)
			continue;
		char ch = _getch();
		switch (ch)//接受键盘w、s、a、d分别为上下左右
		{
		case'W':
		case'w':
			if (direct == 1)
				break;
			direct = 0;
			IsChange = false;
			break;
		case'S':
		case's':
			if (direct == 0)
				break;
			direct = 1;
			IsChange = false;
			break;
		case'A':
		case'a':
			if (direct == 3)
				break;
			direct = 2;
			IsChange = false;
			break;
		case'D':
		case'd':
			if (direct == 2)
				break;
			direct = 3;
			IsChange = false;
			break;
		}
	}
	return 0;
}